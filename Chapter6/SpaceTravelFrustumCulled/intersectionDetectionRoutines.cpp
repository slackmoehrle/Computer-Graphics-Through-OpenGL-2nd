///////////////////////////////////////////////////////////////////////////////////////////////     
// intersectionDetectionRoutines.cpp
//
// Routines are written to check for intersection between two co-planar straight line segments,
// between two coplanar quadrilaterals, and a coplanar disc and axis-aligned rectangle. 
// Required sub-routines are written as well.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////////////////     

// Return determinant of a 2x2 matrix with elements input in row-major order.
float det2(float a11, float a12, float a21, float a22)
{
   return a11*a22 - a12*a21;
}

// Return determinant of a 3x3 matrix with elements input in row-major order.
float det3(float a11, float a12, float a13, float a21, float a22, 
					 float a23, float a31, float a32, float a33)
{
   return a11*a22*a33 - a11*a23*a32 + a12*a23*a31 - a12*a21*a33 + a13*a21*a32 - a13*a22*a31;
}

// Given three collinear points (x1,y1) and (x2,y2) and (x3,y3) return 0 if (x3,y3) lies
// in the segment joining (x1,y1) and (x2,y2), -1 if it lies on one side, and 1 if on the other.
int checkPointWRTSegment(float x1, float y1, float x2, float y2, float x3, float y3)
{
   if (x1 < x2)
   {
      if (x3 < x1) return -1;
	  else if (x3 > x2) return 1;
	  else return 0;
   }
   else if (x2 < x1)
   {
      if (x3 < x2) return -1;
	  else if (x3 > x1) return 1;
	  else return 0;
   }
   else // x1 = x2
   {
	  if (y1 < y2)
	  {
         if (y3 < y1) return -1;
	     else if (y3 > y2) return 1;
	     else return 0;
	  }
	  else // x1 = x2 & y2 < = y1
	  {
         if (y3 < y2) return -1;
	     else if (y3 > y1) return 1;
	     else return 0;
	  }
   }
}

// Return 1 if the segment joining (x1,y1) and (x2,y2) intersects the 
// segment joining (x3,y3) and (x4,y4), otherwise return 0.
int checkSegmentsIntersection(float x1, float y1, float x2, float y2, 
							 float x3, float y3, float x4, float y4)
{
   float denom, p, q;	 
   denom = det2(x2 - x1, x3 - x4, y2 - y1, y3 - y4);

   if (denom != 0) 
   // The straight lines through (x1,y1) and (x2,y2) and through (x3,y3) and (x4,y4) 
   // intersect uniquely at (1-p)(x1,y1) + p(x2,y2) = (1-q)(x3,y3) + q(x4,y4), which
   // is a point of both segments if both p and q are between 0 and 1.
   {
      p = det2(x3 - x1, x3 - x4, y3 - y1, y3 - y4) / denom;
      q = det2(x2 - x1, x3 - x1, y2 - y1, y3 - y1) / denom;
	  if ( (p >= 0) && (p <= 1) && (q >= 0) && (q <= 1) ) return 1;
	  else return 0;
   }

   else if ( det2(x3 - x2, x3 - x1, y3 - y2, y3 - y1) != 0 ) 
   // The straight lines through (x1,y1) and (x2,y2) and through (x3,y3) and (x4,y4) 
   // do no intersect uniquely, and (x1,y1), (x2,y2) and (x3,y3) are not collinear,
   // in which case the segments do not intersect.	   
      return 0; 

   else
   // All four points are collinear, in which case they do not intersect if 
   // (x3,y3) and (x4,y4) both lie on the same side of segment joining (x1,y1) and (x2,y2). 
   {
	  if ( 
		    (  (checkPointWRTSegment(x1, y1, x2, y2, x3, y3) == 1) && 
		       (checkPointWRTSegment(x1, y1, x2, y2, x4, y4) == 1)
		    )
		    ||
            (  (checkPointWRTSegment(x1, y1, x2, y2, x3, y3) == -1) && 
		       (checkPointWRTSegment(x1, y1, x2, y2, x4, y4) == -1)
		    )
		 )
         return 0;
	  else return 1;
   }
}

// Return 1 if the point (x5,y5) lies in the quadrilateral with vertices at (x1,y1), (x2,y2), (x3,y3) 
// and (x4,y4), otherwise return 0.
int checkPointInQuadrilateral(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
							   float x5, float y5)
{
   // Point (x5,y5) lies in the quadrilateral with vertices at (x1,y1), (x2,y2), (x3,y3) and (x4,y4)
   // if the orders (xi,yi,1), (x(i+1),y(i+1),1), (x5,y5) all appear clockwise or all counter-clockwise.
   if (
		 (  (det3(x1, y1, 1.0, x2, y2, 1.0, x5, y5, 1.0) >= 0) &&
	        (det3(x2, y2, 1.0, x3, y3, 1.0, x5, y5, 1.0) >= 0) &&
		    (det3(x3, y3, 1.0, x4, y4, 1.0, x5, y5, 1.0) >= 0) &&
		    (det3(x4, y4, 1.0, x1, y1, 1.0, x5, y5, 1.0) >= 0) 
         )
	     ||
	     (  (det3(x1, y1, 1.0, x2, y2, 1.0, x5, y5, 1.0) <= 0) &&
	        (det3(x2, y2, 1.0, x3, y3, 1.0, x5, y5, 1.0) <= 0) &&
    	    (det3(x3, y3, 1.0, x4, y4, 1.0, x5, y5, 1.0) <= 0) &&
 		    (det3(x4, y4, 1.0, x1, y1, 1.0, x5, y5, 1.0) <= 0)
         )
      )
	  return 1;
   else  return 0;
}

