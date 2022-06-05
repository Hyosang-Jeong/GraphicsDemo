//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
/*!
@file    geometry_test.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/09/2022

Note : This file is for Sixth demo that shows 2 demo
       All of these demo, you can control Slices and stacks by Imgui.
       Also If you clicked Animated, you can see animation.
       

*//*__________________________________________________________________________*/

#include "geometry_test.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace glm;

Geometry_test::Geometry_test() :view{ 0 }, projection{ 0 }, light{ 0 }, eye{ 0 },sphere_mode{false}
{
    stack = 16;
    slice = 16;
}

Geometry_test::~Geometry_test()
{

}

void Geometry_test::init()
{
	meshes.push_back(CreateSphere(stack, slice));
    meshes.push_back(CreateSphere(stack, slice));
    meshes.push_back(CreateSphere(stack, slice));
    meshes.push_back(CreateSphere(stack, slice));
    meshes[0].position = { 0,0,0 };
    meshes[0].scale = { 1,1,1 };
    meshes[0].rotation = { 0,0,0 };

    meshes[1].position = { 0,0,-2 };
    meshes[1].scale = { 1,1,1 };
    meshes[1].rotation = { 0,0,0 };

    meshes[2].position = { 0,0,-2 };
    meshes[2].scale = { 1,1,1 };
    meshes[2].rotation = { 0,0,0 };



    std::string vert = "../shaders/";
    std::string geo = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + "geometry" + ".vert";
    geo = geo + "geometry" + ".gs";
    frag = frag + "geometry" + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, geo));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    meshes[0].renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == meshes[0].renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes[0].renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    shdr_files.clear();
    vert = "../shaders/";
    frag = "../shaders/";
    vert = vert + "triangle" + ".vert";
    frag = frag + "triangle" + ".frag";
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    meshes[1].renderProg.CompileLinkValidate(shdr_files);

    shdr_files.clear();
    vert = "../shaders/";
    geo = "../shaders/";
    frag = "../shaders/";

    vert = vert + "geometry_virus" + ".vert";
    geo = geo + "geometry_virus" + ".gs";
    frag = frag + "geometry_virus" + ".frag";
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, geo));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    meshes[2].renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == meshes[2].renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes[2].renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }


    meshes[0].setup_mesh();
    meshes[1].setup_mesh();
    meshes[2].setup_mesh();


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


void Geometry_test::Update(float deltaTime)
{
    timer += deltaTime;
}

void Geometry_test::Draw()
{
    glDisable(GL_CULL_FACE);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    GLint  timeLoc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "time");
    glUniform1f(timeLoc, timer);
    OnImGuiRender();

    if (animated)
    {
        if (sphere_mode == true)
        {
            glClearColor(0.0, 0.0, 0.0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            meshes[0].renderProg.Use();
            meshes[0].draw(useNormal, view, projection, light, -eye);
        }
        else
        {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            meshes[2].renderProg.Use();
            meshes[2].draw(useNormal, view, projection, light, -eye);
        }
    }
    else
    {
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        meshes[1].renderProg.Use();
        meshes[1].draw(useNormal, view, projection, light, -eye);
    }
}

void Geometry_test::UnLoad()
{
}

void Geometry_test::OnImGuiRender()
{
    if (ImGui::SliderInt("Stack", &stack, 10, 40))
    {
        meshes.clear();
        init();
    }
    if (ImGui::SliderInt("Slice", &slice, 10, 40))
    {
        meshes.clear();
        init();
    }
    ImGui::Checkbox("Animated", &animated);

    if (ImGui::Button("Sphere mode_1"))
    {
        sphere_mode = true;
   }

    if (ImGui::Button("Sphere mode_2"))
    {
        sphere_mode = false;
    }
}

