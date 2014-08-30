#version 430 core

#define FIELD 0
#define SKY 1

in vec2 texCoordsExport;

uniform sampler2D grassTex;
uniform sampler2D skyTex;
uniform uint object;

out vec4 colorsOut;

vec4 fieldTexColor, skyTexColor;

void main(void)
{  
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   
   if (object == FIELD) colorsOut = fieldTexColor;
   if (object == SKY) colorsOut = skyTexColor;
}