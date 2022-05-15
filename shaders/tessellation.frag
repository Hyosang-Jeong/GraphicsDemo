#version 450 core

out vec4 outColor;

in vec3 gNormal;


uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 viewPos;
vec3 norm = normalize(gNormal);

vec3 lightDir = normalize(lightPos); 

vec3 lightColor = vec3(1.0, 1, 1);

float diff = max(dot(norm, lightDir), 0.5f);

void main(void)
{
	outColor =  color * vec4(lightColor,1.0);

	outColor = diff * outColor;

}