/*!
@file    Curve.h
@author  Hyosang Jung, Jaewoo.choi
@date    04/14/2022

Note : This file contains the declaration TriangleTest class member function
and this class inherited by Test, So it has all function what Test class have.

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
	CurveTest();
	~CurveTest();
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
	

private:
	GLuint VAO;
	GLuint VBO;
	GLSLShader Prog;

	GLuint Derive_VAO;
	GLuint Derive_VBO;

	std::vector<glm::vec2> vertices;
	std::vector<Point> start_point;
	std::vector<Point> end_point;

	int num_vertices;
	bool is_hermite;
};