#version 430 core

#define HEMISPHERE 0
#define TORUS 1

layout(location=0) in vec4 hemCoords;
layout(location=1) in vec4 torCoords;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform uint object;
uniform vec4 clipPlane;

float gl_ClipDistance[1];

vec4 coords;
vec4 colors;

void main(void)
{
   if (object == HEMISPHERE) coords = hemCoords;
   if (object == TORUS) coords = torCoords;
   
   gl_Position = projMat * modelViewMat * coords;

   gl_ClipDistance[0] = dot(clipPlane, modelViewMat * coords);
}
