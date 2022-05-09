/*!
@file    geometry_test.h
@author  Hyosang Jung, Jaewoo.choi
@date    05/09/2022

Note :
*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include <GL/glew.h> 
#include"../glslshader.h"
#include"../Mesh.h"

class GeometryTest : public Test
{
public:
    GeometryTest();
    ~GeometryTest();
    void init() override;
    void Update(float deltaTime)  override;
    void Draw() override;
    void UnLoad() override;
    void OnImGuiRender() override;

    Mesh mesh;
    const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
    const GLfloat one = 1.0f;
    Vec4 useNormal = Vec4(1.f, 0.2f, 0.4f, -1.0f);

private:
    glm::mat4  view;
    glm::mat4  projection;
    glm::vec3 eye;
    glm::vec3 light;

};