#include "ValueNoise.h"
#include<random>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
float vertices[] = {

    0.5f,  0.5f, -1.f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   
    0.5f, -0.5f, -1.f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   
   -0.5f, -0.5f,-1.f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  
   -0.5f,  0.5f, -1.f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

ValueNoise::ValueNoise()
{}

ValueNoise::~ValueNoise()
{
}

float ValueNoise::evalute(glm::vec2 p)
{
    p.x /=  float(width);
    p.y /= float(height);
    p.x*= float(size-1);
    p.y*=float(size-1);
    int xi = std::floor(p.x);
    int yi = std::floor(p.y);

    float tx = p.x - xi;
    float ty = p.y - yi;

    int rx0 = xi;
    int rx1 = (rx0 == size) ? 0 : rx0 + 1;
    int ry0 = yi;
    int ry1 = (ry0 == size) ? 0 : ry0 + 1;

    const float& c00 = random_values[ry0][rx0];
    const float& c10 = random_values[ry0][rx1];
    const float& c01 = random_values[ry1][rx0];
    const float& c11 = random_values[ry1][rx1];

    // remapping of tx and ty using the Smoothstep function 
    float sx =  smoothstep( tx);
    float sy = smoothstep( ty);

    // linearly interpolate values along the x axis
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);

    // linearly interpolate the nx0/nx1 along they y axis
    return lerp(nx0, nx1, sy);
}


void ValueNoise::init()
{   
    generate_random_value();
    setup_opengl();
    setup_shdrpgm("value_noise");

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {

            float val = evalute(glm::vec2(j / 3, i));  //     /3  because  r  g  b
            data[i][j] = static_cast<unsigned char>(val * 255.f);
        }
    }
}

void ValueNoise::Update(float )
{

}

void ValueNoise::Draw()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Prog.Use();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
 //   glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void ValueNoise::OnImGuiRender()
{
}

void ValueNoise::UnLoad()
{
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteTextures()
}

void ValueNoise::setup_shdrpgm(std::string shader)
{
    std::string vert = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + shader + ".vert";
    frag = frag + shader + ".frag";

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
}

float ValueNoise::lerp(float min, float max, float t)
{
    return  min * (1 - t) + max * t;
}

float ValueNoise::smoothstep(const float& t)
{
    return t * t * (3 - 2 * t);
}

void ValueNoise::generate_random_value()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrFloat(0,1);
       

    // create an array of random values and initialize permutation table
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = 0; j < size; ++j)
        {
            random_values[i][j] = distrFloat(gen);
        }
    }
}

void ValueNoise::setup_opengl()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

