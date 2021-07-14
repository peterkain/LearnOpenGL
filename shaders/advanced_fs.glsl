#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture;
uniform bool visualizeDepth;
uniform bool drawOutline;

float near = .1;
float far = 100.;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; 
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
	if (visualizeDepth) {
		float depth = LinearizeDepth(gl_FragCoord.z) / far;
		FragColor = vec4(vec3(depth), 1.0);
	}
	else {
		if (!drawOutline)
			FragColor = texture(Texture, TexCoord);
		else
			FragColor = vec4(.5, .2, .2, 1.);
	}

	if (FragColor.a < 0.1) {
		discard;
	}
}