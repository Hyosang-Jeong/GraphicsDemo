#version 450 core

layout(location=1) in vec3 NRM;

uniform float time;
uniform vec4 color;
out vec4 outColor;
in vec3 FragPos; 

uniform vec3 lightPos;

uniform vec3 viewPos;

vec3 norm = normalize(NRM);

vec3 lightDir = normalize(lightPos - FragPos);  

vec3 lightColor = vec3(1.0, 0.5, 0.1);

float diff = max(dot(norm, lightDir), 0.0);

void main(void)
{
	outColor =  color * vec4(lightColor,1.0);
	outColor = diff * outColor;
	outColor += vec4(sin(time*1.0)/2.0+0.2,cos(time)/3.0+0.3,sin(time*2.0)+0.3,1.0);
}