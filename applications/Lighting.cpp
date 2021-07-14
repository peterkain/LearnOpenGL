#include "Lighting.h"
#include "Resource.h"
#include "Data.h"

glm::vec3 cubePositions[]{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[]{
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

void Lighting::init() {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Data::cubeVTN), Data::cubeVTN, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	Image::flip_vertically(true);

	Resource::shader("lighting", std::move(Shader{ "shaders/light_vs.glsl", "shaders/light_fs.glsl" }));
	Resource::shader("light_source", std::move(Shader{ "shaders/light_vs.glsl", "shaders/light_source_fs.glsl" }));

	Resource::texture("container", std::move(Texture{ "textures/container.png" }));
	Resource::texture("container_specular", std::move(Texture{ "textures/container_specular.png" }));
	Resource::texture("container_emission", std::move(Texture{ "textures/container_emission.jpg" }));

	glClearColor(.04f, .02f, .02f, 1.f);
	glEnable(GL_DEPTH_TEST);
}

void Lighting::loop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view{ camera.GetViewMatrix() };
	glm::mat4 projection{ glm::perspective(glm::radians(camera.zoom), (float)(window.width / window.height), .1f, 100.f) };
	glm::mat4 vp{ projection * view };

	glBindVertexArray(VAO);

	//lightPos.x = sin(glfwGetTime());
	//lightPos.z = cos(glfwGetTime());

	glm::vec3 lightColor{ .5f };
	/*lightColor.x = sin(glfwGetTime() * 2.f);
	lightColor.y = sin(glfwGetTime() * .7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);*/

	glm::vec3 diffuseColor = lightColor;
	glm::vec3 ambientColor = diffuseColor * glm::vec3(.1f);

	Shader lighting{ Resource::shader("lighting") };
	Shader light_source{ Resource::shader("light_source") };

	lighting.use();
	lighting.setVec3("viewPos", camera.position);

	lighting.setInt("material.diffuse", 0);
	lighting.setInt("material.specular", 1);
	lighting.setInt("emission", 2);
	lighting.setFloat("material.shininess", 64.f);
	lighting.setFloat("uTime", glfwGetTime());
	lighting.setBool("blinn", glfwGetKey(window.glfw_window, GLFW_KEY_F1) == GLFW_PRESS);

	lighting.setVec3("dirLight.direction", glm::vec3(-.2f, -.8f, -.5f));
	lighting.setVec3("dirLight.color.ambient", ambientColor);
	lighting.setVec3("dirLight.color.diffuse", diffuseColor);
	lighting.setVec3("dirLight.color.specular", glm::vec3(1.f));

	for (int i{}; i < 4; i++) {
		lighting.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
		lighting.setVec3("pointLights[" + std::to_string(i) + "].color.ambient", ambientColor);
		lighting.setVec3("pointLights[" + std::to_string(i) + "].color.diffuse", diffuseColor);
		lighting.setVec3("pointLights[" + std::to_string(i) + "].color.specular", glm::vec3(1.f));
		lighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.f);
		lighting.setFloat("pointLights[" + std::to_string(i) + "].linear", .09f);
		lighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", .032f);
	}

	lighting.setVec3("spotLight.position", camera.position);
	lighting.setVec3("spotLight.direction", camera.front);
	lighting.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
	lighting.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(14.5f)));
	lighting.setVec3("spotLight.color.ambient", ambientColor);
	lighting.setVec3("spotLight.color.diffuse", glm::vec3(1.f));
	lighting.setVec3("spotLight.color.specular", glm::vec3(1.f));
	lighting.setFloat("spotLight.constant", 1.f);
	lighting.setFloat("spotLight.linear", .09f);
	lighting.setFloat("spotLight.quadratic", .032f);

	glActiveTexture(GL_TEXTURE0);
	Resource::texture("container").bind();
	glActiveTexture(GL_TEXTURE1);
	Resource::texture("container_specular").bind();
	glActiveTexture(GL_TEXTURE2);
	Resource::texture("container_emission").bind();

	for (int i{}; i < 10; i++) {
		glm::mat4 model{ glm::mat4(1.f) };
		model = glm::translate(model, cubePositions[i]);
		float angle{ 20.f * i };
		model = glm::rotate(model, glm::radians(angle) * (i % 2 == 0 ? (float)glfwGetTime() : 1),
			glm::vec3(1.f, .3f, .5f));
		lighting.setMat4("mvp", vp * model);
		lighting.setMat4("model", model);
		lighting.setMat4("imodel", glm::inverse(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	for (int i{}; i < 4; i++) {
		glm::mat4 model{ 1.f };
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(.2f));

		light_source.use();
		light_source.setMat4("mvp", vp * model);
		light_source.setVec3("LightColor", lightColor);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}