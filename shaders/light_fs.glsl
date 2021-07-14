#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct LightColor {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight {
	vec3 direction;

	LightColor color;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	LightColor color;
};

struct SpotLight {
	float innerCutOff;
	float outerCutOff;
	vec3 direction;

	vec3 position;

	float constant;
	float linear;
	float quadratic;

	LightColor color;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;

uniform Material material;
uniform float uTime;
uniform bool blinn;

#define _POINT_LIGHTS 4
uniform PointLight pointLights[_POINT_LIGHTS];
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;


float calcSpec(vec3 lightDir, vec3 normal, vec3 viewDir) {
	if (blinn) {
		// Blinn-Phong
		vec3 halfway = normalize(lightDir + viewDir);  // lightDir zeigt zu Fragment
		return pow(max(dot(normal, halfway), 0.0), material.shininess * 2.5);  // Mehr shininess bei Blinn-Phong
	}
	else {
		// Phong
		vec3 reflectDir = reflect(-lightDir, normal);  // lightDir zeigt zu Fragment
		return pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}
}


vec3 DirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	float spec = calcSpec(lightDir, normal, viewDir);

	vec3 ambient  = light.color.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse  = light.color.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.color.specular * spec * vec3(texture(material.specular, TexCoord));

	return ambient + diffuse + specular;
}


vec3 PointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	float spec = calcSpec(lightDir, normal, viewDir);

	float d = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

	vec3 ambient  = light.color.ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation;
	vec3 diffuse  = light.color.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff * attenuation;
	vec3 specular = light.color.specular * vec3(texture(material.specular, TexCoord)) * spec * attenuation;

	return ambient + diffuse + specular;
}


vec3 SpotLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(normalize(light.direction), -lightDir);
	float epsilon = light.outerCutOff - light.innerCutOff;
	float intensity = clamp((light.outerCutOff - theta) / epsilon, 0.0, 1.0);

	float diff = max(dot(normal, lightDir), 0.0);

	float spec = calcSpec(lightDir, normal, viewDir);

	float d = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

	vec3 ambient  = light.color.ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation * intensity;
	vec3 diffuse  = light.color.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff * attenuation * intensity;
	vec3 specular = light.color.specular * vec3(texture(material.specular, TexCoord)) * spec * attenuation * intensity;

	return ambient + diffuse + specular;
}


void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = DirectionalLighting(dirLight, norm, viewDir);
	for (int i = 0; i < _POINT_LIGHTS; i++) {
		result += PointLighting(pointLights[i], norm, FragPos, viewDir);
	}

	result += SpotLighting(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}