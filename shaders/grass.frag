#version 450 core

out vec4 outColor;

in vec3 gNormal;
in vec2 uv;

uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 viewPos;
vec3 norm = normalize(gNormal);

vec3 lightDir = normalize(lightPos); 

vec3 lightColor = vec3(1.0, 1, 1);

float diff = max(dot(norm, lightDir), 0.5f);
vec4 bottom_color = { 0,0.390625f,0 ,1 };
vec4 top_color = { 0.5,1.0f,0 ,1 };

void main(void)
{
	//outColor =  color * vec4(lightColor,1.0);

	//outColor = diff * outColor;
	outColor =  mix(bottom_color,top_color,uv.y);
}