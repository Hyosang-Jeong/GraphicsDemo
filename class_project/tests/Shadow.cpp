/*!
@file    Shadow.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/19/2022

Note : 

*//*__________________________________________________________________________*/
#include "Shadow.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

Shadow_test::Shadow_test()
{
}

Shadow_test::~Shadow_test()
{
}

void Shadow_test::init()
{
    mesh = CreateCube(30, 30);
    plane = CreatePlane(30, 30);
    sphere = CreateSphere(30, 30);

    mesh.init("depth", { 0, 0, 0 }, { 0.5,0.5,0.5 });
    plane.init("depth", { 0,-0.5,0 }, { 100,100,100 }, { -HALF_PI,0,0 });
    sphere.init("depth", { 3.f, 0, 0 }, { 0.5,0.5,0.5 });

    camera = { {0,0,3} };
    light = { -2.0f, 4.0f, 0.0f };
    FOV = 45.f;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);

    glPolygonOffset(2.0f, 4.0f);
    DepthMap_Setup();
    projection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, 0.1f, 100.0f);
}

void Shadow_test::Update(float deltaTime)
{
    camera.Update(deltaTime);
}

void Shadow_test::Draw()
{
    glClearColor(0.5, 0.5, 0.5, 1);

    Depth_Draw();
    Scene_Draw();
     
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SliderFloat3("Light position", &light.x, -10.f, 10.f);
    ImGui::SliderFloat("FOV", &FOV, 30.f, 179.f);
}

void Shadow_test::OnImGuiRender()
{
}

void Shadow_test::DepthMap_Setup()
{
    glGenTextures(1, &depthMap); 
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow_test::Depth_Draw()
{

    glm::mat4 lightProjection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, 0.1f, 100.0f);
    glm::mat4 lightView = glm::lookAt(light,
                                                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    mesh.renderProg.Use();
    GLint modeLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "mode");
    GLint shadowLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc,1);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    mesh.draw(useNormal, lightView, lightProjection, light, camera.GetEye());


    sphere.renderProg.Use();
    modeLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc, 1);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    sphere.draw(useNormal, lightView, lightProjection, light, camera.GetEye());

    plane.renderProg.Use();
    modeLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc, 1);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    plane.draw(useNormal, lightView, lightProjection, light, camera.GetEye());
}

void Shadow_test::Scene_Draw()
{

    glm::mat4 lightProjection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, 0.1f, 100.0f);
    glm::mat4 lightView = glm::lookAt(light,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, GLHelper::width, GLHelper::height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    mesh.renderProg.Use();
    GLint modeLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "mode");
    GLint shadowLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc, 0);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    mesh.draw(useNormal, camera.GetViewMatrix(), projection, light, camera.GetEye());

    sphere.renderProg.Use();
    modeLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc, 0);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    sphere.draw({ 1,0,0 }, camera.GetViewMatrix(), projection, light, camera.GetEye());
    plane.renderProg.Use();
     modeLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "mode");
     shadowLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc, 0);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    plane.draw(useNormal, camera.GetViewMatrix(), projection, light, camera.GetEye());


}

