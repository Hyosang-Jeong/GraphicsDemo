#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

layout (location=0) out vec3 vClrCoord;
layout (location=1) out vec3 NRM;

uniform mat4 mvpMat;
uniform mat4 rotMat;

out vec2 UV;


void main(void) 
{
    gl_Position = mvpMat * vec4(pos, 1.0);
    vClrCoord = pos;
    UV = uv;
    vec4 tmp = rotMat * vec4(nrm,1.0);
    NRM = -vec3(tmp.x,tmp.y, tmp.z);
}

