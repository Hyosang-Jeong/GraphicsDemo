/*!
@file    Shadow.h
@author  Hyosang Jung, Jaewoo.choi
@date    05/19/2022

Note : This file contains the declaration Toon_Fog class member function
and this class inherited by Test, So it has all function what Test class have.

*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include"../Mesh.h"
#include"../Camera.h"
class Shadow_test : public Test
{
public:
	Shadow_test();
	~Shadow_test();
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void OnImGuiRender() override;
	void DepthMap_Setup();
	void Depth_Draw();
	void Scene_Draw();
	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	const Vec4 useNormal = Vec4(1, 1, 1, 1.0);
	
private:
	Camera camera;
	Mesh mesh;
	Mesh plane;
	Mesh sphere;
	glm::mat4  projection;
	glm::vec3 light;
	

	unsigned int depthMapFBO;
	unsigned int depthMap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float near_plane = 0.1f, far_plane = 100.f;
	float FOV;
};