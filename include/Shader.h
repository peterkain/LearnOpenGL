#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string_view>
#include <glm/glm.hpp>


class Shader {
public:
	GLuint ID;

	Shader(const char* vsPath, const char* fsPath) {
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vsPath);
			fShaderFile.open(fsPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "Failed to read Shader File" << std::endl;
		}

		const GLchar* vShaderCode{ vertexCode.c_str() };
		const GLchar* fShaderCode{ fragmentCode.c_str() };

		GLuint vertex, fragment;
		GLint success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cerr << "Vertex Shader compilation failed\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			std::cerr << "Fragment Shader compilation failed\n" << infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cerr << "Shader Program linking failed\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader(const char* vsPath, const char* gsPath, const char* fsPath) {
		std::string vertexCode, geometryCode, fragmentCode;
		std::ifstream vShaderFile, gShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vsPath);
			gShaderFile.open(gsPath);
			fShaderFile.open(fsPath);
			std::stringstream vShaderStream, gShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			gShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			geometryCode = gShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "Failed to read Shader File" << std::endl;
		}

		const GLchar* vShaderCode{ vertexCode.c_str() };
		const GLchar* gShaderCode{ geometryCode.c_str() };
		const GLchar* fShaderCode{ fragmentCode.c_str() };

		GLuint vertex, geometry, fragment;
		GLint success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cerr << "Vertex Shader compilation failed\n" << infoLog << std::endl;
		}

		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);

		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
			std::cerr << "Geometry Shader compilation failed\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			std::cerr << "Fragment Shader compilation failed\n" << infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, geometry);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cerr << "Shader Program linking failed\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(geometry);
		glDeleteShader(fragment);
	}

	void use() {
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) {
		glUniform1i(Location(name), (GLint)value);
	}

	void setInt(const std::string& name, int value) {
		glUniform1i(Location(name), value);
	}

	void setFloat(const std::string& name, float value) {
		glUniform1f(Location(name), value);
	}

	void setFloat2(const std::string& name, float x, float y) {
		glUniform2f(Location(name), x, y);
	}

	void setFloat3(const std::string& name, float x, float y, float z) {
		glUniform3f(Location(name), x, y, z);
	}

	void setFloat4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(Location(name), x, y, z, w);
	}

	void setVec2(const std::string& name, const glm::vec2& value) {
		glUniform2fv(Location(name), 1, &value[0]);
	}

	void setVec3(const std::string& name, const glm::vec3& value) {
		glUniform3fv(Location(name), 1, &value[0]);
	}

	void setVec4(const std::string& name, const glm::vec4& value) {
		glUniform4fv(Location(name), 1, &value[0]);
	}

	void setMat2(const std::string& name, const glm::mat2& value) {
		glUniformMatrix2fv(Location(name), 1, GL_FALSE, &value[0][0]);
	}

	void setMat3(const std::string& name, const glm::mat3& value) {
		glUniformMatrix3fv(Location(name), 1, GL_FALSE, &value[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& value) {
		glUniformMatrix4fv(Location(name), 1, GL_FALSE, &value[0][0]);
	}

private:
	GLint Location(const std::string& name) {
		auto iter{ locations.find(name) };
		if (iter == locations.end()) {
			GLint location{ glGetUniformLocation(ID, name.c_str()) };
			locations.emplace(name, location);
			return location;
		}

		return iter->second;
	}

	std::unordered_map<std::string, GLint> locations;
};