#version 450 core

layout(location=1) in vec3 NRM;

uniform vec4 color;
uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 u_ambient = vec3(1, 1,1);
uniform vec3 viewPos;
uniform float FogMax;
uniform float FogMin;
uniform float layer_first;
uniform float layer_second;
uniform float layer_third;

in vec2 UV;
in vec3 FragPos; 
out vec4 outColor;

vec3 norm = normalize(NRM);
vec3 lightDir = normalize(lightPos - FragPos);  
vec3 lightColor = vec3(1.0, 1.0, 1.0);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

float specularStrength = 0.5;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);  

float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;  

void main(void)
{
    vec3 fogcolor = vec3(0,0,0);
    float dist = abs(viewPos.z - FragPos.z);
    float fogFactor = (FogMax- dist) /(FogMax -FogMin );
    fogFactor = clamp( fogFactor, 0.0, 1.0 );

    outColor =  color;
    if (color.r < 0)
    {
        if (color.a < 0)
            outColor = vec4(texture(tex, UV).rgb, 1.0); /*  Use texture for color */
        else
            outColor = vec4(NRM, 1.0);              /*  Use normal for color */
            vec3 color_tmp = vec3(outColor.x, outColor.y, outColor.z);
           outColor = vec4(mix(fogcolor, color_tmp,fogFactor),1);
    }
    else
        {
         if (diff > layer_first)
               diff= 1;
         else if (diff > layer_second)
               diff= 0.8;
          else if (diff > layer_third)
               diff= 0.7 ;
        else
               diff= 0.3 ;
         
             outColor= diff * outColor ;

             if(color.x ==1 && color.y == 0.83 && color.z ==0)
             { 
                outColor = vec4(1,0.83,0,1);
             }

             vec3 color_tmp = vec3(outColor.x, outColor.y, outColor.z);

             outColor = vec4(mix(fogcolor, color_tmp,fogFactor),1);

        }

}