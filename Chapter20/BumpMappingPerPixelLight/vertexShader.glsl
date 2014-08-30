#version 430 core

layout(location=0) in vec4 planeCoords;
layout(location=1) in vec3 planeNormal;
layout(location=2) in vec3 planeBumpedNormal;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint isBumpMapped;

out vec3 normalExport;

void main(void)
{   
   normalExport = (isBumpMapped == 1)? planeBumpedNormal : planeNormal;
   normalExport = normalize(normalMat * normalExport);
   
   gl_Position = projMat * modelViewMat * planeCoords;
}
