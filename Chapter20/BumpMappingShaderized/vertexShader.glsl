#version 430 core

layout(location=0) in vec4 planeCoords;
layout(location=1) in vec3 planeNormal;
layout(location=2) in vec3 planeBumpedNormal;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint isBumpMapped;

out vec4 fAndBColsExport;

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
uniform Material planeFandB;

vec3 normal, lightDirection, eyeDirection, halfway;
vec4 fAndBEmit, fAndBGlobAmb, fAndBAmb, fAndBDif, fAndBSpec;

void main(void)
{   
   normal = (isBumpMapped == 1)? planeBumpedNormal : planeNormal;
   normal = normalize(normalMat * normal);
   lightDirection = normalize(vec3(light0.coords));
   eyeDirection = vec3(0.0, 0.0, 1.0);
   halfway = (length(lightDirection + eyeDirection) == 0.0) ? 
             vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);
   
   fAndBEmit = planeFandB.emitCols;
   fAndBGlobAmb = globAmb * planeFandB.ambRefl;
   fAndBAmb = light0.ambCols * planeFandB.ambRefl;
   fAndBDif = max(dot(normal, lightDirection), 0.0) * light0.difCols * planeFandB.difRefl;    
   fAndBSpec = pow(max(dot(normal, halfway), 0.0), planeFandB.shininess) * 
               light0.specCols * planeFandB.specRefl;
   fAndBColsExport =  vec4(vec3(min(fAndBEmit + fAndBGlobAmb + fAndBAmb + 
                           fAndBDif + fAndBSpec, vec4(1.0))), 1.0);  
   
   gl_Position = projMat * modelViewMat * planeCoords;
}
