#ifndef PLANE_H
#define PLANE_H

#include "vertex.h"

#define PLANE_EXTENT 100.0
#define PLANE_LONGS 200 // Number of longitudinal slices.
#define PLANE_LATS 200 // Number of latitudinal slices.

void fillPlaneVertexArray(Vertex planeVertices[(PLANE_LONGS + 1) * (PLANE_LATS + 1)]);
void fillPlaneIndices(unsigned int planeIndices[PLANE_LATS][2*(PLANE_LONGS+1)]);
void fillPlaneCounts(int planeCounts[PLANE_LATS]);
void fillPlaneOffsets(void* planeOffsets[PLANE_LATS]);

void fillPlane(Vertex planeVertices[(PLANE_LONGS + 1) * (PLANE_LATS + 1)], 
	         unsigned int planeIndices[PLANE_LATS][2*(PLANE_LONGS+1)],
			 int planeCounts[PLANE_LATS],
			 void* planeOffsets[PLANE_LATS]);
#endif
