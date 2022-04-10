#version 450 core

out vec4 outColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 color;


void main()
{
   outColor =  color;
}