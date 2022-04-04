#pragma once
#include"Test.h"
#include <GL/glew.h> 
#include"../glslshader.h"
enum state
{
	value_noise,
	fractal,
	wood,
	marble
};
class ValueNoise : public Test
{
public:
	ValueNoise();
	~ValueNoise();
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

	void generate_fractal();
	void generate_value_noise();
	void generate_wood();
	void generate_marble();
private:
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;
	GLSLShader Prog;
	unsigned int texture;
	

	static constexpr unsigned int width = 512;
	static constexpr unsigned int height = 512;
	static constexpr unsigned int size = 100;
	state currstate = value_noise;

	float frequency =0.1f;
	float frequencyMult = 1.8;
	float amplitudeMult = 0.35;
	unsigned numLayers = 5;

	float max = 1.f;
	float random_values[size][size];
	unsigned char data[height][width * 3] = { 0 };

	//float offset = 0.f;
};