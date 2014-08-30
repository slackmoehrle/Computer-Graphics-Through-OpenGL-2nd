#version 430 core

layout(location=0) in vec4 helCoords;
layout(location=1) in vec4 helColors;
layout(location=2) in mat4 helTransformMats; // Because of type mat4 it will 
                                             // occupy locations 2, 3, 4, 5.

uniform mat4 projMat;

out vec4 colorsExport;

void main(void)
{   
   gl_Position = projMat * helTransformMats * helCoords;
   
   colorsExport = helColors;
}
