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

#include "hemisphere.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillHemVertexArray(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)])
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= HEM_LATS; j++)
      for (i = 0; i <= HEM_LONGS; i++)
      {
         hemVertices[k].coords.x = HEM_RADIUS * cos( (float)j/HEM_LATS * PI/2.0 ) * cos( 2.0 * (float)i/HEM_LONGS * PI );
         hemVertices[k].coords.y = HEM_RADIUS * sin( (float)j/HEM_LATS * PI/2.0 );
         hemVertices[k].coords.z = HEM_RADIUS * cos( (float)j/HEM_LATS * PI/2.0 ) * sin( 2.0 * (float)i/HEM_LONGS * PI );
		 hemVertices[k].coords.w = 1.0;
		 k++;
	  }
}

// Fill the array of index arrays.
void fillHemIndices(unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)])
{
   int i, j;
   for(j = 0; j < HEM_LATS; j++)
   {
      for (i = 0; i <= HEM_LONGS; i++)
      {
	     hemIndices[j][2*i] = (j+1)*(HEM_LONGS + 1) + i;
	     hemIndices[j][2*i+1] = j*(HEM_LONGS + 1) + i;
      }
   }
}

// Fill the array of counts.
void fillHemCounts(int hemCounts[HEM_LATS])
{
   int j;
   for(j = 0; j < HEM_LATS; j++) hemCounts[j] = 2*(HEM_LONGS + 1);
}

// Fill the array of buffer offsets.
void fillHemOffsets(void* hemOffsets[HEM_LATS])
{
   int j;
   for(j = 0; j < HEM_LATS; j++) hemOffsets[j] = (GLvoid*)(2*(HEM_LONGS+1)*j*sizeof(unsigned int));
}

// Initialize the hemisphere.
void fillHemisphere(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)], 
	         unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)],
			 int hemCounts[HEM_LATS],
			 void* hemOffsets[HEM_LATS])
{
   fillHemVertexArray(hemVertices);
   fillHemIndices(hemIndices);
   fillHemCounts(hemCounts);
   fillHemOffsets(hemOffsets);
}
