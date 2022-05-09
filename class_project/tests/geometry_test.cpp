/*!
@file    Curve.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/09/2022

Note : 

*//*__________________________________________________________________________*/
#include "geometry_test.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include<vector>
#include"../glhelper.h"
#include<random>
using namespace glm;

GeometryTest::GeometryTest()
{
}

GeometryTest::~GeometryTest()
{
}

void GeometryTest::init()
{

    mesh = CreateCube(2, 2);

    mesh.position = { 0,0,-3 };
    mesh.scale = { 1,1,1 };
    mesh.rotation = { 10,0,0 };

    std::string vert = "../shaders/";
    std::string gs = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + "geometry_test" + ".vert";
    gs = gs + "geometry_test" + ".gs";
    frag = frag + "geometry_test" + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, gs));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    mesh.renderProg.CompileLinkValidate(shdr_files);

    if (GL_FALSE == mesh.renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << mesh.renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }



    mesh.setup_mesh();

    view = {
     1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1
    };
    projection = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
    };
    eye = { 0.0f, 0.0f, -3.0f };
    light = { 0.0f, 0.0f, 3.0f };

    view = glm::translate(view, eye);
    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
}

void GeometryTest::Update(float deltaTime)
{
    //update_vertice();
}

void GeometryTest::Draw()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //mesh.renderProg.Use();


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnImGuiRender();

    mesh.draw(useNormal, view, projection, light, -eye);
}

void GeometryTest::UnLoad()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //useNormal.r = 1;

}

void GeometryTest::OnImGuiRender()
{
    
}

