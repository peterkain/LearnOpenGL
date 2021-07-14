#version 330 core

//in VS_OUT {
//	vec2 uv;
//	vec3 normal;
//} fs_in;

in vec2 TexCoord;
in float Height;

out vec4 Color;

uniform sampler2D Texture;
uniform float startingHeight;

void main(void) {
//	vec3 RGB = vec3(.1, .1, .1);
//	if (Height < startingHeight) {
//		RGB = vec3(0., 0., 1.);
//	}
//	else if (Height < startingHeight * 2) {
//		RGB = vec3(0., 1., 0.);
//	}
//	else {
//		RGB = vec3(.8, .8, .8);
//	}
//
//	if (gl_FragCoord.y + startingHeight < startingHeight) {
//		RGB = vec3(0., 0., 1.);
//		RGB = vec3(0., 0., 1.);
//	}

	vec3 RGB = vec3(0., 1., 0.);

	Color = mix(texture(Texture, TexCoord), vec4(RGB, 1.0), 0.5);
}