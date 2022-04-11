#version 450 core

layout(location=1) in vec3 NRM;

out vec4 outColor;
in vec3 FragPos; 
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 color;

//Light calculate
uniform vec3 lightPos;
uniform vec3 viewPos;
vec3 norm = normalize(NRM);
vec3 lightDir = normalize(lightPos - FragPos);  
vec3 lightColor = vec3(0.5, 0.9, 1.0);
float diff = max(dot(norm, lightDir), 0.0);
vec3 viewDir = normalize(viewPos - FragPos);

void main()
{
   				outColor =  color * vec4(lightColor,1.0);
				outColor= diff * outColor ;
}