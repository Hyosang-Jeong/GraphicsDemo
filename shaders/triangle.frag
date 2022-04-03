#version 450 core

layout(location=1) in vec3 NRM;

in vec3 FragPos; 
uniform vec4 color;
out vec4 outColor;

//Light calculate
uniform vec3 lightPos;
uniform vec3 viewPos;
vec3 norm = normalize(NRM);
vec3 lightDir = normalize(lightPos - FragPos);  
vec3 lightColor = vec3(1.0, 1.0, 1.0);
float diff = max(dot(norm, lightDir), 0.0);
vec3 viewDir = normalize(viewPos - FragPos);


void main(void)
{
	if(color.r<0) //use normal vector color
	{
		   outColor =  vec4(NRM,1);
	}
	else
	  {
				outColor =  color;
				outColor= diff * outColor ;
	 }
}