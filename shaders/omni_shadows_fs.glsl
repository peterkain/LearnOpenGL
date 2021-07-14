#version 330 core

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D Texture;
uniform samplerCube ShadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

float ShadowCalculation(vec3 fragPos) {
	vec3 lightToFrag = fragPos - lightPos;
	float closestDepth = texture(ShadowMap, lightToFrag).r * far_plane;
	float currentDepth = length(lightToFrag);

	float bias = 0.05;
	return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

void main() {
	vec3 color = texture(Texture, fs_in.TexCoord).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(0.8, 0.8, 0.4);

	vec3 ambient = 0.3 * color;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * lightColor;

	float shadow = ShadowCalculation(fs_in.FragPos);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0);
}