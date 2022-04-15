#include "glhelper.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glm/glm.hpp>
#include"tests/TestTriangle.h"
#include"tests/Toon_Fog.h"
#include"tests/ValueNoise.h"
#include"tests/GradientNoise.h"
static void draw();
static void update();
static void init();
static void cleanup();
void demo_switch();
enum DEMO
{
    TRIANGLE,
    TOON_FOG,
    VALUE_NOISE,
    GRADIENT_NOISE
};
std::vector<Test*> tests;
DEMO current = TRIANGLE;

int main() 
{

    TriangleTest triangle;
    Toon_Fog toon_fog;
    Noise value_noise;
    Gradient_Noise gradient_noise;

    tests.push_back(&triangle);
    tests.push_back(&toon_fog);
    tests.push_back(&value_noise);
    tests.push_back(&gradient_noise);

    init();

    while (!glfwWindowShouldClose(GLHelper::ptr_window)) 
    {
        update();
        draw();
    }

    cleanup();
}

static void update()
{
    glfwPollEvents();
    double delta_time = GLHelper::update_time(1.0);
    tests[current]->Update(delta_time);
}

static void draw() {

    tests[current]->Draw();

     ImGui::Begin("Demo");
     demo_switch();
     ImGui::Render();
     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(GLHelper::ptr_window);
}
void demo_switch()
{
    if (ImGui::Button("Procedure Modeling"))
    {
        tests[current]->UnLoad();
        current = TRIANGLE;
        tests[current]->init();
    }

    if (ImGui::Button("Toon Fog"))
    {
        tests[current]->UnLoad();
        current = TOON_FOG;
        tests[current]->init();
    }

    if (ImGui::Button("Value Noise"))
    {
        tests[current]->UnLoad();
        current = VALUE_NOISE;
        tests[current]->init();
    }
    if (ImGui::Button("Gradient Noise"))
    {
        tests[current]->UnLoad();
        current = GRADIENT_NOISE;
        tests[current]->init();
    }
}
static void init() {

    if (!GLHelper::init(1600, 1600, "Class Project")) {

        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }


    tests[current]->init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();
}

void cleanup() {

    GLHelper::cleanup();


    //todo Change to Implement in a destructor
    //triangle_test.triangle.cleanup();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
