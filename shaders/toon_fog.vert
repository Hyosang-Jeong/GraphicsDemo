#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

layout (location=1) out vec3 NRM;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec2 UV;
out vec3 FragPos; 

void main(void) 
{
    gl_Position = projection * view * model * vec4(pos, 1.0);

    FragPos = vec3(model * vec4(pos, 1.0));

    UV = uv;

   // vec4 normal =  projection * view * model *vec4( nrm,1.0);
   // vec4 normal =  model *vec4( nrm,1.0);
    NRM = mat3(transpose(inverse(model))) * nrm;
}

