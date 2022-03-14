#pragma once

#include"Test.h"
#include"../glapp.h"
class TriangleTest: public Test
{
public:
	TriangleTest();
	 ~TriangleTest();
	 void init();
	 void Update(float deltaTime)  override;
	 void Draw() override;
	 void OnImGuiRender() override;
private:
	GLApp triangle;
};