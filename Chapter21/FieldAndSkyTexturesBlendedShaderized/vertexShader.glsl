#version 430 core

#define FIELD 0
#define SKY 1
#define LINE 2

layout(location=0) in vec4 fieldCoords;
layout(location=1) in vec3 fieldNormal;
layout(location=2) in vec2 fieldTexCoords;
layout(location=3) in vec4 skyCoords;
layout(location=4) in vec2 skyTexCoords;
layout(location=5) in vec4 lineCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint object;

out vec4 fAndBColsExport;
out vec2 texCoordsExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;
  
struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
uniform Material matFandB;

vec3 normal, lightDirection, eyeDirection, halfway;
vec4 fAndBEmit, fAndBGlobAmb, fAndBAmb, fAndBDif, fAndBSpec;
vec4 coords;

void main(void)
{   
   if (object == FIELD)
   {
      coords = fieldCoords;
      normal = fieldNormal;
      texCoordsExport = fieldTexCoords;
	  
      normal = normalize(normalMat * normal);
      lightDirection = normalize(vec3(light0.coords));
      eyeDirection = vec3(0.0, 0.0, 1.0);
      halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);
   
      fAndBEmit = matFandB.emitCols;
      fAndBGlobAmb = globAmb * matFandB.ambRefl;
      fAndBAmb = light0.ambCols * matFandB.ambRefl;
      fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * matFandB.difRefl);    
      fAndBSpec = pow(max(dot(normal, halfway), 0.0f), matFandB.shininess) * (light0.specCols * matFandB.specRefl);
      fAndBColsExport =  vec4(vec3(min(fAndBEmit + fAndBGlobAmb + fAndBAmb + fAndBDif + fAndBSpec, vec4(1.0))), 1.0);  
      }
   if (object == SKY)
   {
      coords = skyCoords;
      texCoordsExport = skyTexCoords;
   }
   if (object == LINE)
   {
      coords = lineCoords;
   }
   
   gl_Position = projMat * modelViewMat * coords;
}
