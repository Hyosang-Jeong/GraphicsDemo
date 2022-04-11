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
	Turbulence,
        Gradient
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
	float smoothstepDeriv(const float& t);
	float cosinestep(const float& t);
	float quinticstep(const float& t);
	void generate_random_value();
	void setup_opengl();
	float evalute(glm::vec2 p);
	float evalute(glm::vec3 p, glm::vec3& derivs);
	void generate_fractal(float dt);
	void generate_value_noise(float dt);
	void generate_wood(float dt);
	void generate_marble(float dt);
	void generate_turbulence(float dt);
	void generate_gradient(float dt);
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

	float frequency =0.1f;
	float frequencyMult = 1.8;
	float amplitudeMult = 0.35;
	int numLayers = 5;

	float max = 1.f;
	float random_values[size][size];
	glm::vec3 V3random_values[size][size];
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
	float gradientDotV(
	    uint8_t perm, // a value between 0 and 255 
	    float x, float y, float z) const
	{
	    switch (perm & 15) {
	    case  0: return  x + y; // (1,1,0) 
	    case  1: return -x + y; // (-1,1,0) 
	    case  2: return  x - y; // (1,-1,0) 
	    case  3: return -x - y; // (-1,-1,0) 
	    case  4: return  x + z; // (1,0,1) 
	    case  5: return -x + z; // (-1,0,1) 
	    case  6: return  x - z; // (1,0,-1) 
	    case  7: return -x - z; // (-1,0,-1) 
	    case  8: return  y + z; // (0,1,1), 
	    case  9: return -y + z; // (0,-1,1), 
	    case 10: return  y - z; // (0,1,-1), 
	    case 11: return -y - z; // (0,-1,-1) 
	    case 12: return  y + x; // (1,1,0) 
	    case 13: return -x + y; // (-1,1,0) 
	    case 14: return -y + z; // (0,-1,1) 
	    case 15: return -y - z; // (0,-1,-1) 
	    }
	}

	static const unsigned tableSize = 256;
	static const unsigned tableSizeMask = tableSize - 1;
	glm::vec3 gradients[tableSize];
	unsigned permutationTable[tableSize * 2];
	/* inline */
	uint8_t hash(const int& x, const int& y, const int& z) const
	{
	    return permutationTable[permutationTable[permutationTable[x] + y] + z];
	}
};