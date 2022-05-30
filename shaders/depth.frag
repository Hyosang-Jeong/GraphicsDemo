#version 450 core

layout(location=1) in vec3 NRM;
in vec2 TexCoord;

in vec3 FragPos; 
in vec4 shadowpos;
uniform vec4 color;
out vec4 outColor;

//Light calculate
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int mode;
uniform sampler2D shadowMap;
uniform int shadow_behind;


vec3 norm = normalize(NRM);
vec3 lightDir = normalize(lightPos - FragPos);  
vec3 lightColor = vec3(1.0, 0.5, 0.1);
float diff = max(dot(norm, lightDir), 0.0);
vec3 viewDir = normalize(viewPos - FragPos);

float ShadowCalculation(vec4 fragPosLightSpace) 
{
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; 

        projCoords = projCoords * 0.5 + 0.5;     

        float closestDepth = texture(shadowMap, projCoords.xy).r; 
        float currentDepth = projCoords.z; // check whether current frag pos is in shadow 
        float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
        float shadow = currentDepth > closestDepth ? 1.0 : 0.0; 

      if(projCoords.z <0.0)
      {
          shadow =1;
       }
        if(shadow_behind==1)
       {
             if(projCoords.z >1.0)
              {
                  shadow = 0;
              }
        }
        return shadow;
}


void main()
{             
    if(mode == 0) //basic scene
    {
            float shadow =ShadowCalculation(shadowpos);

            outColor =  color * vec4(lightColor,1.0);
			 outColor= vec4(diff *outColor);
            outColor = vec4(vec3(outColor.xyz)*(1-shadow),1);


    }
    else
    { 
            outColor = color;
            gl_FragDepth = gl_FragCoord.z;
    }

}  