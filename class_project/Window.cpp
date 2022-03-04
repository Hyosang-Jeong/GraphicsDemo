#include "Window.h"
#include<iostream>
void Window::init( std::string window_name)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	ptr_window = glfwCreateWindow(800, 600, window_name.c_str() , NULL, NULL);
	if (ptr_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(0);
	}
	glfwMakeContextCurrent(ptr_window);
	glewInit();
}
