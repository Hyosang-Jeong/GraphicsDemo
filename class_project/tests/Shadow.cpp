/*!
@file    Shadow.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/19/2022

Note : This file is for 8th demo that shows sphere and cube. You can modify Light position
       Field of view and nearest depth and far depth. You can also change camera view
       with mouse dragging and Keyboard button w,a,s,d and left right arrow button.

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
    depth_component = GL_DEPTH_COMPONENT32;
    polygonFactor = 2.0f;
    polygonUnit = 4.0f;
    mesh = CreateCube(30, 30);
    sphere = CreateSphere(30, 30);
    plane = CreatePlane(30, 30);

    mesh.init("depth", { 0, 0, 0 }, { 0.5,0.5,0.5 });
    sphere.init("depth", { 2.0f, 4.0f, 0.0f }, { 0.5,0.5,0.5 });
    plane.init("depth", { 0,-0.5,0 }, { 100,100,100 }, { -HALF_PI,0,0 });

    mesh.setup_shdrpgm("debugDepthQuad");
    glUseProgram(mesh.renderProg.GetHandle());
    glUniform1f(glGetUniformLocation(mesh.renderProg.GetHandle(), "near_plane"), near_plane);
    glUniform1f(glGetUniformLocation(mesh.renderProg.GetHandle(), "far_plane"), far_plane);

    sphere.setup_shdrpgm("debugDepthQuad");
    glUseProgram(sphere.renderProg.GetHandle());
    glUniform1f(glGetUniformLocation(sphere.renderProg.GetHandle(), "near_plane"), near_plane);
    glUniform1f(glGetUniformLocation(sphere.renderProg.GetHandle(), "far_plane"), far_plane);

    plane.setup_shdrpgm("debugDepthQuad");
    glUseProgram(plane.renderProg.GetHandle());
    glUniform1f(glGetUniformLocation(plane.renderProg.GetHandle(), "near_plane"), near_plane);
    glUniform1f(glGetUniformLocation(mesh.renderProg.GetHandle(), "far_plane"), far_plane);
    camera = { {0,0,3} };
    light = { 2.0f, 4.0f, 0.0f };
    FOV = 60.f;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    
    DepthMap_Setup();
    projection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, near_plane, far_plane);
    glFlush();
}

void Shadow_test::Update(float deltaTime)
{
    camera.Update(deltaTime);
    //light.x = sin(glfwGetTime()) * 3.0f;
    //light.z = cos(glfwGetTime()) * 2.0f;
    //light.y = 5.0 + cos(glfwGetTime()) * 1.0f;
}

void Shadow_test::Draw()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexStorage2D(GL_TEXTURE_2D, 1, depth_component, SHADOW_WIDTH, SHADOW_HEIGHT);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (drawBackFacesForRecordDepthPass)
        glCullFace(GL_FRONT);
    else
        glCullFace(GL_BACK);

    glPolygonOffset(polygonFactor, polygonUnit);

    Depth_Draw();
    Scene_Draw();


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnImGuiRender();

}

void Shadow_test::OnImGuiRender()
{

    ImGui::SliderFloat3("Light position", &light.x, -10.f, 10.f);
    sphere.position = light;
    ImGui::SliderFloat("FOV", &FOV, 30.f, 179.f);
    if (ImGui::Button("0.0 - Nearest depth"))
        borderColor[0] = 0.f;
    if (ImGui::Button("1.0 - Far depth"))
        borderColor[0] = 1.f;
    ImGui::SliderFloat3("cube position", &mesh.position.x, -10.f, 10.f);
    ImGui::SliderFloat("Near Plane", &near_plane, 0.1f, 1.f);
    ImGui::SliderFloat("Far Plane", &far_plane, 100.f, 1000.f);
    ImGui::SliderFloat("glPolygonOffset factor", &polygonFactor, 0.0f, 3.f);
    ImGui::SliderFloat("glPolygonOffset units", &polygonUnit, 0.f, 10000.f);
    ImGui::Checkbox("Draw Back Faces For Recordding Depth", &drawBackFacesForRecordDepthPass);

    if (ImGui::Button("Depth Component Bit Size : 16 bits"))
        depthBitSize = 0;
    if (ImGui::Button("Depth Component Bit Size : 24 bits"))
        depthBitSize = 1;
    if (ImGui::Button("Depth Component Bit Size : 32 bits"))
        depthBitSize = 2;
}

void Shadow_test::DepthMap_Setup()
{
    glGenTextures(1, &depthMap); 
    glBindTexture(GL_TEXTURE_2D, depthMap);

    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

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

    glm::mat4 lightProjection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(light,
                                                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    //simpleDepthShader.use();
    //simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    //renderScene(simpleDepthShader);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// reset viewport
    //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    glm::mat4 lightProjection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(light,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    if (depthBitSize == DepthComponentSize::Bit16)
        depth_component = GL_DEPTH_COMPONENT16;
    else if (depthBitSize == DepthComponentSize::Bit24)
        depth_component = GL_DEPTH_COMPONENT24;
    else if (depthBitSize == DepthComponentSize::Bit32)
        depth_component = GL_DEPTH_COMPONENT32;

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
    glUniform1i(modeLoc, 1);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    sphere.draw( useNormal, camera.GetViewMatrix(), projection, light, camera.GetEye());

    plane.renderProg.Use();
    modeLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "shadowMatrix");
    glUniform1i(modeLoc, 0);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    plane.draw(useNormal, camera.GetViewMatrix(), projection, light, camera.GetEye());


}

