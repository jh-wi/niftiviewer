#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovement {
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 5.0f;
const float DEFAULT_SENSITIVITY = 0.1f;

class Camera {
private:
	void UpdateVectors() {
		glm::vec3 temp;
		temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		temp.y = sin(glm::radians(pitch));
		temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(temp);
		right = glm::normalize(glm::cross(front, globalUp));
		up = glm::normalize(glm::cross(right, front));
	}
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 globalUp;
	
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	
	Camera(glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _globalUp = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = DEFAULT_YAW, float _pitch = DEFAULT_PITCH) {
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		position = _position;
		globalUp = _globalUp;
		yaw = _yaw;
		pitch = _pitch;
		speed = DEFAULT_SPEED;
		sensitivity = DEFAULT_SENSITIVITY;
	}
	
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw = DEFAULT_YAW, float _pitch = DEFAULT_PITCH) {
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		position = glm::vec3(posX, posY, posZ);
		globalUp = glm::vec3(upX, upY, upZ);
		yaw = _yaw;
		pitch = _pitch;
		speed = DEFAULT_SPEED;
		sensitivity = DEFAULT_SENSITIVITY;
	}
	
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}
	
	void ProcessKeyboard(CameraMovement dir, float deltaTime) {
		float velocity = speed * deltaTime;
		if (dir == FORWARD) position += front * velocity;
		if (dir == BACK) position -= front * velocity;
		if (dir == LEFT) position -= right * velocity;
		if (dir == RIGHT) position += right * velocity;
		if (dir == UP) position += globalUp * velocity;
		if (dir == DOWN) position -= globalUp * velocity;
	}
	
	void ProcessMouse(float offsetX, float offsetY) {
		offsetX *= sensitivity;
		offsetY *= sensitivity;
		
		yaw += offsetX;
		pitch -= offsetY;
		
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		
		UpdateVectors();
	}
};