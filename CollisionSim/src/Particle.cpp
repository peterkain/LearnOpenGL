#include "Particle.h"


void Particle::move(int xmax, int ymax, GLfloat deltaTime) {
	p += v * deltaTime;

	if (p.x - radius < 0 || p.x + radius > xmax) {
		p.x = glm::clamp(p.x, 0.f, static_cast<GLfloat>(xmax));
		v.x *= -1;
	}
	if (p.y - radius < 0 || p.y + radius > ymax) {
		p.y = glm::clamp(p.y, 0.f, static_cast<GLfloat>(ymax));
		v.y *= -1;
	}
}