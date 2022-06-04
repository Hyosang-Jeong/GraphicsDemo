#version 450 core

layout (location = 0) in vec3 pos;

out vec3 vpos;

void main(void) 
{
    vpos = pos;
}

