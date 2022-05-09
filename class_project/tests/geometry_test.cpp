#include "geometry_test.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
using namespace glm;

Geometry_test::Geometry_test()
{
    stack = 16;
    slice = 16;
}

Geometry_test::~Geometry_test()
{

}

void Geometry_test::init()
{
    animated = true;
	meshes.push_back(CreateSphere(stack, slice));
    meshes.push_back(CreateSphere(stack, slice));
    meshes[0].position = { 0,0,0 };
    meshes[0].scale = { 1,1,1 };
    meshes[0].rotation = { 0,0,0 };
    meshes[1].position = { 0,0,0 };
    meshes[1].scale = { 1,1,1 };
    meshes[1].rotation = { 0,0,0 };
    std::string vert = "../shaders/";
    std::string geo = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + "geometry" + ".vert";
    geo = geo + "geometry" + ".gs";
    frag = frag + "geometry" + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, geo));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    meshes[0].renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == meshes[0].renderProg.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << meshes[0].renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    shdr_files.clear();
    vert = "../shaders/";
    frag = "../shaders/";
    vert = vert + "triangle" + ".vert";
    frag = frag + "triangle" + ".frag";
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));

    meshes[1].renderProg.CompileLinkValidate(shdr_files);


    meshes[0].setup_mesh();
    meshes[1].setup_mesh();



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
    light = { 0.0f, 0.0f, 3.0f };

    view = glm::translate(view, eye);
    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
}

void Geometry_test::Update(float deltaTime)
{
    timer += deltaTime;
}

void Geometry_test::Draw()
{
    glClearColor(0.5,0.5,0.5,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    GLint  timeLoc = glGetUniformLocation(meshes[0].renderProg.GetHandle(), "time");
    glUniform1f(timeLoc, timer);
    OnImGuiRender();

    if (animated)
    {
        meshes[0].renderProg.Use();
        meshes[0].draw(useNormal, view, projection, light, -eye);
    }
    else
    {
        meshes[1].renderProg.Use();
        meshes[1].draw(useNormal, view, projection, light, -eye);
    }
}

void Geometry_test::UnLoad()
{
}

void Geometry_test::OnImGuiRender()
{
    if (ImGui::SliderInt("Stack", &stack, 10, 40))
    {
        meshes.clear();
        init();
    }
    if (ImGui::SliderInt("Slice", &slice, 10, 40))
    {
        meshes.clear();
        init();
    }
    ImGui::Checkbox("Animated", &animated);
}

