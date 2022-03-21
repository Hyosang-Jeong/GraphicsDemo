#include "TestTriangle.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
TriangleTest::TriangleTest()
{
    camera_init();
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{
    GLint wid = GLHelper::width;
    GLint hei = GLHelper::height;


    plane = mesh[PLANE];
    cube = mesh[CUBE];
    sphere = mesh[SPHERE];
    torus = mesh[TORUS];
    cylinder = mesh[CYLINDER];
    cone = mesh[CONE];//There is no cap in bottom of the cone.

    cube.init({ wid / 6.f, hei * (3.f / 4.f) , 0 }, { 0.5,0.5,0.5 }, { 0,0,0 });

    sphere.init({ wid / 2.f, hei * (3.f / 4.f) , 0 }, { 0.5,0.5,0.5 }, { 0,0,0 });

    torus.init({ wid * (5.f / 6.f), hei * (3.f / 4.f) , 0 }, { 0.5,0.5,0.5 }, { 0,0,0 });

    cylinder.init({ wid / 6.f, hei * (1.f / 4.f) , 0 }, { 0.5,0.5,0.5 }, { 0,0,0 });

    cone.init({ wid / 2.f, hei * (1.f / 4.f) , 0 }, { 0.5,0.5,0.5 }, { 0,0,0 });

    plane.init({ 5.f * wid / 6.f, hei * (1.f / 4.f) , 0 }, { 0.5,0.5,0.5 }, { 0,0,0 });

    position = cube.position;
}

void TriangleTest::Update(float deltaTime)
{

    plane.compute_matrix(deltaTime);
    cube.compute_matrix(deltaTime);
    sphere.compute_matrix(deltaTime);
    torus.compute_matrix(deltaTime);
    cylinder.compute_matrix(deltaTime);
    cone.compute_matrix(deltaTime);

}

void TriangleTest::Draw()
{
    glClearBufferfv(GL_DEPTH, 0, &one);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display FPS in another viewport
    ImGui::Begin("Triangle Position/Color");






    //static float position[3] = { 0.f,0.f,0.f };
    //ImGui::SliderFloat3("position", position, 0.0, 1.0);
    //cube.position.x = rotation[0];
    //cube.position.y = rotation[1];
    //cube.position.z = rotation[2];

    //static float color[4] = { 1.0f,1.0f,1.0f,1.0f };

    // pass the parameters to the shader

    //shdr_pgm.SetUniform("rotation", rotation);
    //shdr_pgm.SetUniform("translation", translation[0], translation[1]);
    // color picker
    //ImGui::ColorEdit3("color", color);
    // multiply triangle's color with this color
    //shdr_pgm.SetUniform("color", color[0], color[1], color[2]);

    /*  Send each part's data to shaders for rendering */
    glUniform4fv(plane.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cube.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(sphere.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(torus.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cylinder.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cone.colorLoc, 1, ValuePtr(useNormal));

    onOffSwitch();




    glUniformMatrix4fv(plane.mvpMatLoc, 1, GL_FALSE, &plane.SRT_mat[0].x);
    glUniformMatrix4fv(plane.rotMatLoc, 1, GL_FALSE, &plane.rotate_mat[0].x);
    glBindVertexArray(plane.VAO);
    if (meshSwitch[PLANE])
        glDrawElements(GL_TRIANGLES, plane.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(cube.mvpMatLoc, 1, GL_FALSE, &cube.SRT_mat[0].x);
    glUniformMatrix4fv(cube.rotMatLoc, 1, GL_FALSE, &cube.rotate_mat[0].x);
    glBindVertexArray(cube.VAO);
    if (meshSwitch[CUBE])
        glDrawElements(GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_INT, nullptr);


    glUniformMatrix4fv(sphere.mvpMatLoc, 1, GL_FALSE, &sphere.SRT_mat[0].x);
    glUniformMatrix4fv(sphere.rotMatLoc, 1, GL_FALSE, &sphere.rotate_mat[0].x);
    glBindVertexArray(sphere.VAO);
    if (meshSwitch[SPHERE])
        glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(torus.mvpMatLoc, 1, GL_FALSE, &torus.SRT_mat[0].x);
    glUniformMatrix4fv(torus.rotMatLoc, 1, GL_FALSE, &torus.rotate_mat[0].x);
    glBindVertexArray(torus.VAO);
    if (meshSwitch[TORUS])
        glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(cylinder.mvpMatLoc, 1, GL_FALSE, &cylinder.SRT_mat[0].x);
    glUniformMatrix4fv(cylinder.rotMatLoc, 1, GL_FALSE, &cylinder.rotate_mat[0].x);
    glBindVertexArray(cylinder.VAO);
    if (meshSwitch[CYLINDER])
        glDrawElements(GL_TRIANGLES, cylinder.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(cone.mvpMatLoc, 1, GL_FALSE, &cone.SRT_mat[0].x);
    glUniformMatrix4fv(cone.rotMatLoc, 1, GL_FALSE, &cone.rotate_mat[0].x);
    glBindVertexArray(cone.VAO);
    if (meshSwitch[CONE])
        glDrawElements(GL_TRIANGLES, cone.numIndices, GL_UNSIGNED_INT, nullptr);
    ImGui::End();
}

void TriangleTest::OnImGuiRender()
{
}

bool TriangleTest::is_switch_pressed(const char* buttonName, bool& buttonType)
{
    
    if (ImGui::Button(buttonName))
    {
        if (buttonType)
            buttonType = false;
        else
            buttonType = true;
    }
    return buttonType;
}

void TriangleTest::onOffSwitch()
{
    if (is_switch_pressed("Plane", meshSwitch[PLANE]))
    {
        ImGui::SliderFloat3("rotation_plane", &rotation[PLANE].x, 0.0, 2.f * PI);
        plane.set_rotation(rotation[PLANE]);

        //ImGui::SliderFloat3("position", &position.x, 0.f,1.f);
        //cube.set_position(position);
    }
    if (is_switch_pressed("Cube", meshSwitch[CUBE]))
    {
        ImGui::SliderFloat3("rotation_cube", &rotation[CUBE].x, 0.0, 2.f * PI);
        cube.set_rotation(rotation[CUBE]);

        //ImGui::SliderFloat3("position", &position.x, 0.f,1.f);
        //cube.set_position(position);
    }
    if (is_switch_pressed("Sphere", meshSwitch[SPHERE]))
    {
        ImGui::SliderFloat3("rotation_sphere", &rotation[SPHERE].x, 0.0, 2.f * PI);
        sphere.set_rotation(rotation[SPHERE]);

        //ImGui::SliderFloat3("position", &position.x, 0.f,1.f);
        //cube.set_position(position);
    }
    if (is_switch_pressed("Torus", meshSwitch[TORUS]))
    {
        ImGui::SliderFloat3("rotation_torus", &rotation[TORUS].x, 0.0, 2.f * PI);
        torus.set_rotation(rotation[TORUS]);

        //ImGui::SliderFloat3("position", &position.x, 0.f,1.f);
        //cube.set_position(position);
    }
    if (is_switch_pressed("Cylinder", meshSwitch[CYLINDER]))
    {
        ImGui::SliderFloat3("rotation_cylinder", &rotation[CYLINDER].x, 0.0, 2.f * PI);
        cylinder.set_rotation(rotation[CYLINDER]);

        //ImGui::SliderFloat3("position", &position.x, 0.f,1.f);
        //cube.set_position(position);
    }
    if (is_switch_pressed("Cone", meshSwitch[CONE]))
    {
        ImGui::SliderFloat3("rotation_cone", &rotation[CONE].x, 0.0, 2.f * PI);
        cone.set_rotation(rotation[CONE]);

        //ImGui::SliderFloat3("position", &position.x, 0.f,1.f);
        //cube.set_position(position);
    }
}
