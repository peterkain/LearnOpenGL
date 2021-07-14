#pragma once

#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>

struct Float3 {
	GLfloat x, y, z;
};

struct Float2 {
	GLfloat x, y;
};

struct Mesh {
private:
	std::vector<GLfloat> vertices;
	size_t stride;
	GLuint VBO;
public:
	GLuint VAO;
	GLuint count;

	Mesh(const char* filename) {
		std::ifstream file{ filename };
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << '\n';
		}

		std::string line;
		Float3 position;
		Float2 texture;
		Float3 normal;
		unsigned positionIndex[3];
		unsigned textureIndex[3];
		unsigned normalIndex[3];
		std::vector<Float3> tempPositions;
		std::vector<Float2> tempTextures;
		std::vector<Float3> tempNormals;
		std::vector<unsigned> tempPositionIndices;
		std::vector<unsigned> tempTextureIndices;
		std::vector<unsigned> tempNormalIndices;
		bool foundTexture{};
		while (std::getline(file, line)) {
			const char type[3] {};
			sscanf_s(line.c_str(), "%s", type, 3);

			if (std::strcmp(type, "v") == 0) {
				sscanf_s(line.c_str(), "%*s %f %f %f", &position.x, &position.y, &position.z);
				tempPositions.push_back(position);
			}
			else if (std::strcmp(type, "vt") == 0) {
				foundTexture = true;
				sscanf_s(line.c_str(), "%*s %f %f", &texture.x, &texture.y);
				tempTextures.push_back(texture);
			}
			else if (std::strcmp(type, "vn") == 0) {
				sscanf_s(line.c_str(), "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
			}
			else if (std::strcmp(type, "f") == 0) {
				if (foundTexture) {
					sscanf_s(line.c_str(), "%*s %u/%u/%u %u/%u/%u %u/%u/%u",
						&positionIndex[0], &textureIndex[0], &normalIndex[0],
						&positionIndex[1], &textureIndex[1], &normalIndex[1],
						&positionIndex[2], &textureIndex[2], &normalIndex[2]
					);

					for (unsigned i{}; i != 3; i++) {
						tempPositionIndices.push_back(positionIndex[i] - 1);
						tempTextureIndices.push_back(textureIndex[i] - 1);
						tempNormalIndices.push_back(normalIndex[i] - 1);
					}
				}
				else {
					sscanf_s(line.c_str(), "%*s %u//%u %u//%u %u//%u",
						&positionIndex[0], &normalIndex[0],
						&positionIndex[1], &normalIndex[1],
						&positionIndex[2], &normalIndex[2]
					);

					for (unsigned i{}; i != 3; i++) {
						tempPositionIndices.push_back(positionIndex[i] - 1);
						tempNormalIndices.push_back(normalIndex[i] - 1);
					}
				}
			}
		}

		count = tempPositionIndices.size();

		if (foundTexture) {
			for (std::size_t s{}; s != tempPositionIndices.size(); s++) {
				position = tempPositions[tempPositionIndices[s]];
				texture = tempTextures[tempTextureIndices[s]];
				normal = tempNormals[tempNormalIndices[s]];

				vertices.push_back(position.x);
				vertices.push_back(position.y);
				vertices.push_back(position.z);
				vertices.push_back(texture.x);
				vertices.push_back(texture.y);
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}
			stride = 8;
		}
		else {
			for (std::size_t s{}; s != tempPositionIndices.size(); s++) {
				position = tempPositions[tempPositionIndices[s]];
				normal = tempNormals[tempNormalIndices[s]];

				vertices.push_back(position.x);
				vertices.push_back(position.y);
				vertices.push_back(position.z);
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}
			stride = 6;
		}

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

		if (stride == 8) {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		}
		else {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		}

		glBindVertexArray(0);
		vertices.clear();
	}
};

