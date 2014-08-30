#version 430 core

layout(location=0) in vec4 torCoords;
layout(location=1) in vec2 torTexCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;

out vec2 texCoordsExport;

void main(void)
{  
   gl_Position = projMat * modelViewMat * torCoords;
   texCoordsExport = torTexCoords;
}
