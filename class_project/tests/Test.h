/*!
@file    Test.h
@author  Hyosang Jung, Jaewoo.choi
@date    04/04/2022

Note :This file is for virtual class

*//*__________________________________________________________________________*/
#pragma once

class Test
{
public:
	Test() {};
	virtual ~Test() {};

	virtual void init() {}
	virtual void Update(float ) {}
	virtual void Draw() {}
	virtual void OnImGuiRender() {};
	virtual void UnLoad() {}
private:

};