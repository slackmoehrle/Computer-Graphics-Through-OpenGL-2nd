#version 430 core

layout(location=0) in vec4 cylCoords;
layout(location=1) in vec3 cylNormal;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;

out vec4 frontColsExport, backColsExport;

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
uniform Material cylFront, cylBack;

vec3 normal, lightDirection, eyeDirection, halfway;
vec4 frontEmit, frontGlobAmb, frontAmb, frontDif, frontSpec,
     backEmit, backGlobAmb, backAmb, backDif, backSpec;
   
void main(void)
{
   normal = cylNormal;

   normal = normalize(normalMat * normal);
   lightDirection = normalize(vec3(light0.coords));
   eyeDirection = -1.0 * normalize(vec3(modelViewMat * cylCoords));
   halfway = (length(lightDirection + eyeDirection) == 0.0) ? 
             vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);
 
   frontEmit = cylFront.emitCols;
   frontGlobAmb = globAmb * cylFront.ambRefl;
   frontAmb = light0.ambCols * cylFront.ambRefl;
   frontDif = max(dot(normal, lightDirection), 0.0) * light0.difCols * cylFront.difRefl;    
   frontSpec = pow(max(dot(normal, halfway), 0.0), cylFront.shininess) * 
               light0.specCols * cylFront.specRefl;
   frontColsExport =  vec4(vec3(min(frontEmit + frontGlobAmb + frontAmb + 
                           frontDif + frontSpec, vec4(1.0))), 1.0);  
   
   normal = -1.0 * normal;
   backEmit = cylBack.emitCols;
   backGlobAmb = globAmb * cylBack.ambRefl;    
   backAmb = light0.ambCols * cylBack.ambRefl;
   backDif = max(dot(normal, lightDirection), 0.0) * light0.difCols * cylBack.difRefl;    
   backSpec = pow(max(dot(normal, halfway), 0.0), cylBack.shininess) * 
              light0.specCols * cylBack.specRefl;
   backColsExport =  vec4(vec3(min(backEmit + backGlobAmb + backAmb + 
                          backDif + backSpec, vec4(1.0))), 1.0);  
  
   gl_Position = projMat * modelViewMat * cylCoords;
}
