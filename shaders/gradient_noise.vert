#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 color;

layout (location=1) out vec3 NRM;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec2 TexCoord;
out vec3 FragPos; 
out vec3 Color;

void main(void) 
{
    gl_Position = projection * view * model *vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   TexCoord = aTexCoord;
    NRM = nrm;
    Color = color;
}

