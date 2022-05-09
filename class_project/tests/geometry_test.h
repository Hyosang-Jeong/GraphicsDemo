/*!
@file    geometry_test.h
@author  Hyosang Jung, Jaewoo.choi
@date    05/6/2022

Note : This file contains the declaration TriangleTest class member function
and this class inherited by Test, So it has all function what Test class have.

*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include <GL/glew.h> 
#include"../Mesh.h"
#include"../glslshader.h"


class Geometry_test : public Test
{
public:
	Geometry_test();
	~Geometry_test();
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void UnLoad() override;
	void OnImGuiRender() override;

	std::vector<Mesh> meshes;

	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	Vec4 useNormal = Vec4(1.f, 0.2f, 0.4f, -1.0f);
	GLSLShader explode;
private:
	glm::mat4  view;
	glm::mat4  projection;
	glm::vec3 eye;
	glm::vec3 light;
	float timer{ 0 };
	bool animated{ true };
	int stack;
	int slice;
};