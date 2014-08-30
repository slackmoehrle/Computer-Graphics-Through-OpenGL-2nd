#version 430 core

layout(location=0) in vec4 helCoords;

uniform mat4 projMat;
uniform samplerBuffer helColorsTex;
uniform samplerBuffer helTransformMatsTex;

out vec4 colorsExport;

vec4 helColors;
vec4 col0, col1, col2, col3;
mat4 helTransformMats;

void main(void)
{
   helColors = texelFetch(helColorsTex, gl_InstanceID);
   
   col0 = texelFetch(helTransformMatsTex, gl_InstanceID * 4);
   col1 = texelFetch(helTransformMatsTex, gl_InstanceID * 4 + 1);
   col2 = texelFetch(helTransformMatsTex, gl_InstanceID * 4 + 2);
   col3 = texelFetch(helTransformMatsTex, gl_InstanceID * 4 + 3);
   helTransformMats = mat4(col0, col1, col2, col3);
   
   gl_Position = projMat * helTransformMats * helCoords;
   
   colorsExport = helColors;
}
