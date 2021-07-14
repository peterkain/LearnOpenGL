#include "AsteroidField.h"
#include "Resource.h"


constexpr unsigned amount{ 10000 };
constexpr float radius{ 70.f };
bool printed{};

void AsteroidField::init() {
	Resource::mesh("planet", std::move(Mesh{ "models/planet.obj" }));
	Resource::mesh("asteroid", std::move(Mesh{ "models/rock.obj" }));
	Resource::texture("rock", std::move(Texture{ "textures/rock.png" }));
	Resource::texture("mars", std::move(Texture{ "textures/mars.png" }));
	Resource::shader("asteroids", std::move(Shader{ "shaders/asteroids_vs.glsl", "shaders/asteroids_fs.glsl" }));
	Resource::shader("model", std::move(Shader{ "shaders/model_vs.glsl", "shaders/model_fs.glsl" }));

	glGenBuffers(1, &instancedVBO);
	glBindVertexArray(Resource::mesh("asteroid").VAO);

	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, 0, GL_STREAM_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, 0, GL_STATIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	srand(glfwGetTime());
	/*displacements.reserve(amount);
	rotations.reserve(amount);
	scales.reserve(amount);*/
	modelMatrices.resize(amount);

	float offset{ 25.f };
	glm::vec3 displacement;
	glm::vec3 rotation;
	float scale;
	for (int i{}; i < amount; i++) {
		float angle{ (float)i / (float)amount * 360.f };
		glm::mat4 model{ 1.f };
		displacement.x = (rand() % (int)(2 * 100 * offset)) / 100.f - offset;
		displacement.y = (rand() % (int)(2 * 100 * offset)) / 100.f - offset;
		displacement.z = (rand() % (int)(2 * 100 * offset)) / 100.f - offset;
		//displacements.push_back(displacement);

		glm::vec3 translation;

		translation.x = displacement.x + sin(angle) * radius;
		translation.y = displacement.y * .3f;
		translation.z = displacement.z + cos(angle) * radius;

		model = glm::translate(model, translation);

		rotation.x = (rand() % 15) / 10.f;
		rotation.y = (rand() % 15) / 10.f;
		rotation.z = (rand() % 15) / 10.f;
		//rotations.push_back(rotation);

		model = glm::rotate(model, (float)(rand() % 360), rotation);

		scale = (rand() % 10) / 100.f + .025f;
		//scales.push_back(scale);

		model = glm::scale(model, glm::vec3{ scale });

		modelMatrices[i] = model;
	}

	glBindVertexArray(Resource::mesh("asteroid").VAO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * modelMatrices.size(), &modelMatrices[0]);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
}

void AsteroidField::loop() {
	glClearColor(.1f, .1f, .1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mesh asteroid{ Resource::mesh("asteroid") };
	Shader asteroids{ Resource::shader("asteroids") };
	asteroids.use();

	glm::mat4 view{ camera.GetViewMatrix() };
	glm::mat4 proj{ glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f) };

	asteroids.setMat4("vp", proj * view);

	glActiveTexture(GL_TEXTURE0);
	Resource::texture("rock").bind();
	glBindVertexArray(asteroid.VAO);

	/*for (int i{}; i < amount; i++) {
		float angle{ (float)i / (float)amount * 360.f };
		glm::mat4 model{ 1.f };
		glm::vec3 displacement{ displacements[i] };
		glm::vec3 translation;

		// Redo transformation (model[t][x/z])
		translation.x = displacement.x + sin(angle + glfwGetTime() * .1f) * radius;
		translation.y = displacement.y;
		translation.z = displacement.z + cos(angle + glfwGetTime() * .1f) * radius;

		model = glm::translate(model, translation);
		model = glm::rotate(model, (float)glfwGetTime() * .5f, rotations[i]);
		model = glm::scale(model, glm::vec3{ scales[i] });

		modelMatrices[i] = model;
	}*/

	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * amount, &modelMatrices[0]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, asteroid.count, amount);

	Mesh planet{ Resource::mesh("planet") };
	Shader model{ Resource::shader("model") };
	model.use();

	model.setMat4("vp", proj * view);
	model.setMat4("model", glm::mat4{ 1.f });

	Resource::texture("mars").bind();
	glBindVertexArray(planet.VAO);
	glDrawArrays(GL_TRIANGLES, 0, planet.count);

	if ((int)(glfwGetTime()) % 2 != 0 && !printed) {
		std::cerr << "FPS: " << 1.f / deltaTime << '\n';
		printed = true;
	}
	else if ((int)(glfwGetTime()) % 2 == 0 && printed) {
		printed = false;
	}
}