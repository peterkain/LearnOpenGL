#pragma once
#include "Application.h"

class HeightMapVisualizer : public Application {
	void init() override;
	void loop() override;

	GLuint VBO, VAO, EBO;
	std::size_t count;
	float startingHeight;
public:
	HeightMapVisualizer(int width, int height, const std::string& title, bool fullscreen = false) :
		Application(width, height, title, fullscreen) {
	}
};