#include "NormalMapping.h"
#include "Resource.h"
#include "Data.h"


void NormalMapping::init() {
	/*Resource::texture("brickwall", std::move(Texture{ "textures/brickwall.jpg" }));
	Resource::texture("brickwall_normal", std::move(Texture{ "textures/brickwall_normal.jpg" }));*/
	Resource::shader("shader", std::move(Shader{ "shaders/normal_mapping_vs.glsl", "shaders/normal_mapping_fs.glsl" }));

	Resource::texture("bricks", std::move(Texture{ "textures/bricks.jpg" }));
	Resource::texture("bricks_normal", std::move(Texture{ "textures/bricks_normal.jpg" }));
	Resource::texture("bricks_displacement", std::move(Texture{ "textures/bricks_disp.jpg" }));

	// *****************************************************
	// Calculating Tangent & Bitangent Vectors for Tangent Space
	glm::vec3 pos1{ -1.f,  1.f, 0.f };
	glm::vec3 pos2{ -1.f, -1.f, 0.f };
	glm::vec3 pos3{  1.f, -1.f, 0.f };
	glm::vec3 pos4{  1.f,  1.f, 0.f };

	glm::vec2 uv1{ 0.f, 1.f };
	glm::vec2 uv2{ 0.f, 0.f };
	glm::vec2 uv3{ 1.f, 0.f };
	glm::vec2 uv4{ 1.f, 1.f };

	glm::vec3 nm{ 0.f, 0.f, 1.f };

	glm::vec3 tangent1, tangent2;
	glm::vec3 bitangent1, bitangent2;

	// Triangle 1
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y); // Determinant of deltaUV Matrix

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	/*bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);*/

	bitangent1 = glm::cross(nm, tangent1);

	// Triangle 2
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	/*bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);*/

	bitangent2 = glm::cross(nm, tangent2);

	float quadVertices[] = {
		// positions            // texcoords  // normal         // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, uv2.x, uv2.y, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, uv3.x, uv3.y, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, uv3.x, uv3.y, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, uv4.x, uv4.y, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);

	glClearColor(.1f, .1f, .1f, 1.f);

	camera.position = glm::vec3{ 0.f, 0.f, 3.f };
}


void NormalMapping::loop() {
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 proj{ glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, .1f, 100.f )};
	glm::mat4 view{ camera.GetViewMatrix() };
	//glm::vec3 lightPos{ sin(glfwGetTime()), cos(glfwGetTime()), 0. };
	//glm::vec3 lightPos{ sin(glfwGetTime()), cos(glfwGetTime()), 1.f };
	glm::vec3 lightPos{ 0.f, 0.f, 1.f };

	Shader shader{ Resource::shader("shader") };
	shader.use();
	shader.setMat4("projview", proj * view);
	//shader.setMat4("model", glm::rotate(glm::mat4{ 1.f }, (float)glfwGetTime(), glm::vec3{ 1.f }));
	shader.setMat4("model", glm::mat4{ 1.f });
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera.position);
	shader.setBool("normalMapping", glfwGetKey(window.glfw_window, GLFW_KEY_F1) == GLFW_PRESS);
	shader.setBool("parallaxMapping", glfwGetKey(window.glfw_window, GLFW_KEY_F2) == GLFW_PRESS);
	shader.setFloat("height_scale", .1f);

	shader.setInt("Texture", 0);
	shader.setInt("NormalMap", 1);
	shader.setInt("DepthMap", 2);

	glActiveTexture(GL_TEXTURE0);
	Resource::texture("bricks").bind();
	glActiveTexture(GL_TEXTURE1);
	Resource::texture("bricks_normal").bind();
	glActiveTexture(GL_TEXTURE2);
	Resource::texture("bricks_displacement").bind();
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}