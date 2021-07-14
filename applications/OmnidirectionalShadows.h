#pragma once
#include "Application.h"
#include "Resource.h"
#include <array>

class OmnidirectionalShadows : public Application {
	void init() override;
	void drawScene(Shader& shader);
	void loop() override;

	GLuint framebuffer, depthMap;
	glm::mat4 shadowProj;
	std::array<glm::mat4, 6> shadowTransforms;
	glm::vec3 lightPos;
	float far_plane;
public:
	OmnidirectionalShadows(int width, int height, const std::string& title, bool fullscreen = false) :
		Application{ width, height, title, fullscreen }, lightPos{ 0.f }, far_plane{ 25.f } {
	}
};