#version 430 core

#define HEMISPHERE 0
#define TORUS 1
#define NONE 2

layout(std430, binding=0) buffer shaderStorage 
{
   ivec2 clickedCoords; 
   uint clickedObj; 
   float minClickedDepth;
};

layout(origin_upper_left, pixel_center_integer) in  vec4 gl_FragCoord;

uniform uint object;
uniform vec4 hemColor, torColor, highlightColor;
uniform int highlightFrames;
uniform int isSelecting;

out vec4 colorsOut;

void main(void)
{
   if (isSelecting == 1)
   if (     (abs(gl_FragCoord.x - clickedCoords.x) <= 1) 
         && (abs(gl_FragCoord.y - clickedCoords.y) <= 1) 
		 && (gl_FragCoord.z < minClickedDepth)
	  )
   {	  
	  minClickedDepth = gl_FragCoord.z;
      clickedObj = object;
   }
   
   if (object == HEMISPHERE) 
   {
      if ( (clickedObj == HEMISPHERE) && (highlightFrames > 0) ) colorsOut = highlightColor;
      else colorsOut = hemColor; 
   }
   if (object == TORUS)   
   {
      if ( (clickedObj == TORUS) && (highlightFrames > 0) ) colorsOut = highlightColor;
      else colorsOut = torColor; 
   }
}