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

#include <functional>
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
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = quinticstep(tx);
    float v = quinticstep(ty);
    float w = quinticstep(tz);

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
    float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
    float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
    float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
    float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
    float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
    float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
    float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

    float du = smoothstepDeriv(tx);
    float dv = smoothstepDeriv(ty);
    float dw = smoothstepDeriv(tz);

    float k0 = a;
    float k1 = (b - a);
    float k2 = (c - a);
    float k3 = (e - a);
    float k4 = (a + d - b - c);
    float k5 = (a + f - b - e);
    float k6 = (a + g - c - e);
    float k7 = (b + c + e + h - a - d - f - g);

    derivs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    derivs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    derivs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

    return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
}

void Gradient_Noise::generate_gradient(float dt)
{
    for (int i = 0; i < tableSize; i++)
    {
        for (int j = 0; j < tableSize * 3; j++) //  *3  because  r  g  b
        {
            glm::vec3 derivs;
            float val = (evalute(glm::vec3(j / 3, 0, i) * frequency, derivs) + 1) * 0.5f;  //     /3  because  r  g  b


            data[i][j] = static_cast<unsigned char>(val * 255.f);
        }
    }
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
   // generate_gradient(0);
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
    std::uniform_real_distribution<float> distrFloat(0, 1);
    auto dice = std::bind(distrFloat, gen);

    // create an array of random values and initialize permutation table
    for (unsigned i = 0; i < tableSize; ++i)
    {

        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * PI;

        float x = cos(phi) * sin(theta);
        float y = sin(phi) * sin(theta);
        float z = cos(theta);
        gradients[i] = glm::vec3(x, y, z);
        permutationTable[i] = i;
    }
    std::uniform_int_distribution<unsigned> distributionInt;
    auto diceInt = std::bind(distributionInt, gen);
    // create permutation table
    for (unsigned i = 0; i < tableSize; ++i)
        std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
    // extend the permutation table in the index range [256:512]
    for (unsigned i = 0; i < tableSize; ++i) {
        permutationTable[tableSize + i] = permutationTable[i];
    }
}