// Return 1 if the quadrilateral with  vertices (x1,y1), (x2,y2), (x3,y3) and (x4,y4) 
// intersects the quadrilateral with vertices at (x5,y5), (x6,y6), (x7,y7) and (x8,y8) 
// (both assumed not self-intersecting), otherwise return 0.
int checkQuadrilateralsIntersection(float x1, float y1, float x2, float y2, 
								    float x3, float y3, float x4, float y4,
								    float x5, float y5, float x6, float y6, 
								    float x7, float y7, float x8, float y8)
{
   // The boundaries of the two quadrilaterals intersect if one of the 16 pairs of sides,
   // one from either quadrilateral, is intersecting.
   if ( checkSegmentsIntersection(x1, y1, x2, y2, x5, y5, x6, y6) ||
	    checkSegmentsIntersection(x1, y1, x2, y2, x6, y6, x7, y7) ||
		checkSegmentsIntersection(x1, y1, x2, y2, x7, y7, x8, y8) ||
	    checkSegmentsIntersection(x1, y1, x2, y2, x8, y8, x5, y5) ||
        checkSegmentsIntersection(x2, y2, x3, y3, x5, y5, x6, y6) ||
	    checkSegmentsIntersection(x2, y2, x3, y3, x6, y6, x7, y7) ||
		checkSegmentsIntersection(x2, y2, x3, y3, x7, y7, x8, y8) ||
	    checkSegmentsIntersection(x2, y2, x3, y3, x8, y8, x5, y5) ||
		checkSegmentsIntersection(x3, y3, x4, y4, x5, y5, x6, y6) ||
	    checkSegmentsIntersection(x3, y3, x4, y4, x6, y6, x7, y7) ||
		checkSegmentsIntersection(x3, y3, x4, y4, x7, y7, x8, y8) ||
	    checkSegmentsIntersection(x3, y3, x4, y4, x8, y8, x5, y5) ||
		checkSegmentsIntersection(x4, y4, x1, y1, x5, y5, x6, y6) ||
	    checkSegmentsIntersection(x4, y4, x1, y1, x6, y6, x7, y7) ||
		checkSegmentsIntersection(x4, y4, x1, y1, x7, y7, x8, y8) ||
	    checkSegmentsIntersection(x4, y4, x1, y1, x8, y8, x5, y5) 
	  )
	   return 1;

   // If the boundaries do not intersect then the quadrilaterals intersect when one
   // lies entirely within the other, which is checked by examining if the vertex (x5,y5)
   // lies in the first quadrilateral, which would imply (given that the boundaries don't
   // intersect) that the second quadrilateral lies entirely within the first, or if the  
   // vertex (x1,y1) lies in the second quadrilateral, which would imply that the first 
   // quadrilateral lies entirely in the second.
   else if ( checkPointInQuadrilateral(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5) ) return 1;
   else if ( checkPointInQuadrilateral(x5, y5, x6, y6, x7, y7, x8, y8, x1, y1) ) return 1;

   else return 0;
}

// Return 1 if the axes-parallel rectangle with diagonally opposite corners at (x1,y1) and (x2,y2)
// intersects the disc centered (x3,y3) of radius r, otherwise return 0.
int checkDiscRectangleIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float r)
{
   float minX, maxX, minY, maxY;

   // Set minX to smaller of x1 and x2, and maxX to the larger; likewise minY and maxY.
   if (x1 <= x2) 
   {
      minX = x1; maxX = x2;
   }
   else
   {
      minX = x2; maxX = x1;
   }
   if (y1 <= y2) 
   {
      minY = y1; maxY = y2;
   }
   else
   {
      minY = y2; maxY = y1;
   }

   // The disc intersects the rectangle if its center lies in the strip with corners at
   // (minX-r, minY) and (maxX+r, maxY), or if its center lies in the strip with corners
   // at (minX, minY-r) and (maxX, maxY+r), or if its center is within distance r of one
   // the four corners of the rectangles.
   if      ( (x3 >= minX - r) && (x3 <= maxX + r) && (y3 >= minY) && (y3 <= maxY) ) return 1;
   else if ( (x3 >= minX) && (x3 <= maxX) && (y3 >= minY - r) && (y3 <= maxY + r) ) return 1;
   else if ( (x3 - x1)*(x3-x1) + (y3 - y1)*(y3 - y1) <= r*r ) return 1;
   else if ( (x3 - x1)*(x3-x1) + (y3 - y2)*(y3 - y2) <= r*r ) return 1;
   else if ( (x3 - x2)*(x3-x2) + (y3 - y2)*(y3 - y2) <= r*r ) return 1;
   else if ( (x3 - x2)*(x3-x2) + (y3 - y1)*(y3 - y1) <= r*r ) return 1;
   else return 0;
}
