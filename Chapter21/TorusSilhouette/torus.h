#ifndef TORUS_H
#define TORUS_H

#include "vertex.h"

#define PI 3.14159265
#define TOR_OUTRAD 12.0 // Torus outer radius.
#define TOR_INRAD 2.0 // Torus inner radius.
#define TOR_LONGS 20 // Number of longitudinal slices.
#define TOR_LATS 20 // Number of latitudinal slices.
#define TOR_COLORS 0.0, 1.0, 0.0, 1.0 // Torus colors.

void fillTorVertexArray(Vertex torVertices[(TOR_LONGS + 1) * (TOR_LATS + 1)]);
void fillTorIndices(unsigned int torIndices[TOR_LATS][4*(TOR_LONGS+1)]);
void fillTorCounts(int torCounts[TOR_LATS]);
void fillTorOffsets(void* torOffsets[TOR_LATS]);

void fillTorus(Vertex torVertices[(TOR_LONGS + 1) * (TOR_LATS + 1)], 
	         unsigned int torIndices[TOR_LATS][4*(TOR_LONGS+1)],
			 int torCounts[TOR_LATS],
			 void* torOffsets[TOR_LATS]);
#endif
