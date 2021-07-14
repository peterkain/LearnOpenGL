#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

//out VS_OUT {
//	vec2 uv;
//	vec3 normal;
//} vs_out;

out vec2 TexCoord;
out float Height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D Heightmap;

uniform float uTime;
uniform float startingHeight;

void main(void) {
	//float currHeight = texelFetch(Heightmap, ivec2(aPos) % ivec2(128), 0).r;
	//height = currHeight;

//	vs_out.uv = aTexCoord;
//	vs_out.normal = aNormal;
	TexCoord = aTexCoord;
	Height = aPos.y;

//	if (Height < startingHeight) {
//		Height = aPos.y + sin(uTime + aPos.x * aPos.z) / 4;
//	}

	gl_Position = projection * view * model * vec4(aPos.x, Height, aPos.z, 1.0);
}