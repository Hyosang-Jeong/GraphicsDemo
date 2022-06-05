//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
#include "Camera.h"
#include"glhelper.h"
#include "glm/gtc/matrix_transform.hpp"
Camera::Camera( glm::vec3 eye):eye(eye),pitch(0),yaw(-90.0f),angle(0)
{
  cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraDirection = glm::vec3(0.0f, 0.0f, -1.f);

  cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
  cameraUp = glm::cross(cameraDirection, cameraRight);
}
Camera::Camera(glm::vec3 eye, glm::vec3 direction) :eye(eye), cameraDirection(direction), pitch(0), yaw(-90.0f), angle(0)
{
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
	cameraUp = glm::cross(direction, cameraRight);
}
void Camera::Update(float dt)
{
	float speed = 10.f;

	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_UP))
	{
		eye += speed * dt * cameraDirection;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_DOWN) )
	{
		eye -= speed * dt * cameraDirection;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_W))
	{
		eye += speed * dt * cameraUp;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_S))
	{
		eye -= speed * dt * cameraUp;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_A))
	{
		eye -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * speed * dt;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_D))
	{
		eye += glm::normalize(glm::cross(cameraDirection, cameraUp)) * speed * dt;
	}

	view = glm::lookAt(eye, eye + mouse_update(dt), cameraUp);
}
glm::vec3 Camera::mouse_update(float)
{
	double mouse_pos_x=0;
	double mouse_pos_y=0;
	static glm::vec2 start_pos{ 0 };
	static bool mouse_start = false;
	glfwGetCursorPos(GLHelper::ptr_window, &mouse_pos_x, &mouse_pos_y);

	glm::mat4 rotate = {
1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1
	};
	if (GLHelper::mouse_pressed == true)
	{
		if (mouse_start==false)
		{
			start_pos.x = static_cast<float>(mouse_pos_x);
			start_pos.y = static_cast<float>(mouse_pos_y);
			mouse_start = true;
		}
		float xoffset = static_cast<float>(mouse_pos_x - start_pos.x);
		float yoffset = static_cast<float>(start_pos.y - mouse_pos_y);
		start_pos.x = static_cast<float>(mouse_pos_x);
		start_pos.y = static_cast<float>(mouse_pos_y);

		yaw += xoffset*0.07f;
		pitch += yoffset*0.07f;
	}
	else
	{
		mouse_start = false;
	}

	glm::vec3 front = cameraDirection;
	front.x += cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y += sin(glm::radians(pitch));
	front.z += sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(front, cameraUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	cameraUp = glm::normalize(glm::cross(cameraRight, front));
	return (front);
}
glm::mat4& Camera::GetViewMatrix()
{
    return view;
}

glm::vec3 Camera::GetEye()
{
    return eye;
}
