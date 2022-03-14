#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec3 vVertexClrCoord;

layout (location=0) out vec3 vClrCoord;


uniform float rotation;
uniform vec2 translation;

void main() {
  gl_Position = vec4(vVertexPosition.x + translation.x,vVertexPosition.y + translation.y, 0.0, 1.0);

  vClrCoord = vVertexClrCoord;
}
