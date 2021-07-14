#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>


constexpr GLfloat diameter{ 1.f };
constexpr GLfloat radius{ diameter * 0.5f };


struct Particle {
	glm::vec2& p;
	glm::vec2 v;

	Particle(glm::vec2& p, glm::vec2 v) :
		p{ p }, v{ v } {
	}

	void move(int xmax, int ymax, GLfloat deltaTime);
};