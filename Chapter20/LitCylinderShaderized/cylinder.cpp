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

#include "cylinder.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillCylVertexArray(Vertex cylVertices[(CYL_LONGS + 1) * (CYL_LATS + 1)])
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= CYL_LATS; j++)
      for (i = 0; i <= CYL_LONGS; i++)
      {
         cylVertices[k].coords.x = cos( (-1 + 2*(float)i/CYL_LONGS) * PI );
         cylVertices[k].coords.y = sin( (-1 + 2*(float)i/CYL_LONGS) * PI );
         cylVertices[k].coords.z = -1 + 2*(float)j/CYL_LATS;
		 cylVertices[k].coords.w = 1.0;
		 cylVertices[k].normal.x = cos( (-1 + 2*(float)i/CYL_LONGS) * PI );
         cylVertices[k].normal.y = sin( (-1 + 2*(float)i/CYL_LONGS) * PI );
		 cylVertices[k].normal.z = 0.0;
		 k++;
	  }
}

void fillCylIndices(unsigned int cylIndices[CYL_LATS][2*(CYL_LONGS+1)])
{
   int i, j;
   for(j = 0; j < CYL_LATS; j++)
   {
      for (i = 0; i <= CYL_LONGS; i++)
      {
	     cylIndices[j][2*i] = (j+1)*(CYL_LONGS + 1) + i;
	     cylIndices[j][2*i+1] = j*(CYL_LONGS + 1) + i;
      }
   }
}

// Fill the array cylCounts.
void fillCylCounts(int cylCounts[CYL_LATS])
{
   int j;
   for(j = 0; j < CYL_LATS; j++) cylCounts[j] = 2*(CYL_LONGS + 1);
}

// Fill the array cylOffsets.
void fillCylOffsets(void* cylOffsets[CYL_LATS])
{
   int j;
   for(j = 0; j < CYL_LATS; j++) cylOffsets[j] = (GLvoid*)(2*(CYL_LONGS+1)*j*sizeof(unsigned int));
}

void fillCylinder(Vertex cylVertices[(CYL_LONGS + 1) * (CYL_LATS + 1)], 
	         unsigned int cylIndices[CYL_LATS][2*(CYL_LONGS+1)],
			 int cylCounts[CYL_LATS],
			 void* cylOffsets[CYL_LATS])
{
   fillCylVertexArray(cylVertices);
   fillCylIndices(cylIndices);
   fillCylCounts(cylCounts);
   fillCylOffsets(cylOffsets);
}