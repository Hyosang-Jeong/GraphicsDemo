/*!
@file    ValueNoise.h
@author  Hyosang Jung, Jaewoo.choi
@date    04/04/2022

Note : This file contains the declaration Noise class member function
and this class inherited by Test, So it has all function what Test class have.
Also, there is a enum state for what is now demo showing.


*//*__________________________________________________________________________*/
#pragma once
#include"Test.h"
#include <GL/glew.h> 
#include"../glslshader.h"
enum state
{
    value_noise,
    wood,
    fractal,
    marble,
    Turbulence
};
class Noise : public Test
{
public:
    Noise();
    ~Noise();
    void init() override;
    void Update(float deltaTime)  override;
    void Draw() override;
    void OnImGuiRender() override;
    void UnLoad() override;
    void setup_shdrpgm(std::string shader);
    float lerp(float min, float max, float t);
    float smoothstep(const float& t);
    float cosinestep(const float& t);
    float quinticstep(const float& t);
    void generate_random_value();
    void setup_opengl();
    float evalute(glm::vec2 p);

    void generate_fractal(float dt);
    void generate_value_noise(float dt);
    void generate_wood(float dt);
    void generate_marble(float dt);
    void generate_turbulence(float dt);
private:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    GLSLShader Prog;
    unsigned int texture;


    static constexpr unsigned int width = 256;
    static constexpr unsigned int height = 256;
    static constexpr unsigned int size = 100;
    state currstate = value_noise;

    float frequency = 0.1f;
    float frequencyMult = 1.8;
    float amplitudeMult = 0.35;
    int numLayers = 5;

    float max = 1.f;
    float random_values[size][size];
    unsigned char data[height][width * 3] = { 0 };

    float offset = 0.f;
    bool animated = false;

    float vertices[32] = {

0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
-0.5f, -0.5f,-0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[6] = {
	    0, 1, 3, // first triangle
	    1, 2, 3  // second triangle
    };
};