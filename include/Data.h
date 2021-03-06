#pragma once

#include <glad/glad.h>

namespace Data {
	static GLfloat cubeVTN[] {
		 1.f,  1.f, -1.f,  1.0f, 1.0f,  0.f, 0.f, -1.f,
		 1.f, -1.f, -1.f,  1.0f, 0.0f,  0.f, 0.f, -1.f,
		-1.f, -1.f, -1.f,  0.0f, 0.0f,  0.f, 0.f, -1.f,
		-1.f, -1.f, -1.f,  0.0f, 0.0f,  0.f, 0.f, -1.f,
		-1.f,  1.f, -1.f,  0.0f, 1.0f,  0.f, 0.f, -1.f,
		 1.f,  1.f, -1.f,  1.0f, 1.0f,  0.f, 0.f, -1.f,

		-1.f, -1.f,  1.f,  0.0f, 0.0f,  0.f, 0.f, 1.f,
		 1.f, -1.f,  1.f,  1.0f, 0.0f,  0.f, 0.f, 1.f,
		 1.f,  1.f,  1.f,  1.0f, 1.0f,  0.f, 0.f, 1.f,
		 1.f,  1.f,  1.f,  1.0f, 1.0f,  0.f, 0.f, 1.f,
		-1.f,  1.f,  1.f,  0.0f, 1.0f,  0.f, 0.f, 1.f,
		-1.f, -1.f,  1.f,  0.0f, 0.0f,  0.f, 0.f, 1.f,

		-1.f,  1.f,  1.f,  1.0f, 0.0f,  -1.f, 0.f, 0.f,
		-1.f,  1.f, -1.f,  1.0f, 1.0f,  -1.f, 0.f, 0.f,
		-1.f, -1.f, -1.f,  0.0f, 1.0f,  -1.f, 0.f, 0.f,
		-1.f, -1.f, -1.f,  0.0f, 1.0f,  -1.f, 0.f, 0.f,
		-1.f, -1.f,  1.f,  0.0f, 0.0f,  -1.f, 0.f, 0.f,
		-1.f,  1.f,  1.f,  1.0f, 0.0f,  -1.f, 0.f, 0.f,

		 1.f, -1.f, -1.f,  0.0f, 1.0f,  1.f, 0.f, 0.f,
		 1.f,  1.f, -1.f,  1.0f, 1.0f,  1.f, 0.f, 0.f,
		 1.f,  1.f,  1.f,  1.0f, 0.0f,  1.f, 0.f, 0.f,
		 1.f,  1.f,  1.f,  1.0f, 0.0f,  1.f, 0.f, 0.f,
		 1.f, -1.f,  1.f,  0.0f, 0.0f,  1.f, 0.f, 0.f,
		 1.f, -1.f, -1.f,  0.0f, 1.0f,  1.f, 0.f, 0.f,

		-1.f, -1.f, -1.f,  0.0f, 1.0f,  0.f, -1.f, 0.f,
		 1.f, -1.f, -1.f,  1.0f, 1.0f,  0.f, -1.f, 0.f,
		 1.f, -1.f,  1.f,  1.0f, 0.0f,  0.f, -1.f, 0.f,
		 1.f, -1.f,  1.f,  1.0f, 0.0f,  0.f, -1.f, 0.f,
		-1.f, -1.f,  1.f,  0.0f, 0.0f,  0.f, -1.f, 0.f,
		-1.f, -1.f, -1.f,  0.0f, 1.0f,  0.f, -1.f, 0.f,

		 1.f,  1.f,  1.f,  1.0f, 0.0f,  0.f, 1.f, 0.f,
		 1.f,  1.f, -1.f,  1.0f, 1.0f,  0.f, 1.f, 0.f,
		-1.f,  1.f, -1.f,  0.0f, 1.0f,  0.f, 1.f, 0.f,
		-1.f,  1.f, -1.f,  0.0f, 1.0f,  0.f, 1.f, 0.f,
		-1.f,  1.f,  1.f,  0.0f, 0.0f,  0.f, 1.f, 0.f,
		 1.f,  1.f,  1.f,  1.0f, 0.0f,  0.f, 1.f, 0.f
	};

