#pragma once

#include"Test.h"
#include"../glapp.h"
#include"../Mesh.h"

class TriangleTest: public Test
{
public:
	TriangleTest();
	 ~TriangleTest();
	 void init();
	 void Update(float deltaTime)  override;
	 void Draw() override;
	 void OnImGuiRender() override;
	
	std::vector<Mesh> meshes;

	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	const Vec4 useNormal = Vec4(-0.5f, 0.5f, 1.0f, 1.0f);

private:
	glm::mat4  view;
	glm::mat4  projection;

    bool meshSwitch[NUM_MESHES] = { true,true,true,true,true,true };
    bool is_switch_pressed(const char* buttonName, bool &buttonType);
    void onOffSwitch();
};