#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
	vec2 TexCoord;
	vec3 Normal;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

uniform mat4 projview;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));

	// Equals Inverse, because TBN is an orthogonal matrix
	mat3 TBN = transpose(mat3(T, B, N));

	vec3 fragPos = vec3(model * vec4(aPos, 1.0));

	vs_out.TexCoord = aTexCoord;
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos  = TBN * viewPos;
	vs_out.TangentFragPos  = TBN * fragPos;
	gl_Position = projview * vec4(fragPos, 1.0);
}