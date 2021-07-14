#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture;

uniform bool inverseColor;
uniform bool grayscale;
uniform bool sharpen;
uniform bool blur;
uniform bool edgeDetect;
uniform bool custom;

uniform float offset;

vec2 offsets[9] = vec2[](
	vec2(-offset, offset),
	vec2(0.f,     offset),
	vec2(offset,  offset),
	vec2(-offset, 0.f   ),
	vec2(0.f,     0.f   ),
	vec2(offset,  0.f   ),
	vec2(-offset, -offset),
	vec2(0.f,     -offset),
	vec2(offset, -offset)
);

float ksharpen[9] = float[](
	-1, -1, -1,
	-1,  9, -1,
	-1, -1, -1
);

float kblur[9] = float[](
	1. / 16, 2. / 16, 1. / 16,
	2. / 16, 4. / 16, 2. / 16,
	1. / 16, 2. / 16, 1. / 16
);

float kedgeDetect[9] = float[](
	1,  1,  1,
	1, -8,  1,
	1,  1,  1
);

float kcustom[9] = float[](
	1, 3, 1,
	3, -15, 3,
	1, 3, 1
);

vec4 calcKernel(float kernel[9]) {
	vec3 sampleTexture[9];
	for (int i = 0; i < 9; i++) {
		sampleTexture[i] = vec3(texture(Texture, TexCoord.st + offsets[i]));
	}
	
	vec3 col = vec3(0.0);

	for (int i = 0; i < 9; i++) {
		col += sampleTexture[i] * kernel[i];
	}

	return vec4(col, 1.0);
}

void main() {
	if (inverseColor) {
		FragColor = vec4(vec3(1.0 - texture(Texture, TexCoord)), 1.0);
	}
	else if (sharpen) {
		FragColor = calcKernel(ksharpen);
	}
	else if (blur) {
		FragColor = calcKernel(kblur);
	}
	else if (edgeDetect) {
		FragColor = calcKernel(kedgeDetect);
	}
	else if (custom) {
		FragColor = calcKernel(kcustom);
	}
	else {
		FragColor = texture(Texture, TexCoord);
		if (grayscale) {
			float avg = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
			FragColor = vec4(avg, avg, avg, 1.0);
		}
	}
}