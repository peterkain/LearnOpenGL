#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "applications/Lighting.h"
#include "applications/AdvancedOpenGL.h"
#include "applications/Cubemap.h"
#include "applications/Instancing.h"
#include "applications/AsteroidField.h"
#include "applications/Shadows.h"
#include "applications/OmnidirectionalShadows.h"
#include "applications/NormalMapping.h"
#include "applications/HeightMapVisualizer.h"

constexpr int window_w{ 1280 };
constexpr int window_h{ 1080 };

int main() {
	/*Lighting lighting{ window_w, window_h, "Learn OpenGL - Lighting", true };
	lighting.run();*/

	/*AdvancedOpenGL advancedopengl{ window_w, window_h, "Learn OpenGL - Advanced OpenGL", true };
	advancedopengl.run();*/

	/*Cubemap cubemap{ window_w, window_h, "Learn OpenGL - Cubemap", true };
	cubemap.run();*/

	/*Instancing instancing{ window_w, window_h, "Learn OpenGL - Instancing", false };
	instancing.run();*/

	/*AsteroidField asteroidField{ window_w, window_h, "Learn OpenGL - Asteroid Field", false };
	asteroidField.run();*/

	/*Shadows shadows{ window_w, window_h, "Learn OpenGL - Shadows", false };
	shadows.run();*/

	/*OmnidirectionalShadows omni_shadows{ window_w, window_h, "Learn OpenGL - Point Shadows", true };
	omni_shadows.run();*/

	/*NormalMapping normal_mapping{ window_w, window_h, "Learn OpenGL - Normal Mapping", false };
	normal_mapping.run();*/

	HeightMapVisualizer hm_vis{ window_w, window_h, "Learn OpenGL - HeightMap Visualizer", false };
	hm_vis.run();
}