#include "TestTriangle.h"
#include"../glhelper.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
TriangleTest::TriangleTest()
{

}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{
	meshes.push_back(CreatePlane(6, 6));
	meshes.push_back(CreateCube(2, 2));
	meshes.push_back(CreateSphere(8, 8));
	meshes.push_back(CreateTorus(16, 32, 0, TWO_PI));
	meshes.push_back(CreateCylinder(1, 8));
	meshes.push_back(CreateCone(16, 8));

	meshes[PLANE].init("triangle",{ -1.5,1,-2 }, { 1,1,1 }, { 0,0,0 });
	meshes[CUBE].init("triangle", { 0,1,-2 }, { 1,1,1 }, { 0,0,0 });
	meshes[SPHERE].init("triangle", { +1.5,1,-2 }, { 1,1,1 }, { 0,0,0 });
	meshes[TORUS].init("triangle", { -1.5,-1,-2 }, { 1,1,1 }, { 0,0,0 });
	meshes[CYLINDER].init("triangle", { 0,-1,-2 }, { 1,1,1 }, { 0,0,0 });
	meshes[CONE].init("triangle", { +1.5,-1,-2 }, { 1,1,1 }, { 0,0,0 });
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

void TriangleTest::Update(float deltaTime)
{
    //for (int i = 0; i < NUM_MESHES; i++)
    //{
    //    meshes[i].compute_matrix(deltaTime);
    //}
}

void TriangleTest::Draw()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

     //Display FPS in another viewport

    ImGui::Begin("Triangle Position/Color");
    onOffSwitch();

    for (int i = 0; i < NUM_MESHES; i++)
    {
            meshes[i].draw(useNormal, view,  projection, light, -eye);
    }

}

void TriangleTest::OnImGuiRender()
{
}

void TriangleTest::UnLoad()
{
    meshes.clear();
    
}

bool TriangleTest::is_switch_pressed(const char* buttonName, bool& buttonType)
{
    if (ImGui::Button(buttonName))
    {
        buttonType = !buttonType;
    }
    return buttonType;
}

