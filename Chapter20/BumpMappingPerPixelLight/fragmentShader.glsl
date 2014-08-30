#version 430 core

in vec3 normalExport;

out vec4 colorsOut;

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
   normal = normalize(normalExport);
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
   colorsOut =  vec4(vec3(min(fAndBEmit + fAndBGlobAmb + fAndBAmb + 
                fAndBDif + fAndBSpec, vec4(1.0))), 1.0);  
}