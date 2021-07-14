#include "ParticleSim.h"

constexpr int width{ 3440 };
constexpr int height{ 1440 };

int main() {
	ParticleSim app{ width, height, "Particle Sim", true };
	app.run();
}