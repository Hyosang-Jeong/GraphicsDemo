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
#include"../Camera.h"


class PostMortem : public Test
{
public:
    PostMortem();
    ~PostMortem();
    void init() override;
    void Update(float deltaTime)  override;
    void Draw() override;
    void UnLoad() override;
    void OnImGuiRender() override;

    std::vector<Mesh> meshes;

    void DrawMeshes(Mesh& meshes);
    void InitMesh(Mesh& meshes);
    void SetUpMeshes(Mesh& meshes);
    void SetUpShaders(Mesh& meshes);

    const GLfloat bgColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };
    const GLfloat one = 1.0f;
    Vec4 useNormal = Vec4(1, 1, 1, -1.0f);
    GLSLShader explode;
    Camera cam;
private:
    glm::mat4  view;
    glm::mat4  projection;
    glm::vec3 eye;
    glm::vec3 light;
    float timer{ 0 };
    int stack;
    int slice;

    GLint inner_loc;
    GLint outer_loc;
    GLint shrink_loc;
    float inner{ 1 };
    float outer{ 1 };
    float shrink{ 0.9f };
    float factor{ 1 };
    bool animated{ false };

};