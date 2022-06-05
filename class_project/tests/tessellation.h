//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
/*!
@file    tessellation.h
@author  Hyosang Jung, Jaewoo.choi

@date    05/14/2022

Note : This file contains the declaration Tessellation_test class member function
and this class inherited by Test, So it has all function what Test class have.


*//*__________________________________________________________________________*/
#pragma once

#include"Test.h"
#include <GL/glew.h> 
#include"../Mesh.h"
#include"../glslshader.h"


class Tessellation_test : public Test
{
public:
    Tessellation_test() = default;
    ~Tessellation_test();
    void init() override;
    void Update(float deltaTime)  override;
    void Draw() override;
    void Draw_sphere();
    void Draw_grass();
    void UnLoad() override;
    void OnImGuiRender() override;

    std::vector<Mesh> meshes;

    const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
    const GLfloat one = 1.0f;
    Vec4 useNormal = Vec4(1, 1, 1, -1.0f);
    GLSLShader explode;
private:
    glm::mat4  view{ 0 };
    glm::mat4  projection{ 0 };
    glm::vec3 eye{ 0 };
    glm::vec3 light{ 0 };
    float timer{ 0 };
    int stack{ 30 };
    int slice{ 30 };

    GLint inner_loc{ 0 };
    GLint outer_loc{ 0 };
    GLint shrink_loc{ 0 };
    float inner{ 1 };
    float outer{1 };
    float shrink{ 0.9f };
    float depth{ 0.f };
    bool animated{ false };
    bool draw_sphere{ true };
};