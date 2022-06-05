#include "Engine.h"
#include "glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

enum Demo
{
    TRIANGLE,
    TOON_FOG,
    VALUE_NOISE,
    GRADIENT_NOISE,
    CURVE,
    GEOMETRY,
    TESSELLATION,
    SHADOW,
    POINTSHADOW,
};

Engine::Engine()
{
	if (!GLHelper::init(1600, 1000, "Class Project")) 
    {
		std::cout << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
	const char* glsl_version = "#version 450";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
    current = TRIANGLE;
}

Engine::~Engine()
{
    GLHelper::cleanup();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (auto& i : tests)
    {
        delete i;
    }
}

void Engine::Add(Test* demo)
{
	tests.push_back(demo);
}

void Engine::init()
{
	tests[current]->init();
}

void Engine::Update()
{
	glfwPollEvents();
	double delta_time = GLHelper::update_time(1.0);
	tests[current]->Update(static_cast<float>(delta_time));
}

void Engine::Draw()
{
	tests[current]->Draw();
	ImGui::Begin("Demo");
	demo_switch();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(GLHelper::ptr_window);
}

bool Engine::ShouldClose()
{
	return glfwWindowShouldClose(GLHelper::ptr_window);
}

void Engine::demo_switch()
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
    if (ImGui::Button("Hermite & CatmullRom Curve"))
    {
        tests[current]->UnLoad();
        current = CURVE;
        tests[current]->init();
    }
    if (ImGui::Button("Geometry Demo"))
    {
        tests[current]->UnLoad();
        current = GEOMETRY;
        tests[current]->init();
    }
    if (ImGui::Button("Tessellation Demo"))
    {
        tests[current]->UnLoad();
        current = TESSELLATION;
        tests[current]->init();
    }
    if (ImGui::Button("Shadow Demo"))
    {
        tests[current]->UnLoad();
        current = SHADOW;
        tests[current]->init();
    }

    if (ImGui::Button("Point Shadow Demo"))
    {
        tests[current]->UnLoad();
        current = POINTSHADOW;
        tests[current]->init();
    }

}