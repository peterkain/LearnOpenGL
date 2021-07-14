#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture;

void main() {
	float depth = texture(Texture, TexCoord).r;
	FragColor = vec4(vec3(depth), 1.0);
}