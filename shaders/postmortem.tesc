#version 450 core

layout(vertices = 3)out;

in vec3 vpos[];

out vec3 tcPosition[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main(void)
{
	gl_TessLevelInner[0] = TessLevelInner;

	gl_TessLevelOuter[0] = TessLevelOuter;
	gl_TessLevelOuter[1] = TessLevelOuter;
	gl_TessLevelOuter[2] = TessLevelOuter;

	tcPosition[gl_InvocationID] = vpos[gl_InvocationID];
}