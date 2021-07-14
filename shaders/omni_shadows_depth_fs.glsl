#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
	vec3 lightToFrag = FragPos.xyz - lightPos;
	float lightDistance = length(lightToFrag) / far_plane;

	gl_FragDepth = lightDistance;
}