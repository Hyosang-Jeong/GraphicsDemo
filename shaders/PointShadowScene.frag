#version 450 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far_plane;
uniform vec4 color;

float ShadowCalculation(vec3 fragPos)
{
     vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

float shadow  = 0.0;
float bias    = 0.9; 
float samples = 4.0;
float offset  = 0.1;
for(float x = -offset; x < offset; x += offset / (samples * 0.5))
{
    for(float y = -offset; y < offset; y += offset / (samples * 0.5))
    {
        for(float z = -offset; z < offset; z += offset / (samples * 0.5))
        {
            float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; 
            closestDepth *= far_plane;   // undo mapping [0;1]
            if(currentDepth - bias > closestDepth)
                shadow += 1.0;
        }
    }
}
shadow /= (samples * samples * samples);

    return shadow;
}

void main()
{           
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color.xyz;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPos);                      
    vec3 lighting = ((1.0 - shadow) * (diffuse)) * color.xyz;    
    
    FragColor = vec4(lighting, 1.0);

    if(color.r == 1 && color.g == 1 && color.b == 1)
        FragColor = vec4(1,1,1,1);
}  