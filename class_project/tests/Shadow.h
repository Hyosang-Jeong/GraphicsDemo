/*!
@file    Shadow.h
@author  Hyosang Jung, Jaewoo.choi
@date    05/19/2022

Note : This file contains the declaration Shadow_test class member function
and this class inherited by Test, So it has all function what Test class have.

*//*__________________________________________________________________________*/
#pragma once
enum DepthComponentSize
{
    Bit16=0,
    Bit24,
    Bit32
};
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
	void Frustum_Setup();
	void Frustrum_Draw();
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
	float borderColor[4];

	//for frustum
	GLuint VAO;
	GLuint VBO;
	GLSLShader Prog;
	GLuint vpLoc;
	std::vector<glm::vec3>vertices;
	glm::vec3 rotate{ 0,0,0 };


	unsigned int depthMapFBO;
	unsigned int depthMap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float near_plane = 0.1f, far_plane = 100.f;
	float FOV;
	float polygonFactor;
	float polygonUnit;
	bool drawBackFacesForRecordDepthPass = false;
	int depthBitSize;
	GLenum depth_component;
};