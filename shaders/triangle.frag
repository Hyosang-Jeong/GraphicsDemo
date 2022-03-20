#version 450 core

layout(location=0) in vec3 vClrCoord;
layout(location=1) in vec3 NRM;

uniform vec4 color;
uniform sampler2D tex;
in vec2 UV;

out vec4 outColor;

void main(void)
{
    if (color.r < 0)
    {
        if (color.a < 0)
            outColor = vec4(texture(tex, UV).rgb, 1.0); /*  Use texture for color */
        else
            outColor = vec4(NRM, 1.0);              /*  Use normal for color */
    }
    else
        outColor = vec4(vClrCoord.x,vClrCoord.y ,vClrCoord.z,0.5f);   /* Use obj's color as usual */
}