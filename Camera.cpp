#include "Camera.h"

Camera::Camera(glm::vec3 Position, glm::vec3 Target, glm::vec3 WorldUp) {
	cameraPos = Position;
	cameraTarget = Target;
	cameraDirection = glm::normalize(Position - Target);
	cameraFront = -cameraDirection;
	cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
	cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
	worldUp = WorldUp;
}


Camera::Camera(glm::vec3 Position, float pitch, float yaw, glm::vec3 WorldUp){
	cameraPos = Position;
	worldUp = WorldUp;
	Pitch = pitch;
	Yaw = yaw;
	glm::vec3 direction;
	direction.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	direction.y = sin(glm::radians(Pitch));
	direction.z = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	cameraFront = -glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}


glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
}


void Camera::processMouseMovement(float deltaX, float deltaY){
	float sensitivity = 0.05;
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	Pitch += deltaY;
	Yaw += deltaX;

	/*if (Pitch > 89.0f) {
		Pitch = 89.0f;
	}
	if (Pitch < -89.0f) {
		Pitch = -89.0f;
	}*/

	if (Pitch > 45.0f) {
		Pitch = 45.0f;
	}
	if (Pitch < -45.0f) {
		Pitch = -45.0f;
	}

	updateCameraVectors();
}


void Camera::updateCameraPos(){
	cameraPos += glm::vec3(0, 0, speedZ);
}


void Camera::updateCameraVectors(){
	glm::vec3 direction;
	direction.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	direction.y = sin(glm::radians(Pitch));
	direction.z = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	cameraFront = -glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
