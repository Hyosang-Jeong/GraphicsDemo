/*!
@file    Shadow.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/19/2022

Note : This file is for 8th demo that shows sphere and cube. You can modify Light position
       Field of view and nearest depth and far depth. You can also change camera view
       with mouse dragging and Keyboard button w,a,s,d and arrow buttons.

*//*__________________________________________________________________________*/
#include "Shadow.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>


Shadow_test::~Shadow_test()
{
}

void Shadow_test::init()
{
    polygonFactor = 2.0f;
    polygonUnit = 4.0f;

    mesh = CreateCube(30, 30);
    sphere = CreateSphere(30, 30);
    plane = CreatePlane(30, 30);
    cone = CreateCone(30, 30);

    mesh.init("depth", { 0, 0, 0 }, { 0.5,0.5,0.5 });
    sphere.init("depth", { 2.0f, 4.0f, 0.0f }, { 0.5,0.5,0.5 });
    plane.init("depth", { 0,-0.5,0 }, { 1000,1000,1000 }, { -HALF_PI,0,0 });
    cone.init("depth", { -3, 0, 0 });


    meshes.push_back(mesh);
    meshes.push_back(sphere);
    meshes.push_back(plane);
    meshes.push_back(cone);

    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].setup_shdrpgm("depth");
        glUseProgram(meshes[i].renderProg.GetHandle());
        glUniform1f(glGetUniformLocation(meshes[i].renderProg.GetHandle(), "near_plane"), near_plane);
        glUniform1f(glGetUniformLocation(meshes[i].renderProg.GetHandle(), "far_plane"), far_plane);
    }

    DepthMap_Setup();
    camera = { {0.f, 2.f,10.0f}};
    light = { 0.f, 10.0f, -20.0f };

    FOV = 60.f;
    frustum_rotate = { 0,0,0 };
    frustum_front = { 0,0,-1 };
     light_Fov = 60.f;
     light_near = 0.1f;
     light_far = 50.f;
     is_camera_view = true;
     shadow_behind = false;
     animated = false;
    Frustum_Setup();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);

    projection = glm::perspective(glm::radians(FOV), (float)GLHelper::width / (float)GLHelper::height, near_plane, far_plane);

}

void Shadow_test::Update(float deltaTime)
{
    camera.Update(deltaTime);
    Frustum_Setup();
    static float timer = 0;
    if (animated)
    {
        timer += deltaTime * 0.5f;
        light.x = 10.f * (cos(timer));
        light.y = 10.f * abs(sin(timer));
        sphere.scale.x = 2.f * abs(sin(timer)) + 0.5f;
        sphere.scale.y = 2.f * abs(sin(timer)) + 0.5f;
        sphere.scale.z = 2.f * abs(sin(timer)) + 0.5f;
    }

}

