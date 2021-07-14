#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 Color;

//uniform vec2 offsets[100];

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec2 pos = aPos * (gl_InstanceID / 100.0);
	//vec2 offset = offsets[gl_InstanceID];
	gl_Position = projection * view * vec4(pos + aOffset, 0.0, 1.0);
	Color = aColor;
}