#version 430 core

#define HEMISPHERE 0
#define TORUS 1

layout(location=0) in vec4 hemCoords;
layout(location=1) in vec4 torCoords;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform vec4 hemColor, torColor;

out vec4 colorsExport;

subroutine void objectAction(void);
subroutine uniform objectAction object;

vec4 coords;

subroutine (objectAction) void hemisphere(void)
{
   coords = hemCoords;
   colorsExport = hemColor;
}

subroutine (objectAction) void torus(void)
{
   coords = torCoords;
   colorsExport = torColor;
}

void main(void)
{
   object();
   gl_Position = projMat * modelViewMat * coords;
}
