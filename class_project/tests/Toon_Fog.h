#pragma once

#include"Test.h"
#include"../glapp.h"
#include"../Mesh.h"

class Toon_Fog : public Test
{
public:
	Toon_Fog();
	~Toon_Fog();
	void init();
	void Update(float deltaTime)  override;
	void Draw() override;
	void OnImGuiRender() override;

	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	const Vec4 useNormal = Vec4(0, 0, 1,1.0);

private:
	Mesh sphere;
	Mesh cube;
	Mesh sun;

	glm::mat4  view;
	glm::mat4  projection;
	glm::vec3 eye;
	glm::vec3 light;

	bool meshSwitch[NUM_MESHES] = { true,true,true,true,true,true };
	bool is_switch_pressed(const char* buttonName, bool& buttonType);
	void onOffSwitch();
};