	static GLfloat planeVT[] {
		-5.0f, -0.5f, -5.0f,  0.0f, 5.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		 5.0f, -0.5f,  5.0f,  5.0f, 0.0f,

		 5.0f, -0.5f,  5.0f,  5.0f, 0.0f,
		 5.0f, -0.5f, -5.0f,  5.0f, 5.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 5.0f
	};

	static GLfloat planeVTN[]{
		-5.0f, -0.5f, -5.0f,  0.0f, 5.0f,  0.f, 1.f, 0.f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,  0.f, 1.f, 0.f,
		 5.0f, -0.5f,  5.0f,  5.0f, 0.0f,  0.f, 1.f, 0.f,

		 5.0f, -0.5f,  5.0f,  5.0f, 0.0f,  0.f, 1.f, 0.f,
		 5.0f, -0.5f, -5.0f,  5.0f, 5.0f,  0.f, 1.f, 0.f,
		-5.0f, -0.5f, -5.0f,  0.0f, 5.0f,  0.f, 1.f, 0.f
	};

	static GLfloat norm_planeVTN[]{
		-1.0f, 0.f, -1.0f,  0.0f, 1.0f,  0.f, 1.f, 0.f,
		-1.0f, 0.f,  1.0f,  0.0f, 0.0f,  0.f, 1.f, 0.f,
		 1.0f, 0.f,  1.0f,  1.0f, 0.0f,  0.f, 1.f, 0.f,
		 	   		 
		 1.0f, 0.f,  1.0f,  1.0f, 0.0f,  0.f, 1.f, 0.f,
		 1.0f, 0.f, -1.0f,  1.0f, 1.0f,  0.f, 1.f, 0.f,
		-1.0f, 0.f, -1.0f,  0.0f, 1.0f,  0.f, 1.f, 0.f
	};

	static GLfloat quadVT[] {
		 1.f, -1.f, 0.f,  1.f, 0.f,
		 1.f,  1.f, 0.f,  1.f, 1.f,
		-1.f,  1.f, 0.f,  0.f, 1.f,

		-1.f,  1.f, 0.f,  0.f, 1.f,
		-1.f, -1.f, 0.f,  0.f, 0.f,
		 1.f, -1.f, 0.f,  1.f, 0.f
	};

	static GLfloat quadVTN[]{
		 1.f, -1.f, 0.f,  1.f, 0.f,  0.f, 0.f, 1.f,
		 1.f,  1.f, 0.f,  1.f, 1.f,	 0.f, 0.f, 1.f,
		-1.f,  1.f, 0.f,  0.f, 1.f,	 0.f, 0.f, 1.f,

		-1.f,  1.f, 0.f,  0.f, 1.f,	 0.f, 0.f, 1.f,
		-1.f, -1.f, 0.f,  0.f, 0.f,	 0.f, 0.f, 1.f,
		 1.f, -1.f, 0.f,  1.f, 0.f,	 0.f, 0.f, 1.f
	};

	static GLfloat skyboxV[] {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	static GLfloat testPlane[] {
		1.f, 0.f, 1.f,
		-1.f, 0.f, -1.f,
		-1.f, 0.f, 1.f,

		1.f, 0.f, 1.f,
		1.f, 0.f, -1.f,
		-1.f, 0.f, -1.f
	};

	static GLfloat insQuadVC[]{
		-.05f,  .05f,  1.f, 0.f, 0.f,
		 .05f, -.05f,  0.f, 1.f, 0.f,
		-.05f, -.05f,  0.f, 0.f, 1.f,

		-.05f,  .05f,  1.f, 0.f, 0.f,
		 .05f, -.05f,  0.f, 1.f, 0.f,
		 .05f,  .05f,  0.f, 1.f, 1.f
	};
}