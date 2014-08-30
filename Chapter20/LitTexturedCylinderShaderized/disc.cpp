#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include "disc.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillDiscVertexArray(Vertex discVertices[DISC_SEGS])
{
   int k;

   for (k = 0; k < DISC_SEGS; k++)
   {
      discVertices[k].coords.x = cos( (-1 + 2*(float)k/DISC_SEGS) * PI );
      discVertices[k].coords.y = sin( (-1 + 2*(float)k/DISC_SEGS) * PI );
      discVertices[k].coords.z = 1.0;
      discVertices[k].coords.w = 1.0;
      discVertices[k].normal.x = 0.0;
      discVertices[k].normal.y = 0.0;
      discVertices[k].normal.z = 1.0;
	  discVertices[k].texCoords.s = 0.5 + 0.5*cos( (-1 + 2*(float)k/DISC_SEGS) * PI );
	  discVertices[k].texCoords.t = 0.5 + 0.5*sin( (-1 + 2*(float)k/DISC_SEGS) * PI );
   }
}


