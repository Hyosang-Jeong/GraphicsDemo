#version 450 core

out vec4 outColor;

layout (location=0) in vec3 color;


void main(void)
{
	outColor = vec4( color,1);
}