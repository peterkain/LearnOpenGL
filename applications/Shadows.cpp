#include "Shadows.h"
#include "Data.h"


constexpr unsigned SHADOW_MAP_SIZE{ 2048 };

void Shadows::init() {
	Resource::mesh("cube", std::move(Mesh{ "models/cube.obj" }));
	Resource::shader("shadows", std::move(Shader{ "shaders/shadows_vs.glsl", "shaders/shadows_fs.glsl" }));
	Resource::shader("depthShader", std::move(Shader{ "shaders/shadows_depth_vs.glsl", "shaders/shadows_depth_fs.glsl" }));
	Resource::shader("debug", std::move(Shader{ "shaders/shadows_debug_vs.glsl", "shaders/shadows_debug_fs.glsl" }));
	Resource::texture("alhambra", std::move(Texture{ "textures/alhambra.jpg" }));

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Data::planeVTN), Data::planeVTN, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &debugVAO);
	glGenBuffers(1, &debugVBO);

	glBindVertexArray(debugVAO);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Data::quadVT), Data::quadVT, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.f, 1.f, 1.f, 1.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glClearColor(.1f, .1f, .1f, 1.f);
}


void Shadows::renderScene(Shader& shader) {
	glBindVertexArray(planeVAO);
	shader.setMat4("model", glm::scale(glm::mat4{ 1.f }, glm::vec3{ 5.f, 1.f, 5.f }));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	Mesh cube{ Resource::mesh("cube") };
	glBindVertexArray(cube.VAO);
	glm::mat4 model;
	model = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 0.f, 1.5f, 0.f });
	model = glm::scale(model, glm::vec3{ .5f });
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, cube.count);

	model = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 2.f, 0.f, 1.f });
	model = glm::scale(model, glm::vec3{ .5f });
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, cube.count);

	glBindVertexArray(0);
}


void Shadows::loop() {
	Shader shadows{ Resource::shader("shadows") };
	Shader depth{ Resource::shader("depthShader") };
	Shader debug{ Resource::shader("debug") };

	lightPos.x = sin(glfwGetTime()) * 2.f;
	lightPos.z = cos(glfwGetTime()) * 2.f;
	GLfloat near{ 1.f }, far{ 100.f };
	glm::mat4 lightProj{ glm::ortho(-10.f, 10.f, -10.f, 10.f, near, far) };
	glm::mat4 lightView{ glm::lookAt(
		lightPos,
		glm::vec3{ 0.f, 0.f, 0.f },
		glm::vec3{ 0.f, 1.f, 0.f }
	)};
	glm::mat4 lightSpaceMatrix{ lightProj * lightView };

	depth.use();
	depth.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	Resource::texture("alhambra").bind();
	renderScene(depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, window.width, window.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shadows.use();

	shadows.setMat4("proj", glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f));
	shadows.setMat4("view", camera.GetViewMatrix());
	shadows.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	shadows.setVec3("lightPos", lightPos);
	shadows.setVec3("viewPos", camera.position);
	shadows.setInt("Texture", 0);
	shadows.setInt("ShadowMap", 1);

	glActiveTexture(GL_TEXTURE0);
	Resource::texture("alhambra").bind();
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	renderScene(shadows);

	/*debug.use();
	debug.setInt("Texture", 0);
	glBindVertexArray(debugVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);*/
}