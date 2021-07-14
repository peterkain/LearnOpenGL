#pragma once
#include "Application.h"
#include "Resource.h"

class Shadows : public Application {
	void init() override;
	void renderScene(Shader& shader);
	void loop() override;

	GLuint planeVAO, planeVBO;
	GLuint debugVAO, debugVBO;
	GLuint depthMapFBO, depthMap;
	glm::vec3 lightPos{ -2.f, 4.f, -1.f };
public:
	Shadows(int width, int height, const std::string& title, bool fullscreen = false) :
		Application(width, height, title, fullscreen) {
	}
};