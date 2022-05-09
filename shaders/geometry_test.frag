#version 450 core

//in vec4 gColor;

//out vec4 outColor;

layout(location=1) in vec3 normalVec;

in vec3 FragPosition;
in vec4 gColor;
//uniform vec4 color;
out vec4 outColor;

//Light calculate
uniform vec3 lightPos;
uniform vec3 viewPos;
vec3 norm = normalize(normalVec);
vec3 lightDir = normalize(lightPos - FragPosition);  
vec3 lightColor = vec3(1.0, 0.5, 0.1);
float diff = max(dot(norm, lightDir), 0.0);
vec3 viewDir = normalize(viewPos - FragPosition);

void main()
{
    outColor = gColor * vec4(lightColor,1.0);
    outColor = diff * outColor ;
}