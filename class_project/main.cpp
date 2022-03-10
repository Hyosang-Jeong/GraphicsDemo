/*!
@file    main.cpp
@author  pghali@digipen.edu
@date    10/11/2016

This file uses functionality defined in types GLHelper and GLApp to initialize
an OpenGL context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
// Extension loader library's header must be included before GLFW's header!!!
#include "glhelper.h"
#include "glapp.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glm/glm.hpp>
/*                                                   type declarations
----------------------------------------------------------------------------- */

/*                                                      function declarations
----------------------------------------------------------------------------- */
static void draw();
static void update();
static void init();
static void cleanup();

/*                                                      function definitions
----------------------------------------------------------------------------- */
/*  _________________________________________________________________________ */
/*! main

@param none

@return int

Indicates how the program existed. Normal exit is signaled by a return value of
0. Abnormal termination is signaled by a non-zero return value.
Note that the C++ compiler will insert a return 0 statement if one is missing.
*/
int main() {
    // Part 1
    init();

    // Part 2
    while (!glfwWindowShouldClose(GLHelper::ptr_window)) {
        // Part 2a
        update();
        // Part 2b
        draw();
    }

    // Part 3
    cleanup();
    

    
}

/*  _________________________________________________________________________ */
/*! update
@param none
@return none

Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
For now, there are no objects to animate nor keyboard, mouse button click,
mouse movement, and mouse scroller events to be processed.
*/
static void update() {
    // Part 1
    glfwPollEvents();

    // Part 2
    // time between previous and current frame
    double delta_time = GLHelper::update_time(1.0);
    // write window title with current fps ...
    std::stringstream sstr;
    sstr << std::fixed << std::setprecision(2) << GLHelper::title << ": " << GLHelper::fps;
    glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());

    // Part 3
    GLApp::update(delta_time);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display FPS in another viewport
    ImGui::Begin("Scene");
    ImGui::Text("Application average %0.3f ms/frame (%.01f FPS)", (1.f / GLHelper::fps) * 1000.f, GLHelper::fps);
    ImGui::End();
}

/*  _________________________________________________________________________ */
/*! draw
@param none
@return none

Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
For now, there's nothing to draw - just paint color buffer with constant color
*/
static void draw() {
    // Part 1
    GLApp::draw();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Part 2: swap buffers: front <-> back
    glfwSwapBuffers(GLHelper::ptr_window);
}

/*  _________________________________________________________________________ */
/*! init
@param none
@return none

Get handle to OpenGL context through GLHelper::GLFWwindow*.
*/
static void init() {
    // Part 1
    if (!GLHelper::init(1800, 900, "Tutorial 1")) {
        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Part 2
    GLApp::init();

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

/*  _________________________________________________________________________ */
/*! cleanup
@param none
@return none

Return allocated resources for window and OpenGL context thro GLFW back
to system.
Return graphics memory claimed through
*/
void cleanup() {
    // Part 1
    GLHelper::cleanup();
    // Part 2
    GLApp::cleanup();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
