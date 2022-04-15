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

class CurveTest : public Test
{
public:
	CurveTest();
	~CurveTest();
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void UnLoad() override;
	void send_data();
	void setup_shader(std::string shader);
	void compute_vertices(int num_vertices, glm::vec2 start, glm::vec2 end, glm::vec2 derive_start, glm::vec2 derive_end);
	void draw_curve();
	void draw_derives();
	void update_vertice();
	std::vector<glm::vec2> vertices;

private:
	GLuint VAO;
	GLuint VBO;
	GLSLShader Prog;

	GLuint Derive_VAO;
	GLuint Derive_VBO;

	glm::vec2 start;
	glm::vec2 end;
	glm::vec2 derive_start;
	glm::vec2 derive_end;



	bool is_pressing[4] = { false };
};