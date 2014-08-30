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

#include "plane.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillPlaneVertexArray(Vertex planeVertices[(PLANE_LONGS + 1) * (PLANE_LATS + 1)])
{
   int i, j, k;
   float u, v;

   k = 0;
   for (j = 0; j <= PLANE_LATS; j++)
      for (i = 0; i <= PLANE_LONGS; i++)
      {
		 u = (-1 + 2*(float)j/PLANE_LATS) * PLANE_EXTENT;
		 v = (-1 + 2*(float)i/PLANE_LONGS) * PLANE_EXTENT;
		 
         planeVertices[k].coords.x = u;
         planeVertices[k].coords.y = 0.0;
         planeVertices[k].coords.z = v;
		 planeVertices[k].coords.w = 1.0;
		 planeVertices[k].normal.x = 0.0;
		 planeVertices[k].normal.y = 1.0;
		 planeVertices[k].normal.z = 0.0;
		 planeVertices[k].bumpedNormal.x = 2.0*cos(2.0*u);
		 planeVertices[k].bumpedNormal.y = 1.0;
		 planeVertices[k].bumpedNormal.z = 0.0;
		 k++;
	  }
}

void fillPlaneIndices(unsigned int planeIndices[PLANE_LATS][2*(PLANE_LONGS+1)])
{
   int i, j;
   for(j = 0; j < PLANE_LATS; j++)
   {
      for (i = 0; i <= PLANE_LONGS; i++)
      {
	     planeIndices[j][2*i] = (j+1)*(PLANE_LONGS + 1) + i;
	     planeIndices[j][2*i+1] = j*(PLANE_LONGS + 1) + i;
      }
   }
}

// Fill the array planeCounts.
void fillPlaneCounts(int planeCounts[PLANE_LATS])
{
   int j;
   for(j = 0; j < PLANE_LATS; j++) planeCounts[j] = 2*(PLANE_LONGS + 1);
}

// Fill the array planeOffsets.
void fillPlaneOffsets(void* planeOffsets[PLANE_LATS])
{
   int j;
   for(j = 0; j < PLANE_LATS; j++) planeOffsets[j] = (GLvoid*)(2*(PLANE_LONGS+1)*j*sizeof(unsigned int));
}

void fillPlane(Vertex planeVertices[(PLANE_LONGS + 1) * (PLANE_LATS + 1)], 
	         unsigned int planeIndices[PLANE_LATS][2*(PLANE_LONGS+1)],
			 int planeCounts[PLANE_LATS],
			 void* planeOffsets[PLANE_LATS])
{
   fillPlaneVertexArray(planeVertices);
   fillPlaneIndices(planeIndices);
   fillPlaneCounts(planeCounts);
   fillPlaneOffsets(planeOffsets);
}