void Shadow_test::Draw()
{
    glClearColor(0.5, 0.5, 0.5, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (drawBackFacesForRecordDepthPass)
        glCullFace(GL_FRONT);
    else
        glCullFace(GL_BACK);

    glPolygonOffset(polygonFactor, polygonUnit);

    lightProjection = glm::perspective(glm::radians(light_Fov), (float)GLHelper::width / (float)GLHelper::height, light_near, light_far);
    lightView = glm::lookAt(light,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    Depth_Draw();
    Scene_Draw();

    if(is_camera_view)
        Frustrum_Draw(camera.GetViewMatrix());
    else
        Frustrum_Draw(lightView);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnImGuiRender();
}

void Shadow_test::OnImGuiRender()
{
    ImGui::SliderFloat3("Light position", &light.x, -30.f, 30.f);
    sphere.position = light;
    ImGui::SliderFloat("Light FOV", &light_Fov, 30.f, 179.f);
    ImGui::SliderFloat("Light Near Plane", &light_near, 0.1f, 100.f);
    ImGui::SliderFloat("Light Far Plane", &light_far, 10.f, 100.f);
    if (ImGui::Button("0.0 - Nearest depth"))
        borderColor[0] = 0.f;
    if (ImGui::Button("1.0 - Far depth"))
        borderColor[0] = 1.f;

    ImGui::SliderFloat("glPolygonOffset factor", &polygonFactor, 0.0f, 3.f);
    ImGui::SliderFloat("glPolygonOffset units", &polygonUnit, 0.f, 10000.f);
    if (ImGui::Button("Do shadow behind"))
    {
        shadow_behind = !shadow_behind;
    }
    if (ImGui::Button("Animated"))
    {
        animated = !animated;
    }
    const char* items[] = { "128", "256", "512", "1024", "2048", "4096", "8192"};
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
                glBindTexture(GL_TEXTURE_2D, depthMap);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Camera View"))
    {
        is_camera_view =true;
    }
    if (ImGui::Button("Light View"))
    {
        is_camera_view = false;
    }
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
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Draw_meshes(lightProjection, lightView, 1);
}

void Shadow_test::Scene_Draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, GLHelper::width, GLHelper::height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    if (is_camera_view)
        Draw_meshes(projection, camera.GetViewMatrix(), 0);
    else
        Draw_meshes(projection, lightView, 0);
   
}
void Shadow_test::Draw_meshes(glm::mat4 Projection, glm::mat4 view, int mode)
{

    mesh.renderProg.Use();
    GLint modeLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "mode");
    GLint shadowLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "shadowMatrix");
    GLint behindLoc = glGetUniformLocation(mesh.renderProg.GetHandle(), "shadow_behind");
    glUniform1i(behindLoc, static_cast<int>(shadow_behind));
    glUniform1i(modeLoc, mode);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    mesh.draw({ useNormal }, view, Projection, light, camera.GetEye());

    sphere.renderProg.Use();
    modeLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "shadowMatrix");
    behindLoc = glGetUniformLocation(sphere.renderProg.GetHandle(), "shadow_behind");
    glUniform1i(behindLoc, static_cast<int>(shadow_behind));
    glUniform1i(modeLoc, 1);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    sphere.draw({ 1.f,0.8f,0 }, view, Projection, camera.GetEye(), camera.GetEye());

    cone.renderProg.Use();
    modeLoc = glGetUniformLocation(cone.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(cone.renderProg.GetHandle(), "shadowMatrix");
    behindLoc = glGetUniformLocation(cone.renderProg.GetHandle(), "shadow_behind");
    glUniform1i(behindLoc, static_cast<int>(shadow_behind));
    glUniform1i(modeLoc, mode);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    cone.draw({1,1,0 }, view, Projection, light, camera.GetEye());

    plane.renderProg.Use();
    modeLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "mode");
    shadowLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "shadowMatrix");
    behindLoc = glGetUniformLocation(plane.renderProg.GetHandle(), "shadow_behind");
    glUniform1i(behindLoc, static_cast<int>(shadow_behind));
    glUniform1i(modeLoc, mode);
    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    plane.draw({ 1,1,1 }, view, Projection, light, camera.GetEye());


}
void Shadow_test::Frustum_Setup()
{
    vertices.clear();
    glm::vec3 verts[9];

    verts[0] = light;
    std::vector<glm::vec4> a = {
        {-1,-1,-1,1},{-1,1,-1,1},
       {1,1,-1,1}, {1,-1,-1,1},
        {-1,-1,1,1},{-1,1,1,1},
       {1,1,1,1}, {1,-1,1,1},
    };


    for (int i = 0; i < a.size(); ++i) 
    {
        a[i] = inverse(lightProjection * lightView) * a[i];
        a[i].x /= a[i].w;
        a[i].y /= a[i].w;
        a[i].z /= a[i].w;
        a[i].w = 1;
    }
    verts[1] = glm::vec3(a[0]);
    verts[2] = glm::vec3(a[1]);
    verts[3] = glm::vec3(a[2]);
    verts[4] = glm::vec3(a[3]);
    verts[5] = glm::vec3(a[4]);
    verts[6] = glm::vec3(a[5]);
    verts[7] = glm::vec3(a[6]);
    verts[8] = glm::vec3(a[7]);

    vertices.push_back(verts[0]);
    vertices.push_back(verts[1]);

    vertices.push_back(verts[0]);
    vertices.push_back(verts[2]);

    vertices.push_back(verts[0]);
    vertices.push_back(verts[3]);

    vertices.push_back(verts[0]);
    vertices.push_back(verts[4]);

    vertices.push_back(verts[1]);
    vertices.push_back(verts[2]);

    vertices.push_back(verts[2]);
    vertices.push_back(verts[3]);

    vertices.push_back(verts[3]);
    vertices.push_back(verts[4]);

    vertices.push_back(verts[1]);
    vertices.push_back(verts[4]);
 
    vertices.push_back(verts[5]);
    vertices.push_back(verts[6]);
    vertices.push_back(verts[6]);
    vertices.push_back(verts[7]);
    vertices.push_back(verts[7]);
    vertices.push_back(verts[8]);
    vertices.push_back(verts[5]);
    vertices.push_back(verts[8]);
    vertices.push_back(verts[1]);
    vertices.push_back(verts[5]);
    vertices.push_back(verts[2]);
    vertices.push_back(verts[6]);
    vertices.push_back(verts[3]);
    vertices.push_back(verts[7]);
    vertices.push_back(verts[4]);
    vertices.push_back(verts[8]);

    static bool first = true;
    if (first == true)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        std::string vert = "../shaders/";
        std::string frag = "../shaders/";
        vert = vert + "frustrum" + ".vert";
        frag = frag + "frustrum" + ".frag";

        std::vector<std::pair<GLenum, std::string>> shdr_files;
        shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
        shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));

        Prog.CompileLinkValidate(shdr_files);
        if (GL_FALSE == Prog.IsLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs" << "\n";
            std::cout << Prog.GetLog() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        vpLoc = glGetUniformLocation(Prog.GetHandle(), "MVP");
        first = false;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}
void Shadow_test::Frustrum_Draw(glm::mat4 View)
{

    glDisable(GL_DEPTH_TEST);
    glm::mat4 MVP = projection* ((View) ) ;
    Prog.Use();
    glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(MVP));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 32);
    glEnable(GL_DEPTH_TEST);
}