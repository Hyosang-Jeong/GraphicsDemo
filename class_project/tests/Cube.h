#pragma once

#include"Test.h"
#include"../glapp.h"
class Cube : public Test
{
public:
	Cube();
	~Cube();
	void init();
	void Update(float deltaTime)  override;
	void Draw() override;
	void OnImGuiRender() override;
private:
	GLApp cube;
};