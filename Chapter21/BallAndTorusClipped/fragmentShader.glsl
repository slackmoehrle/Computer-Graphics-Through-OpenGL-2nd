#version 430 core

#define HEMISPHERE 0
#define TORUS 1

uniform uint object;
uniform vec4 hemColor, torColor;

out vec4 colorsOut;

void main(void)
{
   if (object == HEMISPHERE) colorsOut = hemColor; 
   if (object == TORUS) colorsOut = torColor;
}