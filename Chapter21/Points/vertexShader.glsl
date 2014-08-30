#version 430 core

layout(location=0) in vec4 squareCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform float zTrans;

void main(void)
{
   gl_PointSize = 100.0 - zTrans * 10.0;
   gl_Position = projMat * modelViewMat * squareCoords;
}