#include "HeightMapVisualizer.h"
#include "Resource.h"
#include "Data.h"

#include <random>
#include <functional>

struct Float5 {
	float x, y, z, u, v;
};

//void diamond(std::vector<Float5>& data, int m, int step, unsigned size, float roughness) {
//	float p1{ data[m + size * step + step].y };
//	float p2{ data[m + size * step - step].y };
//	float p3{ data[m - size * step + step].y };
//	float p4{ data[m - size * step - step].y };
//
//	float new_y = (p1 + p2 + p3 + p4) / 4.f;
//
//	data[m].y = new_y + roughness;
//}
//
//void square(std::vector<Float5>& data, int m, int step, unsigned size, float roughness) {
//	unsigned p1{ (m - step * size) };
//	unsigned p2{ (m + step * size) };
//	unsigned p3{ static_cast<unsigned>(m - step) };
//	unsigned p4{ static_cast<unsigned>(m + step) };
//
//	float new_y;
//
//	if (p1 >= data.size())
//		new_y = (data[p2].y + data[p3].y + data[p4].y) / 3.f;
//	else if (p2 >= data.size())
//		new_y = (data[p1].y + data[p3].y + data[p4].y) / 3.f;
//	else
//		new_y = (data[p1].y + data[p2].y + data[p3].y + data[p4].y) / 4.f;
//
//	data[m].y = new_y + roughness;
//}
//
//void diamondSquare(std::vector<Float5>& data, unsigned size, unsigned step, unsigned m, float roughness = 0.3f) {
//	std::random_device rd;
//	std::mt19937 randomEngine{ rd() };
//
//	std::uniform_real_distribution<float> dist{ -roughness, roughness };
//
//	diamond(data, m, step, size, dist(randomEngine));
//
//	square(data, m - step * size, step, size, dist(randomEngine));
//	square(data, m + step * size, step, size, dist(randomEngine));
//	square(data, m - step, step, size, dist(randomEngine));
//	square(data, m + step, step, size, dist(randomEngine));
//
//	roughness *= .5f;
//	step = static_cast<unsigned>(std::ceil(step / 2));
//
//	if (step > 0) {
//		diamondSquare(data, size, step, m - step * size - step, roughness);
//		diamondSquare(data, size, step, m - step * size + step, roughness);
//		diamondSquare(data, size, step, m + step * size - step, roughness);
//		diamondSquare(data, size, step, m + step * size + step, roughness);
//	}
//}


std::vector<std::vector<float>> diamond_square(unsigned n, float roughness, float falloff, int seed = -1) {
	std::size_t size{ static_cast<std::size_t>(std::pow(2, n)) };

	std::vector<std::vector<float>> data(size + 1);
	for (auto& v : data) {
		v.resize(size + 1);
	}

	std::mt19937 re{ (seed == -1) ? static_cast<unsigned>(std::time(nullptr)) : seed };
	std::uniform_real_distribution<float> dist(0.f, 1.f);

	data[0][0] = dist(re);
	data[0][size] = dist(re);
	data[size][0] = dist(re);
	data[size][size] = dist(re);

	int step{ static_cast<int>(size / 2) };
	while(step > 0) {
		std::uniform_real_distribution<float> dist(-roughness, roughness);
		// diamond
		for (int x{ step }; x < size; x += 2 * step) {
			for (int y{ step }; y < size; y += 2 * step) {
				float p1{ data[x - step][y - step] };
				float p2{ data[x - step][y + step] };
				float p3{ data[x + step][y - step] };
				float p4{ data[x + step][y + step] };

				float avg{ ((p1 + p2 + p3 + p4) / 4.f) + dist(re) };
				data[x][y] = glm::clamp(avg, 0.f, 1.f);
			}
		}

		// square
		for (int x{}; x <= size; x += step) {
			for (int y = (x / step) % 2 == 0 ? step : 0; y <= size; y += step * 2) {
				int numAvgs{ 4 };

				float p1, p2, p3, p4;

				if (x - step < 0) {
					numAvgs--;
					p1 = 0.f;
				}
				else {
					p1 = data[x - step][y];
				}

				if (x + step > size) {
					numAvgs--;
					p2 = 0.f;
				}
				else {
					p2 = data[x + step][y];
				}

				if (y - step < 0) {
					numAvgs--;
					p3 = 0.f;
				}
				else {
					p3 = data[x][y - step];
				}

				if (y + step > size) {
					numAvgs--;
					p4 = 0.f;
				}
				else {
					p4 = data[x][y + step];
				}

				float avg{ ((p1 + p2 + p3 + p4) / numAvgs) + dist(re) };
				data[x][y] = glm::clamp(avg, 0.f, 1.f);
			}
		}

		roughness *= falloff;
		step /= 2;
	}

	return data;
}


void HeightMapVisualizer::init() {
	std::vector<Float5> vertexData;
	std::vector<glm::uvec3> indexData;

	constexpr unsigned size{ 129 };
	constexpr float quadSize{ (1.f / size) * 5 };

	auto heightmap{ diamond_square(static_cast<unsigned>(std::ceil(std::log2(size))), 2.f, .5f) };

	glm::vec2 uvBias{ static_cast<float>(size / (size - 1)) };
	for (int x{}; x < size; x++) {
		for (int y{}; y < size; y++) {
			vertexData.push_back({ x * quadSize, heightmap[x][y], y * quadSize, uvBias.x * x, uvBias.y * y });
		}
	}

	for (int c{}; c != size * size - size; c += size) {
		for (int y{}; y < size - 1; y++) {
			indexData.push_back({ y + c, y + c + 1, y + c + size });
			indexData.push_back({ y + c + size, y + c + size + 1, y + c + 1 });
		}
	}

	startingHeight = (vertexData[0].y + vertexData[size - 1].y + vertexData[size * size - size].y + vertexData[size * size - 1].y) / 4.f;

	count = indexData.size() * 3;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Float5), vertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(glm::uvec3), indexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	Resource::shader("shader", std::move(Shader{ "shaders/heightmap_vs.glsl", "shaders/heightmap_fs.glsl" }));
	Resource::texture("heightmap", std::move(Texture{ "textures/heightmap4.jpg" }));
	Resource::texture("alhambra", std::move(Texture{ "textures/alhambra.jpg" }));

	glClearColor(.8f, .5f, .1f, 1.f);
	glEnable(GL_DEPTH_TEST);

	camera.position = glm::vec3(size * quadSize / 2, 1.f, size * quadSize / 2);
}


void HeightMapVisualizer::loop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view{ camera.GetViewMatrix() };
	glm::mat4 projection{ glm::perspective(glm::radians(camera.zoom), (float)(window.width / window.height), .1f, 100.f) };
	glm::mat4 model{ 1.f };

	glBindVertexArray(VAO);

	Shader shader{ Resource::shader("shader") };
	shader.use();

	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);

	shader.setInt("Heightmap", 0);
	shader.setInt("Texture", 1);

	shader.setFloat("uTime", glfwGetTime());
	shader.setFloat("startingHeight", startingHeight);

	glActiveTexture(GL_TEXTURE0);
	Resource::texture("heightmap").bind();

	glActiveTexture(GL_TEXTURE1);
	Resource::texture("alhambra").bind();

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}