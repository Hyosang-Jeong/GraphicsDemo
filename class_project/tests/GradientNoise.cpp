/*!
@file    ValueNoise.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    04/04/2022

Note : This file is for Third demo that shows
       5 demo that(fractal, valueNoise, Wood, Marble, Turbulence).
       In Value Noise and Wood demo, you can control Frequency by Imgui.
       And In Fracttal,Marble and Turblence demo, you can control frequency
       frequencyMult,amplitudeMult, numLayers. Also If you clicked Animated, all demos are
       update every frame. so it looks moving to left smoothly.
       Also it has Imgui implementing for showing another demo in left top side.

*//*__________________________________________________________________________*/
#include "GradientNoise.h"
#include<random>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

Gradient_Noise::Gradient_Noise()
{}

Gradient_Noise::~Gradient_Noise()
{
}

float Gradient_Noise::smoothstepDeriv(const float& t)
{
    return t * (6 - 6 * t);
}

float Gradient_Noise::evalute(glm::vec3 p, glm::vec3& derivs)
{
    int xi = static_cast<int>(p.x);
    int yi = static_cast<int>(p.y);
    int zi = static_cast<int>(p.z);

    float tx = p.x - xi;
    float ty = p.y - yi;
    float tz = p.z -  zi;

    int xi0 = xi % size;
    int yi0 = yi % size;
    int zi0 = zi % size;

    int xi1 = (xi0 == size - 1) ? 0 : xi0 + 1;
    int yi1 = (yi0 == size - 1) ? 0 : yi0 + 1;
    int zi1 = (zi0 == size - 1) ? 0 : zi0 + 1;

    float du = quinticstepDeriv(tx);
    float dv = quinticstepDeriv(ty);
    float dw = quinticstepDeriv(tz);

    float u = quinticstep(tx);
    float v = quinticstep(ty);
    float w = quinticstep(tz);

    glm::vec3 c000  = random_values[zi0][xi0];
    glm::vec3 c100  = random_values[zi0][xi1];
    glm::vec3 c010  = random_values[zi0][xi0];
    glm::vec3 c110  = random_values[zi0][xi1];
    glm::vec3 c001  = random_values[zi1][xi0];
    glm::vec3 c101 =  random_values[zi1][xi1];
    glm::vec3 c011  = random_values[zi1][xi0];
    glm::vec3 c111  = random_values[zi1][xi1];

    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

     glm::vec3 p000 =  glm::vec3(x0, y0, z0);
     glm::vec3 p100 =  glm::vec3(x1, y0, z0);
     glm::vec3 p010 =  glm::vec3(x0, y1, z0);
     glm::vec3 p110 =  glm::vec3(x1, y1, z0);
                           
     glm::vec3 p001 =  glm::vec3(x0, y0, z1);
     glm::vec3 p101 =  glm::vec3(x1, y0, z1);
     glm::vec3 p011 =  glm::vec3(x0, y1, z1);
     glm::vec3 p111 =  glm::vec3(x1, y1, z1);

	 float  a = dot(c000, p000);
	 float  b = dot(c100, p100);
	 float  c = dot(c010, p010);
	 float  d = dot(c110, p110);
	 float  e = dot(c001, p001);
	 float  f = dot(c101, p101);
	 float  g = dot(c011, p011);
	 float  h = dot(c111, p111);

     float k0 = (b - a);
     float k1 = (c - a);
     float k2 = (e - a);
     float k3 = (a + d - b - c);
     float k4 = (a + f - b - e);
     float k5 = (a + g - c - e);
     float k6 = (b + c + e + h - a - d - f - g);

     derivs.x = du * (k0 + v * k3 + w * k4 + v * w * k6);
     derivs.y = dv * (k1 + u * k3 + w * k5 + u * w * k6);
     derivs.z = dw * (k2 + u * k4 + v * k5 + u * v * k6);

     return a + u * k0 + v * k1 + w * k2 + u * v * k3 + u * w * k4 + v * w * k5 + u * v * w * k6;
}

