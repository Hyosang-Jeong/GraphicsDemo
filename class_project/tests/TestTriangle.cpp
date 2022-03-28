#include "TestTriangle.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
TriangleTest::TriangleTest()
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
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{
    GLint wid = GLHelper::width;
    GLint hei = GLHelper::height;
    for (int i = 0; i < NUM_MESHES; i++)
    {
        meshes.push_back(mesh[i]);
    }

    meshes[PLANE].init({ -0.6,0.5,-1 }, { 1,1,1 }, { 0,0,0 });
    meshes[CUBE].init({ 0,0.5,-2 }, { 1,1,1 }, { 0,0,0 });
    meshes[SPHERE].init({ 0.6,0.5,-3 }, { 1,1,1 }, { 0,0,0 });
    meshes[TORUS].init({ -0.6,-0.5,-4 }, { 1,1,1 }, { 0,0,0 });
    meshes[CYLINDER].init({ 0,-0.5,-5 }, { 1,1,1 }, { 0,0,0 });
    meshes[CONE].init({ 0.6,-0.5,-6 }, { 1,1,1 }, { 0,0,0 });

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);

}

void TriangleTest::Update(float deltaTime)
{
    for (int i = 0; i < NUM_MESHES; i++)
    {
        meshes[i].compute_matrix(deltaTime);
    }

}

void TriangleTest::Draw()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display FPS in another viewport
    ImGui::Begin("Triangle Position/Color");
    onOffSwitch();
    
    //for (int i = 0; i < NUM_MESHES; i++)
    //{
    //    if (meshSwitch[i] == true)
    //    {
    //        meshes[i].draw(useNormal, view, projection);
    //    }
    //}

}

void TriangleTest::OnImGuiRender()
{
}

bool TriangleTest::is_switch_pressed(const char* buttonName, bool& buttonType)
{
    if (ImGui::Button(buttonName))
    {
        buttonType = !buttonType;
    }
    return buttonType;
}

void TriangleTest::onOffSwitch()
{
    if (is_switch_pressed("Plane", meshSwitch[PLANE]))
    {

        ImGui::SliderAngle("x_dgree", &meshes[PLANE].Get_Rotation().x);
        meshes[PLANE].set_rotation(meshes[PLANE].Get_Rotation());

        ImGui::SliderAngle("y_dgree", &meshes[PLANE].Get_Rotation().y);
        meshes[PLANE].set_rotation(meshes[PLANE].Get_Rotation());

        ImGui::SliderAngle("z_dgree", &meshes[PLANE].Get_Rotation().z);
        meshes[PLANE].set_rotation(meshes[PLANE].Get_Rotation());

        ImGui::SliderFloat3("position_plane", &meshes[PLANE].Get_position().x, -1.f,1.f);
        meshes[PLANE].set_position(meshes[PLANE].Get_position());


    }
    if (is_switch_pressed("Cube", meshSwitch[CUBE]))
    {

        ImGui::SliderAngle("Cube x dgree", &meshes[CUBE].Get_Rotation().x);
        meshes[CUBE].set_rotation(meshes[CUBE].Get_Rotation());


        ImGui::SliderAngle("Cube y dgree", &meshes[CUBE].Get_Rotation().y);
        meshes[CUBE].set_rotation(meshes[CUBE].Get_Rotation());

        ImGui::SliderAngle("Cube z dgree", &meshes[CUBE].Get_Rotation().z);
        meshes[CUBE].set_rotation(meshes[CUBE].Get_Rotation());
        ImGui::SliderFloat3("position_cube", &meshes[CUBE].Get_position().x, -1.f, 1.f);
        meshes[CUBE].set_position(meshes[CUBE].Get_position());
    }
    if (is_switch_pressed("Sphere", meshSwitch[SPHERE]))
    {
        ImGui::SliderAngle("Sphere x dgree", &meshes[SPHERE].Get_Rotation().x);
        meshes[SPHERE].set_rotation(meshes[SPHERE].Get_Rotation());

        ImGui::SliderAngle("Sphere y dgree", &meshes[SPHERE].Get_Rotation().y);
        meshes[SPHERE].set_rotation(meshes[SPHERE].Get_Rotation());

        ImGui::SliderAngle("Sphere z dgree", &meshes[SPHERE].Get_Rotation().z);
        meshes[SPHERE].set_rotation(meshes[SPHERE].Get_Rotation());
        ImGui::SliderFloat3("position_sphere", &meshes[SPHERE].Get_position().x, -1.f, 1.f);
        meshes[SPHERE].set_position(meshes[SPHERE].Get_position());
    }
    if (is_switch_pressed("Torus", meshSwitch[TORUS]))
    {
        ImGui::SliderAngle("Torus x dgree", &meshes[TORUS].Get_Rotation().x);
        meshes[TORUS].set_rotation(meshes[TORUS].Get_Rotation());

        ImGui::SliderAngle("Torus y dgree", &meshes[TORUS].Get_Rotation().y);
        meshes[TORUS].set_rotation(meshes[TORUS].Get_Rotation());

        ImGui::SliderAngle("Torus z dgree", &meshes[TORUS].Get_Rotation().z);
        meshes[TORUS].set_rotation(meshes[TORUS].Get_Rotation());

        ImGui::SliderFloat3("position_torus", &meshes[TORUS].Get_position().x, -1.f, 1.f);
        meshes[TORUS].set_position(meshes[TORUS].Get_position());
    }
    if (is_switch_pressed("Cylinder", meshSwitch[CYLINDER]))
    {
        ImGui::SliderAngle("Cylinder x dgree", &meshes[CYLINDER].Get_Rotation().x);
        meshes[CYLINDER].set_rotation(meshes[CYLINDER].Get_Rotation());

        ImGui::SliderAngle("Cylinder y dgree", &meshes[CYLINDER].Get_Rotation().y);
        meshes[CYLINDER].set_rotation(meshes[CYLINDER].Get_Rotation());

        ImGui::SliderAngle("Cylinder z dgree", &meshes[CYLINDER].Get_Rotation().z);
        meshes[CYLINDER].set_rotation(meshes[CYLINDER].Get_Rotation());
        ImGui::SliderFloat3("position_cylinder", &meshes[CYLINDER].Get_position().x, -1.f, 1.f);
        meshes[CYLINDER].set_position(meshes[CYLINDER].Get_position());
    }
    if (is_switch_pressed("Cone", meshSwitch[CONE]))
    {
        ImGui::SliderAngle("Cone x dgree", &meshes[CONE].Get_Rotation().x);
        meshes[CONE].set_rotation(meshes[CONE].Get_Rotation());

        ImGui::SliderAngle("Cone y dgree", &meshes[CONE].Get_Rotation().y);
        meshes[CONE].set_rotation(meshes[CONE].Get_Rotation());

        ImGui::SliderAngle("Cone z dgree", &meshes[CONE].Get_Rotation().z);
        meshes[CONE].set_rotation(meshes[CONE].Get_Rotation());
        ImGui::SliderFloat3("position_cone", &meshes[CONE].Get_position().x, -1.f, 1.f);
        meshes[CONE].set_position(meshes[CONE].Get_position());
    }
}
