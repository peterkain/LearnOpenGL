#include "Cubemap.h"
#include "Data.h"
#include "Resource.h"
#include "SimpleObjLoader.h"

void Cubemap::init() {

	std::vector<std::string> faces {
		"textures/cubemap1/right.jpg",
		"textures/cubemap1/left.jpg",
		"textures/cubemap1/top.jpg",
		"textures/cubemap1/bottom.jpg",
		"textures/cubemap1/front.jpg",
		"textures/cubemap1/back.jpg"
	};

	Image::flip_vertically(false);
	Resource::cubemap("skybox", std::move(CubeMap{ faces }));

	Image::flip_vertically(true);
	Resource::texture("alhambra", std::move(Texture{ "textures/alhambra.jpg" }));
	Resource::shader("cubemap", std::move(Shader{ "shaders/cube_map_vs.glsl", "shaders/cube_map_fs.glsl" }));
	Resource::shader("cube", std::move(Shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" }));
	Resource::mesh("cube", std::move(Mesh{ "models/cube.obj" }));
	Resource::mesh("edgysphere", std::move(Mesh{ "models/edgysphere.obj" }));

	glClearColor(.2f, .1f, .2f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Cubemap::loop() {
	Mesh mesh_cube{ Resource::mesh("cube") };
	Mesh mesh_edgysphere{ Resource::mesh("edgysphere") };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 model{ 1.f };
	glm::mat4 view{ camera.GetViewMatrix() };
	glm::mat4 projection{ glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f) };

	glBindVertexArray(mesh_cube.VAO);

	glDepthMask(GL_FALSE);
	Shader cubemap{ Resource::shader("cubemap") };
	cubemap.use();

	cubemap.setMat4("view", glm::mat4{ glm::mat3{view} });
	cubemap.setMat4("projection", projection);

	Resource::cubemap("skybox").bind();
	glDrawArrays(GL_TRIANGLES, 0, mesh_cube.count);
	glDepthMask(GL_TRUE);

	glBindVertexArray(mesh_edgysphere.VAO);

	Shader cube{ Resource::shader("cube") };
	cube.use();

	//Resource::texture("alhambra").bind();

	cube.setMat4("view", view);
	cube.setMat4("projection", projection);
	cube.setVec3("cameraPos", camera.position);

	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3{ 1.f, 1.f, 1.f });
	//model = glm::scale(model, glm::vec3(1.2f, 1.f, 1.5f));

	cube.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, mesh_edgysphere.count);
}