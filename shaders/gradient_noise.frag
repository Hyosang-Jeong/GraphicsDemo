#version 450 core

layout(location=1) in vec3 NRM;
layout (location = 2) in vec3 coordcolor;

out vec4 outColor;
in vec3 FragPos; 
in vec2 TexCoord;


uniform sampler2D ourTexture;
uniform vec4 color;

//Light calculate
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 u_ambient = vec3(.1, .5, .9);
vec3 norm = normalize(NRM);
vec3 lightDir = normalize(lightPos - FragPos);  
vec3 lightColor = vec3(0.1, 0.1, 0.1);
float diff = max(dot(norm, lightDir), 0.0);
vec3 viewDir = normalize(viewPos - FragPos);

void main()
{

    if (color.r < 0)
    {
        if(color.a<0)
           outColor = texture(ourTexture, TexCoord);  
         else
             outColor = vec4(coordcolor,1);      
    }
    else
    {
           outColor = vec4(u_ambient,1.0) + color * vec4(lightColor,1.0);
				outColor= diff * outColor ;      
    }

}

