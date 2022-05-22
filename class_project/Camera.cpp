#include "Camera.h"
#include"glhelper.h"
#include"math.h"
Camera::Camera( glm::vec3 eye):eye(eye),pitch(0),yaw(-90.0f),angle(0)
{
  cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraDirection = glm::vec3(0.0f, 0.0f, -1.f);

  cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
  cameraUp = glm::cross(cameraDirection, cameraRight);
}
void Camera::Update(float dt)
{
	float speed = 10.f;

	//glm::mat4 rotate = mouse_update(dt);
	//cameraDirection = rotate * glm::vec4(cameraDirection,1);
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_UP) == true)
	{
		eye += speed * dt * cameraDirection;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_DOWN) == true)
	{
		eye -= speed * dt * cameraDirection;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_W) == true)
	{
		eye += speed * dt * cameraUp;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_S) == true)
	{
		eye -= speed * dt * cameraUp;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_A) == true)
	{
		eye -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * speed * dt;
	}
	if (glfwGetKey(GLHelper::ptr_window, GLFW_KEY_D) == true)
	{
		eye += glm::normalize(glm::cross(cameraDirection, cameraUp)) * speed * dt;
	}

	view = glm::lookAt(eye, eye + mouse_update(dt), cameraUp);
	//cameraDirection = glm::inverse(rotate) * glm::vec4(cameraDirection, 1);
}
glm::vec3 Camera::mouse_update(float dt)
{
	double mouse_pos_x=0;
	double mouse_pos_y=0;
	static glm::vec2 start_pos{ 0 };
	static bool mouse_start = false;
	glfwGetCursorPos(GLHelper::ptr_window, &mouse_pos_x, &mouse_pos_y);
	//mouse_pos_x = ((mouse_pos_x / static_cast<double>(GLHelper::width)) * 2) - 1;
	//mouse_pos_y = ((mouse_pos_y / static_cast<double>(GLHelper::height)) * -2) + 1;
	glm::mat4 rotate = {
1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1
	};
	if (GLHelper::mouse_pressed == true)
	{
		//if (mouse_start == false)
		//{
		//	start_pos.x = mouse_pos_x;
		//	start_pos.y = mouse_pos_y;
		//	mouse_start = true;
		//}
		//direction = start_pos - glm::vec2(static_cast<float>(mouse_pos_x), static_cast<float>(mouse_pos_y));
		//angle.x += -direction.y;
		//angle.y += direction.x;
		//start_pos.x = mouse_pos_x;
		//start_pos.y = mouse_pos_y;
		if (mouse_start==false)
		{
			start_pos.x = mouse_pos_x;
			start_pos.y = mouse_pos_y;
			mouse_start = true;
		}
		float xoffset = mouse_pos_x - start_pos.x;
		float yoffset = start_pos.y - mouse_pos_y;
		start_pos.x = mouse_pos_x;
		start_pos.y = mouse_pos_y;

		yaw += xoffset*0.01f;
		pitch += yoffset*0.01f;

	}
	else
	{
		mouse_start = false;
	}

	//rotate = glm::rotate(rotate, angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//rotate = glm::rotate(rotate, angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//rotate = glm::rotate(rotate, angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//return rotate;
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
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
