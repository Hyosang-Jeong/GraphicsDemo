#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec3 color;

void main () {

  fFragClr = vec4(color, 1.0);
}
