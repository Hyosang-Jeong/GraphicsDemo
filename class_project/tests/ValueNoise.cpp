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

Noise::Noise()
{}

Noise::~Noise()
{
}

float Noise::evalute(glm::vec2 p)
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
    float sx =  smoothstep( tx);
    float sy = smoothstep( ty);

    // linearly interpolate values along the x axis
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);

    // linearly interpolate the nx0/nx1 along they y axis
    return lerp(nx0, nx1, sy);
}

void Noise::generate_fractal()
{
    memset(data, 0, height * width * 3);
    float maxNoiseVal = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            glm::vec2 val = glm::vec2(j / 3, i) * frequency;  //     /3  because  r  g  b

            float amplitude = 0.35f;

            for (unsigned k = 0; k < numLayers; k++)
            {
                data[i][j] +=static_cast<unsigned char>( evalute(val) * amplitude *255.f);
                val *= frequencyMult;
                amplitude *= amplitudeMult;
            }
            if (data[i][j] > maxNoiseVal)
                maxNoiseVal = data[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            data[i][j] = static_cast<unsigned char>((static_cast<float>(data[i][j]) /maxNoiseVal)*255.f);
        }
    }
}

void Noise::generate_marble()
{
    memset(data, 0, height * width * 3);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            glm::vec2 val = glm::vec2(j / 3, i) * frequency;  //     /3  because  r  g  b

            float amplitude = 0.35f;
            float noiseValue = 0;
            for (unsigned k = 0; k < numLayers; k++)
            {
                noiseValue += evalute(val) * amplitude;
                val *= frequencyMult;
                amplitude *= amplitudeMult;
            }

            data[i][j] = static_cast<unsigned char>(((sin((j/3 + noiseValue * 50) * 2 * PI / 100.f) + 1) / 2.f)*255.f);

        }
    }
}

void Noise::generate_turbulence()
{
    memset(data, 0, height * width * 3);
    float maxNoiseVal = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            glm::vec2 val = glm::vec2(j / 3, i) * frequency;  //     /3  because  r  g  b

            float amplitude = 0.35f;

            for (unsigned k = 0; k < numLayers; k++)
            {
                data[i][j] += static_cast<unsigned char>(std::fabs(2* evalute(val)-1) * amplitude * 255.f);
                val *= frequencyMult;
                amplitude *= amplitudeMult;
            }
            if (data[i][j] > maxNoiseVal)
                maxNoiseVal = data[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            data[i][j] = static_cast<unsigned char>((static_cast<float>(data[i][j]) / maxNoiseVal) * 255.f);
        }
    }
}

void Noise::generate_value_noise()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            float val = evalute(glm::vec2(j / 3, i) * frequency);  //     /3  because  r  g  b
            data[i][j] = static_cast<unsigned char>(val * 255.f);
        }
    }
}

void Noise::generate_wood()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
        {
            float val = evalute(glm::vec2(j / 3, i) * frequency);  //     /3  because  r  g  b
            if (currstate == wood)
                val *= 10.f;
            data[i][j] = static_cast<unsigned char>(val * 255.f);
        }
    }
}



void Noise::init()
{   
    generate_random_value();
    setup_opengl();
    setup_shdrpgm("value_noise");

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width * 3; j++) //  *3  because  r  g  b
            {
                float val = evalute(glm::vec2(j / 3, i) * frequency);  //     /3  because  r  g  b
                data[i][j] = static_cast<unsigned char>(val * 255.f);
            }
        }
}

void Noise::Update(float )
{

}

void Noise::Draw()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Prog.Use();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    if (ImGui::Button("Value Noise") == true)
    {
        currstate = value_noise;
        max = 1.f;
        generate_value_noise();
    }
    if (ImGui::Button("Wood") == true)
    {
        currstate = wood;
        frequency = 0.005f;
        max = 0.2f;
        generate_wood();
    }

    if (ImGui::Button("Fractal") == true)
    {
        currstate = fractal;
        frequencyMult = 1.8;
        amplitudeMult = 0.35;
        numLayers = 5;
        max = 1.f;
        generate_fractal();
    }

    if (ImGui::Button("Marble") == true)
    {
        currstate = marble;
        generate_marble();
    }
    if (ImGui::Button("Turblence") == true)
    {
        currstate = Turbulence;
        frequencyMult = 1.8;
        amplitudeMult = 0.35;
        numLayers = 5;
        max = 1.f;
        generate_turbulence();
    }
    if (ImGui::SliderFloat("Frequency", &frequency, 0.005f, max))
    {
        switch (currstate)
        {
        case value_noise:
        {
            generate_value_noise();
            break;
        }
        case wood:
        {
            generate_wood();
            break;
        }
        case marble:
        {
            generate_marble();
            break;
        }
        case fractal: 
        {
            generate_fractal();
            break; 
        }
        case Turbulence:
        {
            generate_turbulence();
            break;
        }
        }
    }

    if (currstate >= fractal)
    {
        if (ImGui::SliderFloat("frequenyMult", &frequencyMult, 0.f, 10.f))
        {
            switch (currstate)
            {
            case fractal:
            {
                generate_fractal();
                break;
            }
            case marble:
            {
                generate_marble();
                break;
            }
            case Turbulence:
            {
                generate_turbulence();
                break;
            }
            }
        }
        if (ImGui::SliderFloat("amplitudeMult", &amplitudeMult, 0.f, 0.8f))
        {
            switch (currstate)
            {
            case fractal:
            {
                generate_fractal();
                break;
            }
            case marble:
            {
                generate_marble();
                break;
            }
            case Turbulence:
            {
                generate_turbulence();
                break;
            }
            }
        }
        if (ImGui::SliderInt("numLayers", &numLayers, 1, 10))
        {
            switch (currstate)
            {
            case fractal:
            {
                generate_fractal();
                break;
            }
            case marble:
            {
                generate_marble();
                break;
            }
            case Turbulence:
            {
                generate_turbulence();
                break;
            }
            }
        }
    }

}

void Noise::OnImGuiRender()
{
}

void Noise::UnLoad()
{
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteTextures()
}

void Noise::setup_shdrpgm(std::string shader)
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

float Noise::lerp(float min, float max, float t)
{
    return  (min * (1 - t)) + (max * t);
}

float Noise::smoothstep(const float& t)
{
    return t * t * (3 - (2 * t));
}


float Noise::cosinestep(const float& t)
{
    return (1.f - cos(t * PI)) * 0.5f;
}
float Noise::quinticstep(const float& t)
{
    return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
}

void Noise::generate_random_value()

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

void Noise::setup_opengl()
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

