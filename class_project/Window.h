#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<string>
class Window
{
public:
	void init(std::string window_name );
	GLFWwindow* Getwindow()
	{
		return ptr_window;
	}
	GLFWwindow* ptr_window;
};