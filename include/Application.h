#pragma once

#include <memory>

#include "Camera.h"
#include "Window.h"

struct Application {
	Window window;
	Camera camera;

	float deltaTime, lastFrame;

	Application(int width, int height, const std::string& title, bool fullscreen) :
		window{ width, height, title, fullscreen }, camera{}, deltaTime{}, lastFrame{} {

		glfwSetWindowUserPointer(window.glfw_window, &window);
		window.resize_cb([](GLFWwindow* w, int width, int height) {
			glViewport(0, 0, width, height);
			
			Window* window{ static_cast<Window*>(glfwGetWindowUserPointer(w)) };
			window->width = width;
			window->height = height;
		});

		glfwSetWindowUserPointer(window.glfw_window, &camera);
		window.scroll_cb([](GLFWwindow* w, double xoffset, double yoffset) {
			static_cast<Camera*>(glfwGetWindowUserPointer(w))->ProcessScroll(yoffset);
		});

		window.cursor_cb([](GLFWwindow* w, double xpos, double ypos) {
			static float lastX{ (float)xpos };
			static float lastY{ (float)ypos };

			float offsetX{ (float)xpos - lastX };
			float offsetY{ lastY - (float)ypos };
			lastX = xpos;
			lastY = ypos;

			static_cast<Camera*>(glfwGetWindowUserPointer(w))->ProcessMouse(offsetX, offsetY);
		});

		window.key_cb([](GLFWwindow* w, int key, int scancode, int action, int mods) {
			static bool wireframe{ true };
			if (key == GLFW_KEY_M && action == GLFW_PRESS) {
				if (wireframe = !wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		});
	}

	virtual ~Application() {
		glfwTerminate();
	}

	void processInput() {
		if (glfwGetKey(window.glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window.glfw_window, true);
		}

		camera.DefaultProcessInput(window.glfw_window, deltaTime);
	}

	void run() {
		init();
		while (!glfwWindowShouldClose(window.glfw_window)) {
			float currentFrame{ static_cast<float>(glfwGetTime()) };
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			processInput();

			loop();

			glfwPollEvents();
			glfwSwapBuffers(window.glfw_window);
		}
	}
private:
	virtual void init() = 0;
	virtual void loop() = 0;
};