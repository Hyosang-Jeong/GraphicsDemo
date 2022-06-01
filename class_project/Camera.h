#pragma once
#include <GL/glew.h> 
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
class Camera
{
public:
	Camera() = default;
	Camera(glm::vec3 eye);
	Camera(glm::vec3 eye, glm::vec3 direction);
	void Update(float dt);
	glm::vec3 mouse_update(float dt);
	glm::mat4& GetViewMatrix();
	glm::vec3 GetEye();

	glm::mat4  view;
	glm::vec3 eye; //camera position
	glm::vec3 cameraTarget; //view target
	glm::vec3 cameraDirection; //view direction
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec2 direction{ 0 };
	float pitch;
	float yaw;
	glm::vec3 angle;
};