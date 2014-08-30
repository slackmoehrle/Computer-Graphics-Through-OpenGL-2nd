#version 430 core

in vec4 fAndBColsExport;

out vec4 colorsOut;

void main(void)
{
   colorsOut = fAndBColsExport;
}