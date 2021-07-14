#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 pos = projection * view * vec4(aPos, 1.0);
	TexCoord = aPos;
	gl_Position = pos.xyww;
}