#pragma once
#include"../math.h"
class Test
{
public:
	Test() {};
	virtual ~Test() {};

	virtual void init() {}
	virtual void Update(float deltaTime) {}
	virtual void Draw() {}
	virtual void OnImGuiRender() {};
	virtual void UnLoad() {}
private:

};