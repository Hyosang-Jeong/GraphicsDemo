#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices =11) out;


uniform float time;
uniform mat4 model;

layout (location = 0) in vec3  pos[3];
layout (location = 1) in vec3  nrm[3];

layout (location=1) out vec3 NRM;

out vec3 FragPos; 

float random2d(vec3 coord){
  return fract(sin(dot(coord.xyz, vec3(12.9898, 78.233,17.145))) * 43758.5453);
}

vec3 GetNormal(vec4 a_0,vec4 a_1,vec4 a_2)
{
   vec3 a = vec3(a_0) - vec3(a_1);
   vec3 b = vec3(a_2) - vec3(a_1);
   return normalize(cross(a, b));
}  
vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * (((random2d(vec3(position.xyz)+1.5)) +sin(time*5)*(random2d(vec3(normal.xyz)+1.5)) + 1.0) / 2.0); 

    return  position + vec4( direction * abs(sin(time)), 0.0);
} 

void main(void) 
{
    vec3 FragPos;
    
    //normal for each triangle
    vec3  tmp = GetNormal(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_in[2].gl_Position);

    gl_Position = gl_in[0].gl_Position; 
    
    FragPos =vec3(gl_Position);
    NRM = tmp; 
    EmitVertex();

    gl_Position = explode(gl_in[0].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM = tmp; 
    EmitVertex();

    gl_Position = gl_in[1].gl_Position; 
    
    FragPos =vec3(gl_Position);
    NRM = tmp; 
    EmitVertex();


    gl_Position = explode(gl_in[1].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM =  nrm[1]; 
    EmitVertex();

    gl_Position = gl_in[2].gl_Position; 
    
    FragPos =vec3(gl_Position);
    NRM = nrm[2]; 
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM = nrm[2]; 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position; 
    
    FragPos =vec3(gl_Position);
    NRM = nrm[0]; 
    EmitVertex();

    gl_Position = explode(gl_in[0].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM = nrm[0]; 
    EmitVertex();

    EndPrimitive();



    //for top triangle
    gl_Position = explode(gl_in[0].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM = nrm[0]; 
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM =  nrm[1]; 
    EmitVertex();


    gl_Position = explode(gl_in[2].gl_Position, tmp) ; 
    
    FragPos =vec3(gl_Position);
    NRM = nrm[2]; 
    EmitVertex();

     EndPrimitive();
}

