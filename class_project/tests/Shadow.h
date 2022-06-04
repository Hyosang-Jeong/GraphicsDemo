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
	Shadow_test() = default;
	~Shadow_test();
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void OnImGuiRender() override;
	void DepthMap_Setup();
	void Frustum_Setup();
	void Frustrum_Draw(glm::mat4 View);
	void Depth_Draw();
	void Scene_Draw();
	void Draw_meshes(glm::mat4 projection, glm::mat4 view, int mode);
	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	const Vec4 useNormal = Vec4(1, 1, 1, 1.0);

private:
	Camera camera;
	Mesh mesh;
	Mesh plane;
	Mesh sphere;
	Mesh cone;

	std::vector<Mesh> meshes;

	glm::mat4  projection{ 0 };
	glm::vec3 light{ 0 };
	float borderColor[4] = { 0 };
	bool is_camera_view{ false };
	//for light

	glm::mat4 lightProjection{ 0 };
	glm::mat4 lightView{ 0 };
	glm::mat4 lightSpaceMatrix{ 0 };
	float light_Fov{ 0 };
	float light_near{ 0 };
	float light_far{ 0 };
	//for frustum
	GLuint VAO{ 0 };
	GLuint VBO{ 0 };
	GLSLShader Prog;
	GLuint vpLoc{ 0 };
	std::vector<glm::vec3>vertices;
	glm::vec3 frustum_rotate{ 0 };
	glm::vec3 frustum_front{ 0 };

	unsigned int depthMapFBO{ 0 };
	unsigned int depthMap{ 0 };
	 int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float near_plane = 0.1f, far_plane = 100.f;
	float FOV{ 0 };
	float polygonFactor{ 0 };
	float polygonUnit{ 0 };
	bool drawBackFacesForRecordDepthPass = false;
	bool shadow_behind{ 0 };
	bool animated{ 0 };
	int depthBitSize{ 0 };

};