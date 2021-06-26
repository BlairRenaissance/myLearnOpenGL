#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection; // 注意cameraDirection与cameraFront方向相反，cameraDirection指向+Z方向
	glm::vec3 cameraFront;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;

	float Pitch;
	float Yaw;
	float speedZ = 0;


	Camera(glm::vec3 Position, glm::vec3 Target, glm::vec3 WorldUp);
	Camera(glm::vec3 Position, float pitch, float yaw, glm::vec3 WorldUp);
	glm::mat4 getViewMatrix();
	void processMouseMovement(float deltaX, float deltaY);
	void updateCameraPos();

private:
	void updateCameraVectors();
};