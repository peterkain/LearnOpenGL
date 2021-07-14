#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

uniform mat4 projview;
uniform mat4 model;

uniform bool reverseNormal;

void main() {
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	if (reverseNormal)
		vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
	else
		vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;
	gl_Position = projview * vec4(vs_out.FragPos, 1.0);
}