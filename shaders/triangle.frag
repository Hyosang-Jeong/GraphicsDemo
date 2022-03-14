#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec3 color;
uniform vec2 translation;
void main () {

  fFragClr = vec4(color, 1.0);
}
