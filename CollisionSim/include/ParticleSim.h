#pragma once
#include "Application.h"
#include "ParticleGenerator.h"


constexpr unsigned numParticles{ 1000000 };


class ParticleSim : public Application {
	void init() override;
	void loop() override;

	ParticleGenerator particles;
public:
	ParticleSim(int width, int height, const std::string& title, bool fullscreen = false) :
		Application{ width, height, title, fullscreen }, particles{ width, height, numParticles } {
		//window.set_input_mode(GLFW_CURSOR_NORMAL);
	}
};