#include"Triangle.h"


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(pos,0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.86f, 0.49f, 0.99f, 1.0f);\n"
"}\n\0";


void Triangle::init()
{
    init_shader();
    float vertices[] = {
    -0.5f, -0.5f, 
     0.5f, -0.5f,  
     0.0f,  0.5f  
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Triangle::init_shader()
{
    vertex_shd = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shd, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shd);

    fragment_shd = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shd, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shd);

    pgm = glCreateProgram();
    glAttachShader(pgm, vertex_shd);
    glAttachShader(pgm, fragment_shd);
    glLinkProgram(pgm);
}

void Triangle::draw()
{
    glUseProgram(pgm);
    glBindVertexArray(VAO); 
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
