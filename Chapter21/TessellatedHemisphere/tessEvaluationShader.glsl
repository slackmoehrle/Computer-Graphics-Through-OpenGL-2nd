#version 430 core

#define PI 3.14159265

layout(quads, equal_spacing, ccw) in;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform float hemRadius;

float u, v;
vec4 hemCoords;

void main( )
{
   u = gl_TessCoord.x;
   v = gl_TessCoord.y;
   
   hemCoords = vec4(hemRadius * cos(u * PI/2.0) * cos(2.0 * v * PI),
                    hemRadius * sin(u * PI/2.0),
					hemRadius * cos(u * PI/2.0) * sin(2.0 * v * PI),
					1.0);
					
   gl_Position = projMat * modelViewMat *  hemCoords;
}
