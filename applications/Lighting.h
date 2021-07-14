#pragma once

#include "Application.h"

class Lighting : public Application {
	void init() override;
	void loop() override;

	GLuint VBO, VAO;
public:
	Lighting(int width, int height, const std::string& title, bool fullscreen = false) :
		Application(width, height, title, fullscreen) {
	}
};