#pragma once
#include "Particle.h"
#include <vector>
#include <thread>
#define USE_THREADS 1


class ParticleGenerator {
	std::vector<Particle> particles;
	std::vector<glm::vec2> positions;
	int xmax, ymax;
	GLuint vao, vbo;
	std::size_t n, sizeInBytes;
	GLfloat deltaTime;
#if USE_THREADS
	std::size_t partLimit;
	std::size_t mt_n;
	short mt_parts;
	bool stop;

	std::vector<std::thread> threads;
	std::vector<bool> threadDone;

	void joinThreads();
	void setThreadStatus(bool status);
	bool getThreadStatus();
#endif

	void moveParticles();
public:
	ParticleGenerator(int w, int h, std::size_t n);
#if USE_THREADS
	~ParticleGenerator();
#endif

	void draw(GLfloat deltaTime);
	void updateBounds(int xmax, int ymax) noexcept;
};