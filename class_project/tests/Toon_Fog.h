/*!
@file    Toon_Fog.h
@author  Hyosang Jung, Jaewoo.choi
@date    04/04/2022

Note : This file contains the declaration Toon_Fog class member function
and this class inherited by Test, So it has all function what Test class have.

*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include"../Mesh.h"

class Toon_Fog : public Test
{
public:
	Toon_Fog();
	~Toon_Fog();
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void OnImGuiRender() override;

	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	const Vec4 useNormal = Vec4(0, 0, 1,1.0);

private:
	Mesh sphere;
	Mesh sun;

	glm::mat4  view;
	glm::mat4  projection;
	glm::vec3 eye;
	glm::vec3 light;
	float angle = 0;
	float FogMax = 5.f;
	float FogMin = 2.f;
	bool meshSwitch[NUM_MESHES] = { true,true,true,true,true,true };
	bool is_switch_pressed(const char* buttonName, bool& buttonType);
	void onOffSwitch();
};