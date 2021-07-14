#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

enum class CameraMovement {
	forward, backward, left, right, up, down
};

constexpr float YAW{ -90.f };
constexpr float PITCH{ 0.f };
constexpr float SPEED{ 3.f };
constexpr float SENSITIVITY{ .1f };
constexpr float ZOOM{ 45.f };

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float speed;
	float sensitivity;
	float zoom;
public:
	Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW, float pitch = PITCH) :
		front{ glm::vec3(0.f, 0.f, -1.f) }, speed{ SPEED }, sensitivity{ SENSITIVITY }, zoom{ ZOOM }, position{ pos }, worldUp{ up },
		yaw{ yaw }, pitch{ pitch } {
		Update();
	}

	glm::mat4 GetViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime) {
		float v{ speed * deltaTime };
		switch (direction) {
		case CameraMovement::forward:
			position += front * v; break;
		case CameraMovement::backward:
			position -= front * v; break;
		case CameraMovement::left:
			position -= right * v; break;
		case CameraMovement::right:
			position += right * v; break;
		case CameraMovement::up:
			position.y += v; break;
		case CameraMovement::down:
			position.y -= v; break;
		}
	}

	void DefaultProcessInput(GLFWwindow* window, float deltaTime) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			ProcessKeyboard(CameraMovement::forward, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			ProcessKeyboard(CameraMovement::backward, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			ProcessKeyboard(CameraMovement::left, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			ProcessKeyboard(CameraMovement::right, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			ProcessKeyboard(CameraMovement::up, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			ProcessKeyboard(CameraMovement::down, deltaTime);
		}
	}

	void ProcessMouse(float offsetX, float offsetY, bool constrainPitch = true) {
		offsetX *= sensitivity;
		offsetY *= sensitivity;

		yaw += offsetX;
		pitch += offsetY;

		if (constrainPitch) {
			pitch = glm::clamp(pitch, -89.f, 89.f);
		}

		Update();
	}

	void ProcessScroll(float offsetY) {
		zoom -= offsetY;
		zoom = glm::clamp(zoom, 1.f, 70.f);
	}

private:
	void Update() {
		glm::vec3 dir;
		dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		dir.y = sin(glm::radians(pitch));
		dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(dir);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};