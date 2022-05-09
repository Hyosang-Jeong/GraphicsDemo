#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
layout(location=1) in vec3 NRM[];
layout(location=1) out vec3 normalVec;
uniform mat4 View;
uniform mat4 Projection;
uniform float uShrink = 0.9;

in vec4 vColor[];
in vec3 FragPos[];

out vec4 gColor;
out vec3 FragPosition;

vec3 Verts[3];
vec3 Center;

void EmitShrunkVertex(int i)
{
    //vec3 move_direction = Verts[i] - Center;
    //gl_Position = vec4(Center + uShrink * move_direction,1);
    gColor = vColor[i];
    gl_Position = vec4(Verts[i],1);
    EmitVertex();
}

void main()
{
   // Verts[0] = gl_in[0].gl_Position.xyz;
   // Verts[1] = gl_in[1].gl_Position.xyz;
   // Verts[2] = gl_in[2].gl_Position.xyz;

    //Center = (Verts[0] + Verts[1] + Verts[2])/3;
   // EmitShrunkVertex(0);
   // EmitShrunkVertex(1);
   // EmitShrunkVertex(2);
    gl_Position = gl_in[0].gl_Position;
    normalVec = NRM[0];
    FragPosition = FragPos[0];
    gColor = vColor[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    normalVec = NRM[1];
    FragPosition = FragPos[1];
    gColor = vColor[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    normalVec = NRM[2];
    FragPosition = FragPos[2];
    gColor = vColor[2];
    EmitVertex();

    EndPrimitive(); //optional
}