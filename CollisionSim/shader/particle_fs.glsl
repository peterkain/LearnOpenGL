#version 330 core

in vec3 Color;
out vec4 FragColor;

uniform float sinTime;

void main() {
	float f = 1 - sinTime;
	FragColor = vec4(f * (gl_FragCoord.y) * 0.02 + 0.5, f * (1440 - gl_FragCoord.y) * 0.02, f * (gl_FragCoord.x) * 0.02, 1.0);
}