#include "ParticleGenerator.h"
#include "Resource.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <glfw/glfw3.h>


constexpr GLfloat maxv{ 200.f };

ParticleGenerator::ParticleGenerator(int w, int h, std::size_t n) :
	xmax{ w }, ymax{ h }, sizeInBytes{ n * sizeof(glm::vec2) }, n{ n }, deltaTime{} {
#if USE_THREADS
	partLimit = std::max(n / 10, 50000ull);
	stop = false;
	mt_parts = static_cast<short>(std::max(1.f, std::ceilf(n / static_cast<float>(partLimit))));
	mt_n = n / mt_parts;

	threadDone.resize(mt_parts, true);

	for (std::size_t i{ 0 }; i != mt_parts - 1; i++) {
		std::size_t limit{ partLimit * (i + 1) };
		threads.emplace_back([this](std::size_t i, std::size_t limit) {
			while (true) {
				if (!threadDone[i]) {
					for (std::size_t j{ partLimit * i }; j != limit; j++) {
						particles[j].move(xmax, ymax, deltaTime);
					}
					threadDone[i] = true;
				}
				else if (stop) break;
			}
		}, i, limit);
	}

	threads.emplace_back([this]() {
		while (true) {
			if (!threadDone[mt_parts - 1]) {
				for (std::size_t j{ partLimit * (mt_parts - 1) }; j != this->n; j++) {
					particles[j].move(xmax, ymax, deltaTime);
				}
				threadDone[mt_parts - 1] = true;
			}
			else if (stop) break;
		}
	});
#endif

	particles.reserve(n);
	positions.reserve(n);

	std::mt19937 randomEngine{};
	std::uniform_int_distribution<int> xcoord{ 0, xmax };
	std::uniform_int_distribution<int> ycoord{ 0, ymax };
	std::uniform_real_distribution<float> vdist{ -maxv, maxv };

	glm::vec2 v{ vdist(randomEngine), vdist(randomEngine) };
	for (std::size_t i{}; i != n; i++) {
		positions.emplace_back(xcoord(randomEngine), ycoord(randomEngine));
		particles.emplace_back(positions.back(), glm::vec2{ vdist(randomEngine), vdist(randomEngine) });
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, positions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	//glVertexAttribDivisor(0, 1);

	glBindVertexArray(0);

	Resource::shader("particle", std::move(Shader{ "shader/particle_vs.glsl", "shader/particle_fs.glsl" }));
	glPointSize(diameter);
}


#if USE_THREADS
ParticleGenerator::~ParticleGenerator() {
	stop = true;
	joinThreads();
}
#endif


#if USE_THREADS
void ParticleGenerator::setThreadStatus(bool status) {
	for (auto&& e : threadDone) {
		e = status;
	}
}


bool ParticleGenerator::getThreadStatus() {
	bool r{ true };
	for (auto e : threadDone) {
		r &= e;
	}
	return r;
}
#endif


void ParticleGenerator::moveParticles() {
#if USE_THREADS
	setThreadStatus(false);
#else
	for (auto& p : particles) {
		p.move(xmax, ymax, deltaTime);
	}
#endif
}


#if USE_THREADS
void ParticleGenerator::joinThreads() {
	for (auto& t : threads) {
		t.join();
	}
}
#endif


void ParticleGenerator::draw(GLfloat deltaTime) {
	this->deltaTime = deltaTime;
	Shader particle{ Resource::shader("particle") };
	particle.use();

	particle.setFloat("sinTime", std::sinf(glfwGetTime() * 0.8f));
	particle.setMat4("proj", glm::ortho(0.f, static_cast<float>(xmax), static_cast<float>(ymax), 0.f));

	glBindVertexArray(vao);

	glDrawArrays(GL_POINTS, 0, n);

	moveParticles();

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeInBytes, &positions[0]);
	glBindVertexArray(0);
#if USE_THREADS
	while (!getThreadStatus());
#endif
}


void ParticleGenerator::updateBounds(int xmax, int ymax) noexcept {
	this->xmax = xmax;
	this->ymax = ymax;
}