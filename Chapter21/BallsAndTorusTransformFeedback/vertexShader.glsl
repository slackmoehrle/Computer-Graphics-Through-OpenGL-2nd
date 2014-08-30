#version 430 core

#define HEMISPHERE 0
#define TORUS 1
#define CENTER 2

layout(location=0) in vec4 hemCoords;
layout(location=1) in vec4 torCoords;
layout(location=2) in vec4 centerCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

out vec4 centerWorldCoords;

vec4 coords;

void main(void)
{
   if (object == HEMISPHERE) coords = hemCoords;
   if (object == TORUS) coords = torCoords;
   if (object == CENTER)
   {
      coords = centerCoords;
	  centerWorldCoords = modelViewMat * coords;
   }
   
   gl_Position = projMat * modelViewMat * coords;
}
