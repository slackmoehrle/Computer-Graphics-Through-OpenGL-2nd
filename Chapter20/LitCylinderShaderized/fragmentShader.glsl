#version 430 core

in vec4 frontColsExport, backColsExport;

out vec4 colorsOut;

void main(void)
{
   colorsOut = gl_FrontFacing? frontColsExport : backColsExport;
}