#pragma once

#include "Application.h"

class Instancing : public Application {
	void init() override;
	void loop() override;

	GLuint quadVAO, quadVBO;
	GLuint instanceVBO;
public:
	Instancing(int width, int height, const std::string& title, bool fullscreen = false) :
		Application{ width, height, title, fullscreen } {
	}
};