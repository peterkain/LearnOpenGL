#pragma once
#include "Application.h"

class NormalMapping : public Application {
	void init() override;
	void loop() override;

	GLuint vao, vbo;
public:
	NormalMapping(int width, int height, const std::string& title, bool fullscreen = false) :
		Application{ width, height, title, fullscreen } {
	}
};
