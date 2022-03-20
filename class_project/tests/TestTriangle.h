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

	Mesh triangle;
	Mesh sphere;




	const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	const Vec4 useNormal = Vec4(0.5f, 0.5f, 1.0f, 1.0f);

private:
};