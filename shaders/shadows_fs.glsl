#version 330 core

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform sampler2D Texture;
uniform sampler2D ShadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
	// perspective divide (from gl_Position)
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// from [-1,1] to [0,1] in order to sample depthMap
	projCoords = projCoords * 0.5 + 0.5;

	if (projCoords.z > 1.0) {
		return 0.0;
	}

	float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
		}
	}

	//float closestDepth = texture(ShadowMap, projCoords.xy).r;

	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	//float bias = 0.001;
	//return currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow / 9.0;
}

void main() {
	vec3 color = texture(Texture, fs_in.TexCoord).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);

	vec3 ambient = 0.15 * color;

	//vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 lightDir = normalize(lightPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0);
}