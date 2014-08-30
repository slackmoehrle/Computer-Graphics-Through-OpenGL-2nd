#version 430 core

#define HEMISPHERE 0
#define TORUS 1
#define CENTER 2

uniform uint object;
uniform samplerBuffer transformFeedbackTex;
uniform vec4 hemColor, torColor, orangeColor, redColor;

out vec4 colorsOut;

vec3 center0, center1;
float distBetweenCenters;

void main(void)
{
   center0 = texelFetch(transformFeedbackTex, 0).xyz;
   center1 = texelFetch(transformFeedbackTex, 1).xyz;    
   distBetweenCenters = distance(center0 , center1);
   
   if (object == HEMISPHERE) 
   {
      colorsOut = hemColor;
	  if (distBetweenCenters <= 8.0) colorsOut = orangeColor; 
      if (distBetweenCenters <= 4.0) colorsOut = redColor;
   }	  
   if (object == TORUS) colorsOut = torColor;
}