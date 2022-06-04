
/*!
@file    tessellation.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/14/2022

Note : This file is for 7th demo that shows sphere and grass using tessellation and geometry shader
      You can change inner,outer value in grass demo. In the sphere demo, 
      the inner and outer value are depending on the camera distance.
       Also If you clicked Animated, you can see animation.


*//*__________________________________________________________________________*/

#include "tessellation.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace glm;


Tessellation_test::~Tessellation_test()
{

}

void Tessellation_test::init()
{
    animated = false;
    draw_sphere = true;
    inner={ 1 };
    outer={ 1 };
    shrink={ 0.9f };

    meshes.clear();

    meshes.push_back(CreateSphere(stack, slice));
    meshes.push_back(CreatePlane(10, 10));

    meshes[0].position = { 0,0,0 };
    meshes[0].scale = { 1,1,1 };
    meshes[0].rotation = { 0,0,0 };
    meshes[1].position = { 0,0,0 };
    meshes[1].scale = { 2.f,2.f,2.f };
    meshes[1].rotation = { QUARTER_PI,0,0 };

    std::string vert = "../shaders/";
    std::string tesc = "../shaders/";
    std::string tese = "../shaders/";
    std::string gs = "../shaders/";
    std::string frag = "../shaders/";

    vert = vert + "tessellation" + ".vert";
    tesc = tesc + "tessellation" + ".tesc";
    tese = tese + "tessellation" + ".tese";
    gs = gs + "tessellation" + ".gs";
    frag = frag + "tessellation" + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_TESS_CONTROL_SHADER, tesc));
    shdr_files.push_back(std::make_pair(GL_TESS_EVALUATION_SHADER, tese));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, gs));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));

    meshes[0].renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == meshes[0].renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes[0].renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    meshes[0].setup_mesh();
    inner_loc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "TessLevelInner");
    outer_loc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "TessLevelOuter");
    shrink_loc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "shrink");
    

    shdr_files.clear();
    vert = "../shaders/";
    tesc = "../shaders/";
    tese = "../shaders/";
    gs = "../shaders/";
    frag = "../shaders/";

    vert = vert + "grass" + ".vert";
    tesc = tesc + "grass" + ".tesc";
    tese = tese + "grass" + ".tese";
    gs = gs + "grass" + ".gs";
    frag = frag + "grass" + ".frag";

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_TESS_CONTROL_SHADER, tesc));
    shdr_files.push_back(std::make_pair(GL_TESS_EVALUATION_SHADER, tese));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, gs));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    meshes[1].renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == meshes[1].renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes[1].renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    meshes[1].setup_mesh();
    inner_loc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "TessLevelInner");
    outer_loc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "TessLevelOuter");
    shrink_loc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "shrink");

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
    depth = { 3.f };
    eye = { 0.0f, 0.0f, depth };
   
    light = { 0.0f, 0.0f, depth };

    view = glm::translate(view, eye);
    
    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
}



void Tessellation_test::Update(float deltaTime)
{
    timer += deltaTime*0.5f;
    if (animated == false)
    {
        timer = 0;
    }

}

void Tessellation_test::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    glEnable(GL_DEPTH_TEST);
    OnImGuiRender();

    
    if (draw_sphere)
    {
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw_sphere();
    }
    else
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw_grass();
    }
}

void Tessellation_test::Draw_sphere()
{
    shrink_loc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "shrink");
    view = glm::lookAt(eye, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1.f,0 });
    eye = { 0.0f, 0.0f, depth };
    light = { 0.0f, 0.0f, depth };

    meshes[0].renderProg.Use();
    GLint  timeLoc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "time");
    glUniform1f(timeLoc, timer);
    GLint  depthLoc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "depth");
    glUniform1f(depthLoc, -depth);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glm::mat4 model = {
   1,0,0,0,
   0,1,0,0,
   0,0,1,0,
   0,0,0,1
    };
    model = glm::translate(model, meshes[0].position);
    model = glm::rotate(model, meshes[0].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, meshes[0].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, meshes[0].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, { meshes[0].scale.x, meshes[0].scale.y, meshes[0].scale.z });

    glUniform4fv(meshes[0].colorLoc, 1, ValuePtr(useNormal));
    glUniformMatrix4fv(meshes[0].modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(meshes[0].viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(meshes[0].projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(meshes[0].LightLoc, 1, ValuePtr(light));
    glUniform3fv(meshes[0].ViewPosLoc, 1, ValuePtr(-eye));
    glUniform1f(shrink_loc, shrink);
    glBindVertexArray(meshes[0].VAO);

    glDrawElements(GL_PATCHES, meshes[0].numIndices, GL_UNSIGNED_INT, nullptr);
}

void Tessellation_test::Draw_grass()
{
    inner_loc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "TessLevelInner");
    outer_loc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "TessLevelOuter");
    shrink_loc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "shrink");
    view = glm::lookAt(glm::vec3{ 0,3.f,2.f }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1.f,0 });
    eye = { 0,3.f,2.f };
    light = { 0,3.f,2.f };

    meshes[1].renderProg.Use();
    GLint  timeLoc = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "time");
    glUniform1f(timeLoc, timer);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glm::mat4 model = {
   1,0,0,0,
   0,1,0,0,
   0,0,1,0,
   0,0,0,1
    };
    model = glm::translate(model, meshes[1].position);
    model = glm::rotate(model, meshes[1].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, meshes[1].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, meshes[1].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, { meshes[1].scale.x, meshes[1].scale.y, meshes[1].scale.z });

    glm::vec4 color = { 0,1.0f,0 ,1 };
    glUniform4fv(meshes[1].colorLoc, 1, ValuePtr(color));
    glUniformMatrix4fv(meshes[1].modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(meshes[1].viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(meshes[1].projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(meshes[1].LightLoc, 1, ValuePtr(light));
    glUniform3fv(meshes[1].ViewPosLoc, 1, ValuePtr(eye));
    glUniform1f(inner_loc, inner);
    glUniform1f(outer_loc, outer);
    glUniform1f(shrink_loc, shrink);
    glBindVertexArray(meshes[1].VAO);

    glDrawElements(GL_PATCHES, meshes[1].numIndices, GL_UNSIGNED_INT, nullptr);
}

void Tessellation_test::UnLoad()
{
}

void Tessellation_test::OnImGuiRender()
{
    if (ImGui::Button("Sphere"))
    {
        draw_sphere = true;
    }
    if (ImGui::Button("Grass"))
    {
        draw_sphere = false;
    }
    if (draw_sphere)
    {
        ImGui::SliderFloat("Shrink", &shrink, 0, 1);
        ImGui::SliderFloat("Depth", &depth, 3.0f, 30.f);
    }
    else
    {
        ImGui::SliderFloat("TessLevelInner", &inner, 0, 20);
        ImGui::SliderFloat("TessLevelOuter", &outer, 0, 20);
    }

   
    ImGui::Checkbox("Animation", &animated);
}

