#version 430 core

layout(location=0) in vec4 inputPointSetCoords;

void main(void)
{
   gl_Position = inputPointSetCoords;
}
