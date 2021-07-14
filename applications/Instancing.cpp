#include "Instancing.h"
#include "Data.h"
#include "Resource.h"


void Instancing::init() {
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(Data::insQuadVC), Data::insQuadVC, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	Resource::shader("instancing", std::move(Shader{ "shaders/instancing_vs.glsl", "shaders/instancing_fs.glsl" }));
	/*Shader instancing{ Resource::shader("instancing") };
	instancing.use();*/

	std::vector<glm::vec2> translations;

	float offset = .1f;
	glm::vec2 translation{};
	unsigned i{ 0 };
	for (int y{ -5 }; y < 5; y++) {
		translation.y = (float)y / 5.f + offset;
		for (int x{ -5 }; x < 5; x++) {
			translation.x = (float)x / 5.f + offset;
			translations.push_back(translation);
			//instancing.setVec2("offsets[" + std::to_string(i++) + "]", translation);
		}
	}

	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);
}


void Instancing::loop() {
	glClearColor(.1f, .1f, .1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	Shader instancing{ Resource::shader("instancing") };
	instancing.use();

	//instancing.setMat4("projection", glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f));
	instancing.setMat4("projection", glm::mat4{ 1.f });
	instancing.setMat4("view", camera.GetViewMatrix());
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}