//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
/*!
@file    Curve.h
@author  Hyosang Jung, Jaewoo.choi
@date    05/02/2022

Note : This file contains the declaration CurveTest class member function
and this class inherited by Test, So it has all function what Test class have.
Also, we have Point struct, it has vec2 position, and vec2 tangent

*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include <GL/glew.h> 
#include"../glslshader.h"

struct Point
{
	glm::vec2 pos;
	glm::vec2 tangent;
};

class CurveTest : public Test
{
public:
	CurveTest()  = default;
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void UnLoad() override;
	void OnImGuiRender() override;


	void send_data();
	void send_derive_data();
	void setup_shader(std::string shader);
	void hermite_curve();
	void catmullRom_curve();
	void draw_curve();
	void draw_derives();
	void update_vertice();
	bool in_mouse(double mouse_pos_x, double mouse_pos_y, glm::vec2 pos);
	void add_vertex();
	void clear_vertices();
	void calculate(std::vector<glm::vec2> vertice);

private:
	GLuint VAO{ 0 };
	GLuint VBO{ 0 };
	GLSLShader Prog;

	GLuint Derive_VAO{ 0 };
	GLuint Derive_VBO{ 0 };

	std::vector<glm::vec2> vertices;
	std::vector<Point> start_point;
	std::vector<Point> end_point;
	int num_vertices{ 0 };
	bool is_hermite{ true };
	float t_min{ 0 };
	float t_max{ 0 };
};