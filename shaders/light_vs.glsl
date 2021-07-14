#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 imodel;

void main() {
	gl_Position = mvp * vec4(aPos, 1.0);
	Normal = mat3(transpose(imodel)) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoord = aTexCoord;
}