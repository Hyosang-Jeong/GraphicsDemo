#pragma once
#include"Test.h"
#include <GL/glew.h> 
#include"../glslshader.h"
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
	void generate_random_value();
	void setup_opengl();
	float evalute(glm::vec2 p);
private:
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;
	GLSLShader Prog;
	unsigned int texture;
	

	static constexpr unsigned int width = 512;
	static constexpr unsigned int height = 512;
	static constexpr unsigned int size = 200;


	float random_values[size][size];
	unsigned char data[height][width*3];

};