#version 430 core

#define CYLINDER 0
#define DISC 1

in vec4 frontAmbDiffExport, frontSpecExport, backAmbDiffExport, backSpecExport;
in vec2 texCoordsExport;

uniform sampler2D canLabelTex;
uniform sampler2D canTopTex;
uniform uint object;

out vec4 colorsOut;

vec4 texColor;

void main(void)
{  
   if (object == CYLINDER) texColor = texture(canLabelTex, texCoordsExport);
   if (object == DISC) texColor = texture(canTopTex, texCoordsExport);

   colorsOut = gl_FrontFacing? (frontAmbDiffExport * texColor + frontSpecExport) : 
                               (backAmbDiffExport * texColor + backSpecExport);
}