/*!
@file    Toon_Fog.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    04/04/2022

Note : This file is for Second demo that shows
       one demo that two sphere and both of them has toon shading.
       And Yellow Sphere is for fog shading. They are spinning in the demo.
              Also it has Imgui implementing for showing another demo in left top side.

*//*__________________________________________________________________________*/
#include "Toon_Fog.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

Toon_Fog::Toon_Fog() :light(1.f, 0, 0)
{
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
}

Toon_Fog::~Toon_Fog()
{
}

void Toon_Fog::init()
{

   sphere = CreateSphere(30, 30);
   sun = CreateSphere(30, 30);
    light = { 1,0,0 };
    sphere.init("toon_fog",{ 0, 0, 0 }, {0.5,0.5,0.5});
    sun.init("toon_fog", light, { 0.5,0.5,0.5 });

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
    angle = 0;
    FogMax = 5.f;
    FogMin = 2.f;
    layer[0] = 0.7f;
    layer[1] = 0.4f;
    layer[2] = 0.2f;
    sun.renderProg.SetUniform("FogMax", FogMax);
    sun.renderProg.SetUniform("FogMin", FogMin);
    sun.renderProg.SetUniform("layer_first", layer[0]);
    sun.renderProg.SetUniform("layer_second", layer[1]);
    sun.renderProg.SetUniform("layer_third", layer[2]);
    eye = { 0.0f, 0.0f, -3.0f };
    view = glm::translate(view, eye);
    projection = glm::perspective(glm::radians(45.0f), (float)GLHelper::width / (float)GLHelper::height, 0.1f, 100.0f);
}

void Toon_Fog::Update(float deltaTime)
{

    float x =  sin(angle);
    float z =  2.f * cos(angle);
    light = { x,0,z };
    glm::mat4 rot = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
    };
    rot = glm::rotate(rot, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec4 tmp(light.x, light.y, light.z, 1);
    light = rot * tmp;
    sun.set_position({ light });
    //angle += deltaTime;
}

void Toon_Fog::Draw()
{
    glClearColor(0., 0., 0., 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnImGuiRender();


    glm::vec4 sun_color = { 1,0.83,0,1 };
    sphere.draw(useNormal, view, projection, light, { 0.0f, 0.0f, 3.0f });
    sun.draw(sun_color, view, projection, -light, { 0.0f, 0.0f, 3.0f });


}

void Toon_Fog::OnImGuiRender()
{
    ImGui::SliderFloat("Angle", &angle, 0.0f, 10.f);

    if (ImGui::SliderFloat("FogMax", &FogMax, 5.0f, 10.f))
    {
        sun.renderProg.SetUniform("FogMax", FogMax);
    }
    if (ImGui::SliderFloat("FogMin", &FogMin, 0.0f, 5.f))
    {
        sun.renderProg.SetUniform("FogMin", FogMin);
    }

    if (ImGui::SliderFloat("Toon first layer", &layer[0], 0.5f, 1.f))
    {
        sun.renderProg.SetUniform("layer_first", layer[0]);
    }
    if (ImGui::SliderFloat("Toon second layer", &layer[1], 0.0f, 0.7f))
    {
        sun.renderProg.SetUniform("layer_second", layer[1]);
    }
    if (ImGui::SliderFloat("Toon third layer", &layer[2], -0.1f, 0.2f))
    {
        sun.renderProg.SetUniform("layer_third", layer[2]);
    }
}




