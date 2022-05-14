#version 450 core

out vec4 FragColor;

in vec3 gNormal;

uniform vec3 LightPosition;
uniform vec3 diff;
uniform vec3 Ambient;

void main(void)
{
	vec3 N = normalize(gNormal);
	vec3 L = normalize(-LightPosition);
	float nl = max(dot(N,L),0);

	vec3 color = nl * diff + Ambient;

	FragColor = vec4(color,1);
}