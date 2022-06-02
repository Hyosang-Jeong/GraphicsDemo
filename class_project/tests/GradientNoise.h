/*!
@file    GradientNoise.h
@author  Hyosang Jung, Jaewoo.choi
@date    04/11/2022

Note : This file contains the declaration gradient_state class member function
and this class inherited by Test, So it has all function what Test class have.
Also, there is a enum state for what is now demo showing.


*//*__________________________________________________________________________*/
#pragma once
#include"Test.h"
#include <GL/glew.h> 
#include"../glslshader.h"
#include"../Mesh.h"

enum gradient_state
{
	Gradient_noise,
	Gradient_plane,
	Sun
};

class Gradient_Noise : public Test
{
public:
	Gradient_Noise();
	~Gradient_Noise();
	void init() override;
	void Update(float deltaTime)  override;
	void Draw() override;
	void OnImGuiRender() override;
	void UnLoad() override;
	float lerp(float min, float max, float t);
	float smoothstep(const float& t);
	float cosinestep(const float& t);
	float quinticstep(const float& t);
	float quinticstepDeriv(const float& t);
	void generate_random_value();
	float evalute(glm::vec3 p, glm::vec3& derivs);
	float smoothstepDeriv(const float& t);

	void generate_gradient(float dt);
	Mesh create_gradient_plane(int stacks, int slices,float dt);
	Mesh create_gradient_sphere(int stacks, int slices, float dt);
	void update_plane(float dt);
	void update_sun(float dt);
private:

	glm::mat4 view = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4 projection = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::vec3 eye;
	glm::vec3 light;


	static constexpr unsigned int stack = 100;
	static constexpr unsigned int slice = 100;
	static constexpr unsigned int size = 20;
	//for texture
	static constexpr unsigned int width = 256;
	static constexpr unsigned int height = 256;
	unsigned char data[height][width * 3] = { 0 };
	unsigned int texture;


	float frequency = 0.1f;


	float offset = 0.f;
	bool animated = false;
	Mesh plane;
	Mesh sun;


	gradient_state currstate = Gradient_noise;


	static const unsigned tableSize = 256;
	static const unsigned tableSizeMask = tableSize - 1;
	glm::vec3 gradients[tableSize];
	unsigned permutationTable[tableSize * 2];
	/* inline */
	uint8_t hash(const int& x, const int& y, const int& z) const
	{
	    return static_cast<uint8_t>(permutationTable[permutationTable[permutationTable[x] + y] + z]);
	}


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
		return 0;
	}


};