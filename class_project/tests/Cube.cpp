#include "Cube.h"

Cube::Cube()
{
}

Cube::~Cube()
{
}

void Cube::init()
{
	cube.init();
}

void Cube::Update(float deltaTime)
{
	cube.update(deltaTime);
}

void Cube::Draw()
{
	cube.draw();
}

void Cube::OnImGuiRender()
{
}
