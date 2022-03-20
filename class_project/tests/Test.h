#pragma once
#include"../math.h"
class Test
{
public:
	Test() {};
	virtual ~Test() {};

	virtual void Update(float deltaTime) {}
	virtual void Draw() {}
	virtual void OnImGuiRender() {};
	virtual void camera_init()
	{
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	virtual glm::mat4 Get_camera()
	{
		return view;
	}
private:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.5f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 view;

};