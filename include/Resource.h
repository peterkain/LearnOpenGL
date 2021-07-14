#pragma once

#include <unordered_map>
#include <string_view>

#include "Shader.h"
#include "Texture.h"
#include "SimpleObjLoader.h"

class Resource {
	inline static std::unordered_map<std::string, Shader> shaders;
	inline static std::unordered_map<std::string, Texture> textures;
	inline static std::unordered_map<std::string, CubeMap> cubemaps;
	inline static std::unordered_map<std::string, Mesh> meshes;

	Resource();
public:
	static void shader(const std::string& name, Shader&& shader) {
		shaders.emplace(name, shader);
	}

	static void texture(const std::string& name, Texture&& texture) {
		textures.emplace(name, texture);
	}

	static void cubemap(const std::string& name, CubeMap&& cubemap) {
		cubemaps.emplace(name, cubemap);
	}

	static void mesh(const std::string& name, Mesh&& mesh) {
		meshes.emplace(name, mesh);
	}

	static Shader& shader(const std::string& name) {
		return shaders.find(name)->second;
	}

	static Texture& texture(const std::string& name) {
		return textures.find(name)->second;
	}

	static CubeMap& cubemap(const std::string& name) {
		return cubemaps.find(name)->second;
	}

	static Mesh& mesh(const std::string& name) {
		return meshes.find(name)->second;
	}
};