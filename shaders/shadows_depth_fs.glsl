#version 330 core

void main() {
	float bias = 0.005;
	gl_FragDepth = gl_FragCoord.z;
	gl_FragDepth += gl_FrontFacing ? bias : 0.0;
}