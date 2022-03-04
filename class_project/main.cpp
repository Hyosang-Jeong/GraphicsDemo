#include"Window.h"
#include"Triangle.h"
#include<iostream>
int main()
{
	Window window;
	window.init("Class Project");

	Triangle triangle;
	triangle.init();

	while (!glfwWindowShouldClose(window.Getwindow()))
	{
		triangle.draw();
		glfwSwapBuffers(window.Getwindow());
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}

