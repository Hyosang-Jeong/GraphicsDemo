#include "Toon_Fog.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

Toon_Fog::Toon_Fog()
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
    GLint wid = GLHelper::width;
    GLint hei = GLHelper::height;

   cube = CreateCylinder(10,10);
   sphere = CreateSphere(30, 30);
   sun = CreateSphere(30, 30);
   sphere.init({ 0,0,0 },{ 0.5,0.5,0.5 });
    cube.init();
    light = { 1,0,0 };

    sun.init(light, { 0.5,0.5,0.5 });

    view = glm::translate(view, eye);
    projection = glm::perspective(glm::radians(45.0f), (float)GLHelper::width / (float)GLHelper::height, 0.1f, 100.0f);

}

void Toon_Fog::Update(float deltaTime)
{
    sphere.compute_matrix(deltaTime);
    cube.compute_matrix(deltaTime);
    static float angle = 0;
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
    angle += deltaTime*10;
}

void Toon_Fog::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display FPS in another viewport
    ImGui::Begin("Triangle Position/Color");
    onOffSwitch();

    glm::vec4 sun_color = { 1,0.83,0,1 };
    sphere.draw(useNormal, view, projection, light, { 0.0f, 0.0f, 3.0f });

    sun.draw(sun_color, view, projection, -light, { 0.0f, 0.0f, 3.0f });



   //cube.draw(useNormal, view, projection, { 0,0,1 }, { 0.0f, 0.0f, 3.0f });
}

void Toon_Fog::OnImGuiRender()
{
}

void Toon_Fog::onOffSwitch()
{
    ImGui::SliderAngle("sphere x dgree", &sphere.Get_Rotation().x);
    sphere.set_rotation(sphere.Get_Rotation());

    ImGui::SliderAngle("sphere y dgree", &sphere.Get_Rotation().y);
    sphere.set_rotation(sphere.Get_Rotation());

    ImGui::SliderAngle("sphere z dgree", &sphere.Get_Rotation().z);
    sphere.set_rotation(sphere.Get_Rotation());
    ImGui::SliderFloat3("position_sphere", &sphere.Get_position().x, -1.f, 1.f);
    sphere.set_position(sphere.Get_position());



    ImGui::SliderAngle("Cube x dgree", &cube.Get_Rotation().x);
    cube.set_rotation(cube.Get_Rotation());

    ImGui::SliderAngle("Cube y dgree", &cube.Get_Rotation().y);
    cube.set_rotation(cube.Get_Rotation());

    ImGui::SliderAngle("Cube z dgree", &cube.Get_Rotation().z);
    cube.set_rotation(cube.Get_Rotation());
    ImGui::SliderFloat3("position_cube", &cube.Get_position().x, -10.f, 10.f);
    cube.set_position(cube.Get_position());
}



