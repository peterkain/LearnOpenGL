#pragma once
#include "Application.h"
#include <vector>
#include <glm/glm.hpp>

class AsteroidField : public Application {
	void init() override;
	void loop() override;

	GLuint instancedVBO;
	/*std::vector<glm::vec3> displacements;
	std::vector<glm::vec3> rotations;
	std::vector<float> scales;*/
	std::vector<glm::mat4> modelMatrices;
public:
	AsteroidField(int width, int height, const std::string& title, bool fullscreen = false) :
		Application{ width, height, title, fullscreen } {
	}
};