//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
/*!
@file    PointShadow.h
@author  Hyosang Jung, Jaewoo.choi
@date    05/26/2022

Note : This file is for Point shadow demo.

*//*__________________________________________________________________________*/
#include "PointShadow.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace glm;


void PointShadow::init()
{
    meshes.clear();
    light_near = 0.1f;
    light_far = 100.f;
    lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, light_near, light_far);
    projection = glm::perspective(glm::radians(45.f), (float)GLHelper::width / (float)GLHelper::height, 0.1f, 1000.f);
    camera = { {0.f, 0.f,-1.0f} };
    //Light
    light = vec3(0, 0, -20);
    polygonFactor = 2.0f;
    polygonUnit = 4.0f;
    MakeMeshes();
    DepthMap_Setup();
    Setup_shadowTransform();
    Setup_shdrpgm();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);

}

void PointShadow::Update(float deltaTime)
{
    camera.Update(deltaTime);
    meshes[meshes.size() - 1].position = light;
    lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, light_near, light_far);
}

void PointShadow::Draw()
{
    glClearColor(0.5, 0.5, 0.5, 1);

    Depth_Draw();
    Scene_Draw();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnImGuiRender();
}

void PointShadow::OnImGuiRender()
{
    if (ImGui::SliderFloat3("Light position", &light.x, -30.f, 30.f))
    {
        Setup_shadowTransform();
    }
    ////C
    if(ImGui::Button("C"))
    {
        meshes.erase(meshes.end()-1);
        Mesh mesh;
         mesh = CreateTorus(30, 30, -radians(40.f),PI+ radians(40.f));
         mesh.init("triangle", { -3, -2, -33 }, { 2.f,2.f,2.f }, { HALF_PI,PI,0 });
         meshes.push_back(mesh);

         mesh = CreateSphere(30, 30);
         mesh.init("triangle", { light }, { 0.5f,0.5f,0.5f });
         meshes.push_back(mesh);
   }
    if (ImGui::Button("S"))
    {
        meshes.erase(meshes.end() - 1);
        Mesh mesh;
        mesh = CreateTorus(30, 30, -radians(40.f), PI);
        mesh.init("triangle", { -1.5, -2, -33 }, { 1.5f,1.5f,1.5f }, { HALF_PI,PI,0 });
        meshes.push_back(mesh);
        mesh = CreateTorus(30, 30, -radians(50.f), PI );
        mesh.init("triangle", { -1.5, -3., -33 }, { 1.5f,1.5f,1.5f }, { HALF_PI,0,0 });
        meshes.push_back(mesh);

        mesh = CreateSphere(30, 30);
        mesh.init("triangle", { light }, { 0.5f,0.5f,0.5f });
        meshes.push_back(mesh);
    }
    if (ImGui::Button("2"))
    {
        meshes.erase(meshes.end() - 1);
        Mesh mesh;
        //2
        mesh = CreateTorus(30, 30, -radians(20.f), PI + radians(40.f));
        mesh.init("triangle", { 1, -2, -33 }, { 2.f,2.f,2.f }, { HALF_PI,0,0 });
        meshes.push_back(mesh);
        mesh = CreateCube(30, 30);
        mesh.init("triangle", { 1.5, -3, -33 }, { 1.5f,0.5f,0.5f });
        meshes.push_back(mesh);
        mesh = CreateSphere(30, 30);
        mesh.init("triangle", { light }, { 0.5f,0.5f,0.5f });
        meshes.push_back(mesh);
    }
    if (ImGui::Button("5"))
    {
        meshes.erase(meshes.end() - 1);
        Mesh mesh;
    //5
        mesh = CreateCube(30, 30);
        mesh.init("triangle", { 4, -1.5, -33 }, { 1.5f,0.5f,0.5f });
        meshes.push_back(mesh);

        mesh = CreateCube(30, 30);
        mesh.init("triangle", { 3.5, -2.5, -33 }, { 1.5f,0.5f,0.5f }, { 0,0,HALF_PI });
        meshes.push_back(mesh);

        mesh = CreateTorus(30, 30, -radians(50.f), PI);
        mesh.init("triangle", { 3.5, -3.5, -33 }, { 2.f,2.f,2.f }, { HALF_PI,0,0 });
         meshes.push_back(mesh);
        mesh = CreateSphere(30, 30);
        mesh.init("triangle", { light }, { 0.5f,0.5f,0.5f });
        meshes.push_back(mesh);
    }
    if (ImGui::Button("0"))
    {
        meshes.erase(meshes.end() - 1);
        Mesh mesh;
     //0
         mesh = CreateTorus(30, 30, 0, 2*PI);
        mesh.init("triangle", { 6.5, -2.5, -33 }, { 2.f,2.f,2.f }, { HALF_PI,0,0 });
        meshes.push_back(mesh);
        mesh = CreateSphere(30, 30);
        mesh.init("triangle", { light }, { 0.5f,0.5f,0.5f });
        meshes.push_back(mesh);
    }

    const char* items[] = { "128", "256", "512", "1024", "2048", "4096", "8192" };
    static const char* current_item = items[4];

    if (ImGui::BeginCombo("Dimension", current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < 7; n++)
        {
            bool is_selected = (current_item == items[n]);
            if (ImGui::Selectable(items[n], is_selected))
            {
                current_item = items[n];
                SHADOW_WIDTH = 128 * static_cast<int>(pow(2, n));
                SHADOW_HEIGHT = SHADOW_WIDTH;
                glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

                for (unsigned int i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                }
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void PointShadow::DepthMap_Setup()
{
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadow::Setup_shadowTransform()
{
    shadowTransforms.clear();
    shadowTransforms.push_back(lightProjection * glm::lookAt(light, light + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(light, light + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(light, light + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(light, light + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(light, light + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(light, light + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

void PointShadow::MakeMeshes()
{
    Mesh mesh;
    mesh = CreateCube(30, 30);
    mesh.init("triangle", { 0, 0, -30 }, { 40.f,40.f,100.f });
    meshes.push_back(mesh);

    mesh = CreateSphere(30, 30);
    mesh.init("triangle", { 5, 8, -30 }, { 2.f,2.f,2.f });
    meshes.push_back(mesh);

    mesh = CreateCube(30, 30);
    mesh.init("triangle", { -5, -8, -20 }, { 2.f,2.f,2.f });
    meshes.push_back(mesh);

    mesh = CreateCone(30, 30);
    mesh.init("triangle", { -5, 8, -30 }, { 2.f,2.f,2.f });
    meshes.push_back(mesh);

    mesh = CreateTorus(30, 30,0,2*PI);
    mesh.init("triangle", { 5, -8, -35 }, { 2.f,2.f,2.f });
    meshes.push_back(mesh);

    mesh = CreateSphere(30, 30);
    mesh.init("triangle", { light }, { 0.5f,0.5f,0.5f });
    meshes.push_back(mesh);
}

void PointShadow::Setup_shdrpgm()
{
    std::string vert = "../shaders/";
    std::string geo = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + "PointShadow" + ".vert";
    geo = geo + "PointShadow" + ".gs";
    frag = frag + "PointShadow" + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, geo));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    depth_shader.CompileLinkValidate(shdr_files);
    if (GL_FALSE == depth_shader.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << depth_shader.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    shdr_files.clear();
    vert = "../shaders/";
    frag = "../shaders/";
    vert = vert + "PointShadowScene" + ".vert";
    frag = frag + "PointShadowScene" + ".frag";
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    scene_shader.CompileLinkValidate(shdr_files);
    if (GL_FALSE == scene_shader.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << scene_shader.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }

}

void PointShadow::Depth_Draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    depth_shader.Use();
    GLint ShadowTransformLoc = glGetUniformLocation(depth_shader.GetHandle(), "shadowMatrices");
	for (unsigned int i = 0; i < 6; ++i)
	{
        std::string matrix_loc = "shadowMatrices[" + std::to_string(i) + "]";
        ShadowTransformLoc = glGetUniformLocation(depth_shader.GetHandle(), matrix_loc.c_str());
        glUniformMatrix4fv(ShadowTransformLoc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
	}
    RenderScene(depth_shader, shadowTransforms[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadow::Scene_Draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, GLHelper::width, GLHelper::height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene_shader.Use();
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    RenderScene(scene_shader, shadowTransforms[0]);
}

void PointShadow::RenderScene(GLSLShader shader, glm::mat4 )
{

    GLint modelLoc =                       glGetUniformLocation(shader.GetHandle(), "model");
    GLint projectionLoc =                glGetUniformLocation(shader.GetHandle(), "projection");
    GLint viewLoc =                          glGetUniformLocation(shader.GetHandle(), "view");
    GLint ColorLoc =                         glGetUniformLocation(shader.GetHandle(), "color");
    GLint viewPosLoc =                    glGetUniformLocation(shader.GetHandle(), "viewPos");
    GLint LightPosLoc =                    glGetUniformLocation(shader.GetHandle(), "lightPos");
    GLint FarplaneLoc =                   glGetUniformLocation(shader.GetHandle(), "far_plane");
    GLint reverseLoc = glGetUniformLocation(shader.GetHandle(), "reverse_normal");
    glm::mat4 model = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
    };
    glm::vec4 color = { 1.f,0.8f,0.f,1.f };

    for (int i = 0; i < meshes.size(); i++)
    {
        color = { 0.9f , 0.1f, 0.1f , 1.f };
        glUniform1i(reverseLoc, 0);
        if (i == 0)
        {
            color = { 1.f,0.8f,0.f,1.f };
            glUniform1i(reverseLoc, 1);
            glDisable(GL_CULL_FACE);
        }
        if (i == meshes.size() - 1)
        {
            color = { 1.f,1.f,1.f,1.f };
        }
        model =
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };

        model = glm::translate(model, meshes[i].position);
        model = glm::rotate(model, meshes[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, meshes[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, meshes[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, { meshes[i].scale.x,meshes[i].scale.y,meshes[i].scale.z });

        glUniform4fv(ColorLoc, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
     

        glUniform3fv(viewPosLoc, 1, value_ptr(camera.GetEye()));
        glUniform3fv(LightPosLoc, 1, value_ptr(light));
        glUniform1f(FarplaneLoc,  light_far);


        glBindVertexArray(meshes[i].VAO);
        glDrawElements(GL_TRIANGLES, meshes[i].numIndices, GL_UNSIGNED_INT, nullptr);
        glEnable(GL_CULL_FACE);
    }

}