void TriangleTest::onOffSwitch()
{
    if (ImGui::Button("Wire frame"))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        useNormal.r= -1;
    }
    if (ImGui::Button("Fill meshes"))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        useNormal.r = 1;
    }
    if (is_switch_pressed("Plane", meshSwitch[PLANE]))
    {

        ImGui::SliderAngle("x_dgree", &meshes[PLANE].Get_Rotation().x);
        meshes[PLANE].set_rotation(meshes[PLANE].Get_Rotation());

        ImGui::SliderAngle("y_dgree", &meshes[PLANE].Get_Rotation().y);
        meshes[PLANE].set_rotation(meshes[PLANE].Get_Rotation());

        ImGui::SliderAngle("z_dgree", &meshes[PLANE].Get_Rotation().z);
        meshes[PLANE].set_rotation(meshes[PLANE].Get_Rotation());

        int tmp1 = meshes[PLANE].get_stack_slice()[0];
        int tmp2 = meshes[PLANE].get_stack_slice()[1];
        ImGui::SliderInt2("plane stack and slice", meshes[PLANE].get_stack_slice(),1,10);
        if (meshes[PLANE].stack_slice[0] != tmp1 || meshes[PLANE].stack_slice[1] != tmp2)
        {
            meshes[PLANE] = CreatePlane(meshes[PLANE].get_stack_slice()[0], meshes[PLANE].get_stack_slice()[1]);
            meshes[PLANE].init("triangle", { -1.5,1,-2 }, { 1,1,1 }, { 0,0,0 });
        }
    }
    if (is_switch_pressed("Cube", meshSwitch[CUBE]))
    {

        ImGui::SliderAngle("Cube x dgree", &meshes[CUBE].Get_Rotation().x);
        meshes[CUBE].set_rotation(meshes[CUBE].Get_Rotation());


        ImGui::SliderAngle("Cube y dgree", &meshes[CUBE].Get_Rotation().y);
        meshes[CUBE].set_rotation(meshes[CUBE].Get_Rotation());

        ImGui::SliderAngle("Cube z dgree", &meshes[CUBE].Get_Rotation().z);
        meshes[CUBE].set_rotation(meshes[CUBE].Get_Rotation());

        int tmp1 = meshes[CUBE].get_stack_slice()[0];
        int tmp2 = meshes[CUBE].get_stack_slice()[1];
        ImGui::SliderInt2("CUBE stack and slice", meshes[CUBE].get_stack_slice(), 2, 20);
        if (meshes[CUBE].stack_slice[0] != tmp1 || meshes[CUBE].stack_slice[1] != tmp2)
        {
            meshes[CUBE] = CreateCube(meshes[CUBE].get_stack_slice()[0], meshes[CUBE].get_stack_slice()[1]);
            meshes[CUBE].init("triangle", { 0,1,-2 }, { 1,1,1 }, { 0,0,0 });
        }
    }
    if (is_switch_pressed("Sphere", meshSwitch[SPHERE]))
    {
        ImGui::SliderAngle("Sphere x dgree", &meshes[SPHERE].Get_Rotation().x);
        meshes[SPHERE].set_rotation(meshes[SPHERE].Get_Rotation());

        ImGui::SliderAngle("Sphere y dgree", &meshes[SPHERE].Get_Rotation().y);
        meshes[SPHERE].set_rotation(meshes[SPHERE].Get_Rotation());

        ImGui::SliderAngle("Sphere z dgree", &meshes[SPHERE].Get_Rotation().z);
        meshes[SPHERE].set_rotation(meshes[SPHERE].Get_Rotation());

        int tmp1 = meshes[SPHERE].get_stack_slice()[0];
        int tmp2 = meshes[SPHERE].get_stack_slice()[1];
        ImGui::SliderInt2("SPHERE stack and slice", meshes[SPHERE].get_stack_slice(), 5, 30);
        if (meshes[SPHERE].stack_slice[0] != tmp1 || meshes[SPHERE].stack_slice[1] != tmp2)
        {
            meshes[SPHERE] = CreateSphere(meshes[SPHERE].get_stack_slice()[0], meshes[SPHERE].get_stack_slice()[1]);
            meshes[SPHERE].init("triangle", { +1.5,1,-2 }, { 1,1,1 }, { 0,0,0 });
        }

    }
    if (is_switch_pressed("Torus", meshSwitch[TORUS]))
    {
        ImGui::SliderAngle("Torus x dgree", &meshes[TORUS].Get_Rotation().x);
        meshes[TORUS].set_rotation(meshes[TORUS].Get_Rotation());

        ImGui::SliderAngle("Torus y dgree", &meshes[TORUS].Get_Rotation().y);
        meshes[TORUS].set_rotation(meshes[TORUS].Get_Rotation());

        ImGui::SliderAngle("Torus z dgree", &meshes[TORUS].Get_Rotation().z);
        meshes[TORUS].set_rotation(meshes[TORUS].Get_Rotation());
        int tmp1 = meshes[TORUS].get_stack_slice()[0];
        int tmp2 = meshes[TORUS].get_stack_slice()[1];
        ImGui::SliderInt2("TORUS stack and slice", meshes[TORUS].get_stack_slice(), 10, 40);
        if (meshes[TORUS].stack_slice[0] != tmp1 || meshes[TORUS].stack_slice[1] != tmp2)
        {
            meshes[TORUS] = CreateTorus(meshes[TORUS].get_stack_slice()[0], meshes[TORUS].get_stack_slice()[1], 0,TWO_PI);
            meshes[TORUS].init("triangle", { -1.5,-1,-2 }, { 1,1,1 }, { 0,0,0 });
        }
    }
    if (is_switch_pressed("Cylinder", meshSwitch[CYLINDER]))
    {
        ImGui::SliderAngle("Cylinder x dgree", &meshes[CYLINDER].Get_Rotation().x);
        meshes[CYLINDER].set_rotation(meshes[CYLINDER].Get_Rotation());

        ImGui::SliderAngle("Cylinder y dgree", &meshes[CYLINDER].Get_Rotation().y);
        meshes[CYLINDER].set_rotation(meshes[CYLINDER].Get_Rotation());

        ImGui::SliderAngle("Cylinder z dgree", &meshes[CYLINDER].Get_Rotation().z);
        meshes[CYLINDER].set_rotation(meshes[CYLINDER].Get_Rotation());
        int tmp1 = meshes[CYLINDER].get_stack_slice()[0];
        int tmp2 = meshes[CYLINDER].get_stack_slice()[1];
        ImGui::SliderInt2("CYLINDER stack and slice", meshes[CYLINDER].get_stack_slice(), 1, 15);
        if (meshes[CYLINDER].stack_slice[0] != tmp1 || meshes[CYLINDER].stack_slice[1] != tmp2)
        {
            meshes[CYLINDER] = CreateCylinder(meshes[CYLINDER].get_stack_slice()[0], meshes[CYLINDER].get_stack_slice()[1]);
            meshes[CYLINDER].init("triangle", { 0,-1,-2 }, { 1,1,1 }, { 0,0,0 });
        }

    }
    if (is_switch_pressed("Cone", meshSwitch[CONE]))
    {
        ImGui::SliderAngle("Cone x dgree", &meshes[CONE].Get_Rotation().x);
        meshes[CONE].set_rotation(meshes[CONE].Get_Rotation());

        ImGui::SliderAngle("Cone y dgree", &meshes[CONE].Get_Rotation().y);
        meshes[CONE].set_rotation(meshes[CONE].Get_Rotation());

        ImGui::SliderAngle("Cone z dgree", &meshes[CONE].Get_Rotation().z);
        meshes[CONE].set_rotation(meshes[CONE].Get_Rotation());
        int tmp1 = meshes[CONE].get_stack_slice()[0];
        int tmp2 = meshes[CONE].get_stack_slice()[1];
        ImGui::SliderInt2("CONE stack and slice", meshes[CONE].get_stack_slice(), 10, 30);
        if (meshes[CONE].stack_slice[0] != tmp1 || meshes[CONE].stack_slice[1] != tmp2)
        {
            meshes[CONE] = CreateCone(meshes[CONE].get_stack_slice()[0], meshes[CONE].get_stack_slice()[1]);
            meshes[CONE].init("triangle", { +1.5,-1,-2 }, { 1,1,1 }, { 0,0,0 });
        }
    }
}
