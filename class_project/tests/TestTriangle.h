//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
/*!
@file    TestTriangle.h
@author  Hyosang Jung, Jaewoo.choi
@date    04/04/2022

Note : This file contains the declaration TriangleTest class member function
and this class inherited by Test, So it has all function what Test class have.

*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include"../Mesh.h"

class TriangleTest: public Test
{
public:
	TriangleTest();
	 ~TriangleTest();
	 void init() override;
	 void Update(float deltaTime)  override;
	 void Draw() override;
	 void OnImGuiRender() override;
	 void UnLoad() override;
	std::vector<Mesh> meshes;

	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	 Vec4 useNormal = Vec4(1.f,0.2f, 0.4f, -1.0f);

private:
	glm::mat4  view;
	glm::mat4  projection;
	glm::vec3 eye;
	glm::vec3 light;

    bool meshSwitch[NUM_MESHES] = { true,true,true,true,true,true };
    bool is_switch_pressed(const char* buttonName, bool &buttonType);
    void onOffSwitch();
};