void Gradient_Noise::generate_value_noise(float dt)
{
    //for (int i = 0; i < stack; i++)
    //{
    //    for (int j = 0; j < slice; j++) //  *3  because  r  g  b
    //    {
    //        float val = evalute(glm::vec2((j) + dt, i) * frequency);  //     /3  because  r  g  b
    //        data[i][j] = static_cast<unsigned char>(val * 255.f);
    //    }
    //}
}

Mesh Gradient_Noise::create_gradient_plane(int stacks, int slices,float dt)
{
    Mesh mesh;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;
            glm::vec3 derivs;
            float val = evalute(glm::vec3(slice+dt*3, 0, stack+sin(dt*3)/2.f) * frequency, derivs);  //     /3  because  r  g  b

            v.pos = glm::vec3(col - 0.5f, val, row - 0.5f);

            v.nrm = glm::vec3(-derivs.x, 1, -derivs.z);

            v.nrm /= v.nrm.length();

            v.uv = glm::vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}


void Gradient_Noise::init()
{
  //  glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    generate_random_value();

    //plane = create_gradient_plane(stack,slice);
    //plane.init("gradient_noise");

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
    eye = { 2.0f,  -3.f, -2.f };
    light = { 0.0f,  2.f, 0.f };
    view = glm::rotate(view, QUARTER_PI, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, QUARTER_PI, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, eye);
    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
}

void Gradient_Noise::Update(float dt)
{
    static float timer = 0;
    timer += dt;
    plane = create_gradient_plane(stack, slice,timer);
    plane.init("gradient_noise");

}

void Gradient_Noise::Draw()
{
    glClearColor(0.68, 0.87, 0.89, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 model = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
    };

    glm::vec3 color(0.68, 0.87, 0.89 );
    glUniform4fv(plane.colorLoc, 1, ValuePtr(color));
    glUniformMatrix4fv(plane.modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(plane.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(plane.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(plane.LightLoc, 1, ValuePtr(light));
    glUniform3fv(plane.ViewPosLoc, 1, ValuePtr(-eye));


    glBindVertexArray(plane.VAO);
    glDrawElements(GL_TRIANGLES,plane.numIndices, GL_UNSIGNED_INT, 0);

    OnImGuiRender();

}

void Gradient_Noise::OnImGuiRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
   
    ImGui::SliderFloat("frequeny", &frequency, 0.f, 0.25f);
    ImGui::SliderFloat3("Eye", &eye.x, -10.f, 10.f);
    ImGui::SliderFloat3("light", &light.x, -10.f, 10.f);
    if (ImGui::Button("Gradient Noise") == true)
    {
        max = 1.f;
        frequency = 0.01f;
       // generate_value_noise(0);
    }
}

void Gradient_Noise::UnLoad()
{

}


float Gradient_Noise::lerp(float min, float max, float t)
{
    return  (min * (1 - t)) + (max * t);
}

float Gradient_Noise::smoothstep(const float& t)
{
    return t * t * (3 - (2 * t));
}


float Gradient_Noise::cosinestep(const float& t)
{
    return (1.f - cos(t * PI)) * 0.5f;
}
float Gradient_Noise::quinticstep(const float& t)
{
    return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
}
float Gradient_Noise::quinticstepDeriv(const float& t)
{
    return 30.f * t * t * (t * (t - 2.f) + 1.f);
}


void Gradient_Noise::generate_random_value()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrFloat(0, 0.5);


        // create an array of random values
        for (unsigned i = 0; i < size; ++i)
        {
            for (unsigned j = 0; j < size; ++j)
            {

                float theta = acos(2 * distrFloat(gen) - 1);
                float phi = 2 * distrFloat(gen) * PI;

                float x = cos(phi) * sin(theta);
                float y = sin(phi) * sin(theta);
                float z = cos(theta);
                random_values[i][j] = glm::vec3(x, y, z);

            }
        }
}


