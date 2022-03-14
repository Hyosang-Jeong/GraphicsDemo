#pragma once
class Test
{
public:
	Test() {};
	virtual ~Test() {};

	virtual void Update(float deltaTime) {}
	virtual void Draw() {}
	virtual void OnImGuiRender() {};
};