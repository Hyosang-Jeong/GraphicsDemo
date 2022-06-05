#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float shrink;
uniform float time;
out vec3 gNormal;
out vec2 uv;
vec3 Verts[3];
vec3 Center;
vec3 Normal;

mat4 Rotate(float angle, vec3 axis){
float c = cos(angle);
float s = sin(angle);
float c_diff = 1.0f - c;
float x = axis.x;
float y =  axis.y;
float z = axis.z;

return mat4(
c+x*x*c_diff,y*x*c_diff+z*s, z*x*c_diff-y*s,0.0f,
x*y*c_diff-z*s,c+y*y*c_diff,z*y*c_diff+x*s,0.0f,
x*z*c_diff+y*s,y*z*c_diff-x*s,c+z*z*c_diff,0.0f,
0.0f,0.0f,0.0f,1.0f
);
}

mat4 rotate_y(float angle)
{
    return mat4(
    cos(angle), 0, sin(angle),0,
    0,1,0,0,
    -sin(angle),0,cos(angle),0,
    0,0,0,1
    );
}

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
  return sin(dot(coord.xyz, vec3(12.9898, 78.233,17.145)))*43758.5453f;
}

void main()
{
    Verts[0] = gl_in[0].gl_Position.xyz;
    Verts[1] = gl_in[1].gl_Position.xyz;
    Verts[2] = gl_in[2].gl_Position.xyz;
    
    Center = (Verts[0]+Verts[1]+Verts[2])/3.0;

    vec3 A = Verts[2]-Verts[0];
    vec3 B = Verts[1]-Verts[0];

    Normal = mat3(transpose(inverse(model))) * normalize(cross(A,B));
    vec3 up =  mat3(transpose(inverse(model))) *vec3(0.0f,1.0f,0.0f);

    mat4 tangent_matrix = Rotate
    (
        acos(dot(up,Normal)), cross(up,Normal)
    );

   vec3 grass[3] ={vec3(-0.05f,0,0),vec3(0.1f,0,0),vec3(0 ,0.5f,0)};

   //grass height
   float random_height = random2d(Center+grass[0]);
   random_height = clamp(random_height, 0.5f,0.8f);
   grass[2] = vec3(0 ,random_height,0);

   //rotate y
    mat4 random_rotate_y = rotate_y(random2d(Center+grass[0]));

    //rotate x
    float random_x = random2d(Center+grass[0]);
    random_x = clamp(random_x, radians(-40.f),radians(40.f));
    mat4 random_rotate_x = rotate_x(random_x);

    //wind
    mat4 wind = rotate_z(sin(Center.x + time*2.f) );

    for(int i=0; i<3; i++)
    {
        grass[i] = (tangent_matrix *wind * random_rotate_x * random_rotate_y * vec4(grass[i],1)).xyz;
    }


    gl_Position = projection * view *model * vec4(Center+grass[0] ,1);
    gNormal =Normal;
    uv = vec2(0,0);
    EmitVertex();

    gl_Position = projection * view *model *vec4(Center+grass[1] ,1);
    gNormal =Normal;
    uv = vec2(1,0);
    EmitVertex();

    gl_Position = projection * view *model *vec4( Center+ grass[2],1);
    gNormal =Normal;
    uv = vec2(0.5,1);
    EmitVertex();
    EndPrimitive();

}