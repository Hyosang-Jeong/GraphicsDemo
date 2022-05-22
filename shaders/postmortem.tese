#version 450 core

layout (triangles, equal_spacing,ccw) in;

in vec3 tcPosition[];

void main(void) 
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    vec3 point_on_triangle = tcPosition[0]*u +tcPosition[1]*v +tcPosition[2]*w;
    gl_Position = vec4(point_on_triangle,1);

}

