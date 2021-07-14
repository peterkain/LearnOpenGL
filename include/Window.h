#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
	int width, height;
	GLFWwindow* glfw_window;
public:
	Window(int width, int height, const std::string& title, bool fullscreen) :
		width{ width }, height{ height } {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfw_window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

		if (!glfw_window) {
			std::cerr << "Failed to create Window" << std::endl;
			glfwTerminate();
			std::exit(-1);
		}

		glfwMakeContextCurrent(glfw_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to load GLAD" << std::endl;
			glfwTerminate();
			std::exit(-2);
		}

		set_input_mode(GLFW_CURSOR_DISABLED);
		glViewport(0, 0, width, height);

		glfwSwapInterval(1);
	}

	void set_input_mode(int flag) {
		glfwSetInputMode(glfw_window, GLFW_CURSOR, flag);
	}

	void key_cb(GLFWkeyfun cb) {
		glfwSetKeyCallback(glfw_window, cb);
	}

	void cursor_cb(GLFWcursorposfun cb) {
		glfwSetCursorPosCallback(glfw_window, cb);
	}

	void scroll_cb(GLFWscrollfun cb) {
		glfwSetScrollCallback(glfw_window, cb);
	}

	void resize_cb(GLFWframebuffersizefun cb) {
		glfwSetFramebufferSizeCallback(glfw_window, cb);
	}
};