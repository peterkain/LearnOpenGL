#pragma once

#include "Application.h"

class AdvancedOpenGL : public Application {
	void init() override;
	void loop() override;

	GLuint VBO[3];
	GLuint VAO[4];
	GLuint fbo;
	GLuint textureBuffer;
public:
	AdvancedOpenGL(int width, int height, const std::string& title, bool fullscreen = false) :
		Application(width, height, title, fullscreen) {
	}
};