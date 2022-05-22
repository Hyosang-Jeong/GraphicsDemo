#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float shrink;
uniform float time;
out vec3 gNormal;

vec3 Verts[3];
vec3 Center;
vec3 Normal;

mat4 rotate_x(float angle)
{
    return mat4(1,0,0,0,0,cos(angle), -sin(angle),0,0,sin(angle),cos(angle),0,0,0,0,1);
}


mat4 rotate_z(float angle)
{
    return mat4(cos(angle), -sin(angle),0,0,sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1);
}

float random2d(vec3 coord)
{
  return sin(dot(coord.xyz, vec3(12.9898, 78.233,17.145)))*2.f;
}

void EmitShrunkVertex(int i)
{
    vec3 move_direction = Verts[i] - Center;
    move_direction = vec3(rotate_x(time*random2d(Center)*10.f) * vec4(move_direction,1));
    gl_Position = projection * view *model *vec4(Center + shrink * move_direction,1);
    gNormal =Normal;
    EmitVertex();
}

void main()
{
    Verts[0] = gl_in[0].gl_Position.xyz;
    Verts[1] = gl_in[1].gl_Position.xyz;
    Verts[2] = gl_in[2].gl_Position.xyz;
    
    Center = (Verts[0]+Verts[1]+Verts[2])/3.0;

    vec3 A = Verts[2]-Verts[0];
    vec3 B = Verts[1]-Verts[0];
    Normal = mat3(transpose(inverse(model))) * normalize(cross(B,A));

    EmitShrunkVertex(0);
    EmitShrunkVertex(1);
    EmitShrunkVertex(2);
    EndPrimitive();
}