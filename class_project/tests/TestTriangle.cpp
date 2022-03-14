#include "TestTriangle.h"

TriangleTest::TriangleTest()
{
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{
	triangle.init();
}

void TriangleTest::Update(float deltaTime)
{
	triangle.update(deltaTime);
}

void TriangleTest::Draw()
{
	triangle.draw();
}

void TriangleTest::OnImGuiRender()
{
}
