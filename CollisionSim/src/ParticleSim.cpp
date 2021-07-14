#include "ParticleSim.h"
#include <iostream>


void ParticleSim::init() {
}


void ParticleSim::loop() {
	glClear(GL_COLOR_BUFFER_BIT);

	particles.draw(deltaTime);

	std::cout << 1 / deltaTime << '\n';
}