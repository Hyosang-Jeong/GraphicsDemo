#include "glhelper.h"
#include "glapp.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glm/glm.hpp>
#include"tests/TestTriangle.h"

static void draw();
static void update();
static void init();
static void cleanup();

TriangleTest triangle_test;


int main() {
    init();

    while (!glfwWindowShouldClose(GLHelper::ptr_window)) 
    {
        update();
        draw();
    }

    cleanup();
}

static void update() {
    // Part 1
    glfwPollEvents();

    double delta_time = GLHelper::update_time(1.0);
    // write window title with current fps ...
    std::stringstream sstr;
    sstr << std::fixed << std::setprecision(2) << GLHelper::title << ": " << GLHelper::fps;
    glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());

    triangle_test.Update(GLHelper::update_time(1.0));
}

static void draw() {

    triangle_test.Draw();
    // Render dear imgui into screen
    //ImGui::Render();
   // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Part 2: swap buffers: front <-> back
    glfwSwapBuffers(GLHelper::ptr_window);
}

static void init() {
    // Part 1

    if (!GLHelper::init(1200, 1200, "Class Project")) {

        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Part 2
    triangle_test.init();
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void cleanup() {
    // Part 1
    GLHelper::cleanup();
    // Part 2

    //todo destructor에서 하는걸로 change
    //triangle_test.triangle.cleanup();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
