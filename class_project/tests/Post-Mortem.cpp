
/*!
@file    tessellation.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/14/2022

Note : This file is for 7th demo that shows sphere using tessellation and geometry shader
      You can change inner,outer value. Also, changing factor will change scale value
       Also If you clicked Animated, you can see animation.


*//*__________________________________________________________________________*/

#include "Post-Mortem.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace glm;

PostMortem::PostMortem()
{
    stack = 10;
    slice = 10;
}

PostMortem::~PostMortem()
{

}

void PostMortem::init()
{
    animated = false;
    inner = { 1 };
    outer = { 1 };
    shrink = { 0.9 };
    factor = { 1 };
    meshes.clear();
    meshes.push_back(CreateSphere(stack, slice));
    meshes.push_back(CreateSphere(stack, slice));

    meshes[0].position = { 0,0,0 };
    meshes[0].scale = { 1,1,1 };
    meshes[0].rotation = { 0,0,0 };
    InitMesh(meshes[0]);

    meshes[1].position = { 1,0,0 };
    meshes[1].scale = { 1,1,1 };
    meshes[1].rotation = { 0,0,0 };
    InitMesh(meshes[1]);

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

    cam = { { 0.0f, 0.0f, 3.0f } };

    light = { 0.0f, 0.0f, 3.0f };

    view = glm::translate(view, eye);

    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
}



void PostMortem::Update(float deltaTime)
{
    cam.Update(deltaTime);
    timer += deltaTime * 0.5f;
    if (animated == false)
    {
        timer = 0;
    }

}

void PostMortem::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    GLint  timeLoc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "time");
    glUniform1f(timeLoc, timer);

    GLint  timeLoc1 = glGetUniformLocation(meshes[1].renderProg.GetHandle(), "time");
    glUniform1f(timeLoc1, timer);
    glEnable(GL_DEPTH_TEST);
    OnImGuiRender();

    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    DrawMeshes(meshes[0]);
    DrawMeshes(meshes[1]);


}

void PostMortem::UnLoad()
{
}

void PostMortem::OnImGuiRender()
{
    ImGui::SliderFloat("TessLevelInner", &inner, 0, 20);
    ImGui::SliderFloat("TessLevelOuter", &outer, 0, 20);
    ImGui::SliderFloat("Shrink", &shrink, 0, 1);

    if (ImGui::SliderFloat("Factor", &factor, 1.f, 20.f))
    {
        shrink = 1.f / factor;
        inner = factor;
        outer = factor;
        meshes[0].scale.x = factor * 0.5f;
        meshes[0].scale.y = factor * 0.5f;
        meshes[0].scale.z = factor * 0.5f;
    }

    ImGui::Checkbox("Animation", &animated);
}

void PostMortem::DrawMeshes(Mesh& meshes)
{
    meshes.renderProg.Use();
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    glm::mat4 model = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    model = glm::translate(model, meshes.position);
    model = glm::rotate(model, meshes.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, meshes.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, meshes.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, { meshes.scale.x, meshes.scale.y, meshes.scale.z });

    glUniform4fv(meshes.colorLoc, 1, ValuePtr(useNormal));
    glUniformMatrix4fv(meshes.modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(meshes.viewLoc, 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));
    glUniformMatrix4fv(meshes.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(meshes.LightLoc, 1, ValuePtr(light));
    glUniform3fv(meshes.ViewPosLoc, 1, ValuePtr(cam.GetEye()));
    glUniform1f(inner_loc, inner);
    glUniform1f(outer_loc, outer);
    glUniform1f(shrink_loc, shrink);

    glBindVertexArray(meshes.VAO);

    glDrawElements(GL_PATCHES, meshes.numIndices, GL_UNSIGNED_INT, nullptr);
}

void PostMortem::InitMesh(Mesh& meshes)
{
    SetUpShaders(meshes);
    SetUpMeshes(meshes);
}

void PostMortem::SetUpShaders(Mesh& meshes)
{
    std::string vert = "../shaders/";
    std::string tesc = "../shaders/";
    std::string tese = "../shaders/";
    std::string gs = "../shaders/";
    std::string frag = "../shaders/";

    vert = vert + "postmortem" + ".vert";
    tesc = tesc + "postmortem" + ".tesc";
    tese = tese + "postmortem" + ".tese";
    gs = gs + "postmortem" + ".gs";
    frag = frag + "postmortem" + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_TESS_CONTROL_SHADER, tesc));
    shdr_files.push_back(std::make_pair(GL_TESS_EVALUATION_SHADER, tese));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, gs));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));

    meshes.renderProg.CompileLinkValidate(shdr_files);

    if (GL_FALSE == meshes.renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes.renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void PostMortem::SetUpMeshes(Mesh& meshes)
{

    if (GL_FALSE == meshes.renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes.renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    meshes.setup_mesh();

    inner_loc = glGetUniformLocation(meshes.renderProg.GetHandle(), "TessLevelInner");

    outer_loc = glGetUniformLocation(meshes.renderProg.GetHandle(), "TessLevelOuter");

    shrink_loc = glGetUniformLocation(meshes.renderProg.GetHandle(), "shrink");

}

