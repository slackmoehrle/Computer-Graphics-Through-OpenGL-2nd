#version 430 core

#define FIELD 0
#define SKY 1
#define LINE 2

in vec4 fAndBColsExport;
in vec2 texCoordsExport;

uniform sampler2D grassTex;
uniform sampler2D skyTex;
uniform sampler2D nightSkyTex;
uniform uint object;
uniform float alpha;

out vec4 colorsOut;

vec4 fieldTexColor, skyTexColor, nightSkyTexColor;

void main(void)
{  
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   nightSkyTexColor = texture(nightSkyTex, texCoordsExport);
   
   if (object == FIELD) colorsOut = fieldTexColor * fAndBColsExport;
   if (object == SKY) colorsOut = mix(nightSkyTexColor, skyTexColor, alpha);
   if (object == LINE) colorsOut = vec4(1.0, 1.0, 1.0, 1.0);
}