#version 450 core

/*  There's no single standard, but I prefer:
    - Setting fixed locations for per-vertex attributes.
    - Setting automatic locations for other attributes 
*/
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

layout (location=1) out vec3 NRM;

uniform mat4 mvpMat;

out vec2 UV;

void main(void) 
{
    gl_Position = mvpMat * vec4(pos, 1.0);
    UV = uv;
    NRM = nrm;
}

