#include "OmnidirectionalShadows.h"


constexpr unsigned SHADOW_SIZE{ 2048 };


void OmnidirectionalShadows::init() {
	Resource::mesh("cube", std::move(Mesh{ "models/cube.obj" }));
	Resource::shader("shader", std::move(Shader{ "shaders/omni_shadows_vs.glsl", "shaders/omni_shadows_fs.glsl" }));
	Resource::shader("depth", std::move(Shader{ "shaders/omni_shadows_depth_vs.glsl", "shaders/omni_shadows_depth_gs.glsl",
		"shaders/omni_shadows_depth_fs.glsl" }));
	Resource::texture("container", std::move(Texture{ "textures/container.png" }));

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (short i{}; i != 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float aspect{ 1.f }; // SHADOW_SIZE / SHADOW_SIZE
	float near{ 1.f };

	shadowProj = glm::perspective(glm::radians(90.f), aspect, near, far_plane);

	shadowTransforms[0] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3{ 1.f,  0.f,  0.f }, glm::vec3{ 0.f, -1.f,  0.f });
	shadowTransforms[1] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3{-1.f,  0.f,  0.f }, glm::vec3{ 0.f, -1.f,  0.f });
	shadowTransforms[2] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3{ 0.f,  1.f,  0.f }, glm::vec3{ 0.f,  0.f,  1.f });
	shadowTransforms[3] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3{ 0.f, -1.f,  0.f }, glm::vec3{ 0.f,  0.f, -1.f });
	shadowTransforms[4] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3{ 0.f,  0.f,  1.f }, glm::vec3{ 0.f, -1.f,  0.f });
	shadowTransforms[5] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3{ 0.f,  0.f, -1.f }, glm::vec3{ 0.f, -1.f,  0.f });

	glClearColor(.1f, .1f, .1f, 1.f);
	glEnable(GL_DEPTH_TEST);
}


void OmnidirectionalShadows::drawScene(Shader& shader) {
	Mesh cube{ Resource::mesh("cube") };
	glBindVertexArray(cube.VAO);

	glm::mat4 model{ glm::scale(glm::mat4{ 1.f }, glm::vec3{ 7.f, 4.f, 7.f }) };
	shader.setMat4("model", model);
	shader.setBool("reverseNormal", true);

	glDrawArrays(GL_TRIANGLES, 0, cube.count);

	model = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 5.f, 2.f, 5.f });
	shader.setMat4("model", model);
	shader.setBool("reverseNormal", false);

	glDrawArrays(GL_TRIANGLES, 0, cube.count);

	model = glm::translate(glm::mat4{ 1.f }, glm::vec3{ -4.f, -2.f, 5.f });
	model = glm::rotate(model, 20.f, glm::vec3{ 1.f });
	shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, cube.count);

	model = glm::translate(glm::mat4{ 1.f }, glm::vec3{ -5.f, .5f, 0.f });
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3{ 1.f });
	model = glm::scale(model, glm::vec3{ 1.f, .2f, 1.f });
	shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, cube.count);
}


void OmnidirectionalShadows::loop() {
	Shader shader{ Resource::shader("shader") };
	Shader depth{ Resource::shader("depth") };

	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		depth.use();
		for (short i{}; i != 6; i++) 
			depth.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		depth.setFloat("far_plane", far_plane);
		depth.setVec3("lightPos", lightPos);
		drawScene(depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, window.width, window.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 proj{ glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f) };
	glm::mat4 view{ camera.GetViewMatrix() };

	shader.use();
	shader.setMat4("projview", proj * view);
	shader.setFloat("far_plane", far_plane);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera.position);

	glActiveTexture(GL_TEXTURE0);
	Resource::texture("container").bind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

	drawScene(shader);
}