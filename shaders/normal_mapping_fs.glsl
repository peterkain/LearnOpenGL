#version 330 core

in VS_OUT {
	vec2 TexCoord;
	vec3 Normal;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D Texture;
uniform sampler2D NormalMap;
uniform sampler2D DepthMap;

uniform bool normalMapping;
uniform bool parallaxMapping;

uniform float height_scale;

//vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir) {
//	float height = texture(DepthMap, texCoord).r;
//	vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
//	return texCoord - p;
//}

//vec2 SteepParallaxMapping(vec2 texCoord, vec3 viewDir) {
//	const float minLayers = 5.0;
//	const float maxLayers = 150.0;
//
//	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
//	float layerDepth = 1.0 / numLayers;
//	float currLayerDepth = 0.0;
//
//	vec2 P = viewDir.xy * height_scale;
//	// Amount of Steps on P
//	vec2 deltaTexCoord = P / numLayers;
//
//	vec2 currTexCoord = texCoord;
//	float currDepthMapValue = texture(DepthMap, currTexCoord).r;
//
//	while (currLayerDepth < currDepthMapValue) {
//		currTexCoord -= deltaTexCoord;
//		currDepthMapValue = texture(DepthMap, currTexCoord).r;
//		currLayerDepth += layerDepth;
//	}
//
//	return currTexCoord;
//}

vec2 ParallaxOcclusionMapping(vec2 texCoord, vec3 viewDir) {
	// *************************************
	// Steep Parallax Mapping
	const float minLayers = 8.0;
	const float maxLayers = 32.0;

	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
	float layerDepth = 1.0 / numLayers;
	float currLayerDepth = 0.0;

	vec2 P = viewDir.xy * height_scale;
	// Amount of Steps on P
	vec2 deltaTexCoord = P / numLayers;

	vec2 currTexCoord = texCoord;
	float currDepthMapValue = texture(DepthMap, currTexCoord).r;

	while (currLayerDepth < currDepthMapValue) {
		currTexCoord -= deltaTexCoord;
		currDepthMapValue = texture(DepthMap, currTexCoord).r;
		currLayerDepth += layerDepth;
	}

	// **************************************
	// Parallax Occlusion Mapping
	vec2 prevTexCoord = currTexCoord + deltaTexCoord;

	float depthAfter = currDepthMapValue - currLayerDepth;
	float depthBefore = texture(DepthMap, prevTexCoord).r - currLayerDepth + layerDepth;

	float weight = depthAfter / (depthAfter - depthBefore);
	vec2 finalTexCoord = prevTexCoord * weight + currTexCoord * (1.0 - weight);

	return finalTexCoord;
}

void main() {
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoord = ParallaxOcclusionMapping(fs_in.TexCoord, viewDir);

	if (texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
		discard;

	vec3 normal;
	vec3 color;

	if (normalMapping) {
		color = texture(Texture, fs_in.TexCoord).rgb;
		normal = texture(NormalMap, fs_in.TexCoord).rgb;
		normal = normal * 2.0 - 1.0;
	}
	else if (parallaxMapping) {
		color = texture(Texture, texCoord).rgb;
		normal = texture(NormalMap, texCoord).rgb;
		normal = normal * 2.0 - 1.0;
	}
	else {
		color = texture(Texture, fs_in.TexCoord).rgb;
		normal = normalize(fs_in.Normal);
	}

	float ambient = .15f;

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);

	FragColor = vec4((ambient + diff + spec) * color, 1.0);
}