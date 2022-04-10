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

float Gradient_Noise::evalute(glm::vec2 p)
{

    int xi = static_cast<int>(p.x);
    int xMin = xi % size;
    float tx = p.x - xi;
    int xMax = (xMin == size - 1) ? 0 : xMin + 1;

    int yi = static_cast<int>(p.y);
    int yMin = yi % size;
    float ty = p.y - yi;
    int yMax = (yMin == size - 1) ? 0 : yMin + 1;

    const float& c00 = random_values[yMin][xMin];
    const float& c10 = random_values[yMin][xMax];
    const float& c01 = random_values[yMax][xMin];
    const float& c11 = random_values[yMax][xMax];

    // remapping of tx and ty using the Smoothstep function 
    float sx = smoothstep(tx);
    float sy = smoothstep(ty);

    // linearly interpolate values along the x axis
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);

    // linearly interpolate the nx0/nx1 along they y axis
    return lerp(nx0, nx1, sy);
}

void Gradient_Noise::generate_value_noise(float dt)
{
    for (int i = 0; i < stack; i++)
    {
        for (int j = 0; j < slice; j++) //  *3  because  r  g  b
        {
            float val = evalute(glm::vec2((j) + dt, i) * frequency);  //     /3  because  r  g  b
            data[i][j] = static_cast<unsigned char>(val * 255.f);
        }
    }
}

Mesh Gradient_Noise::create_gradient_plane(int stacks, int slices)
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

            float val = data[stack][slice];

            v.pos = glm::vec3(col - 0.5f, val/2.0f, row - 0.5f);

            v.nrm = glm::vec3(0.0f, 0.0f, 1.0f);

            v.uv = glm::vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}


void Gradient_Noise::init()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    generate_random_value();

    for (int i = 0; i <= stack; i++)
    {
        for (int j = 0; j <= slice; j++) //  *3  because  r  g  b
        {
            float val = evalute(glm::vec2(j , i) * frequency);  //     /3  because  r  g  b
            data[i][j] = val;
        }
    }

    plane = create_gradient_plane(stack,slice);

    setup_opengl();

    plane.init("gradient_noise");

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
    eye = { 0.0f,  -0.50f, -2.0f };
    view = glm::translate(view, eye);
    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);

}

void Gradient_Noise::Update(float dt)
{
    
}

void Gradient_Noise::Draw()
{
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 model = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
    };

    glUniformMatrix4fv(plane.modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(plane.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(plane.projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(plane.VAO);
    glDrawElements(GL_TRIANGLES,plane.numIndices, GL_UNSIGNED_INT, 0);

    OnImGuiRender();

}

void Gradient_Noise::OnImGuiRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


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

void Gradient_Noise::generate_random_value()

{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrFloat(0, 1);


    // create an array of random values and initialize permutation table
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = 0; j < size; ++j)
        {
            random_values[i][j] = distrFloat(gen);
        }
    }
}

void Gradient_Noise::setup_opengl()
{
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //// texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

