//////////////////////////////////////////////////////////////////////////////////////         
// spaceTravelFrustumCulled.cpp
//
// This program is based on spaceTravel.cpp with an added frustum culling option.
// It draws a conical spacecraft that can travel and an array of fixed spherical 
// asteroids. The view in the left viewport is from a fixed camera; the view in 
// the right viewport is from the spacecraft.There is approximate collision detection.  
// Frustum culling is implemented by means of a quadtree data structure.
// 
// COMPILE NOTE: File intersectionDetectionRoutines.cpp must be in the same folder.
// EXECUTION NOTE: If ROWS and COLUMNS are large the quadtree takes time to build so
//                 the display may take several seconds to come up.
//
// User-defined constants: 
// ROWS is the number of rows of  asteroids.
// COLUMNS is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
// Press space to toggle between frustum culling enabled and disabled.
// 
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <cmath>
#include <list>
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

#define PI 3.14159265

using namespace std;

#define ROWS 100  // Number of rows of asteroids.
#define COLUMNS 100 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 
                             // filled with an asteroid. It should be an integer between 0 and 100.

// Globals.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.
static int isFrustumCulled = 0;
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.

#include "intersectionDetectionRoutines.cpp"

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

// Asteroid class.
class Asteroid
{
public:
   Asteroid();
   Asteroid(float x, float y, float z, float r, unsigned char valueR, 
	    	unsigned char valueG, unsigned char valueB);
   float getCenterX() { return centerX; }
   float getCenterY() { return centerY; }
   float getCenterZ() { return centerZ; }
   float getRadius()  { return radius; }
   void draw();

private:
   float centerX, centerY, centerZ, radius;
   unsigned char color[3];
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
   centerX = 0.0;
   centerY = 0.0;
   centerZ = 0.0; 
   radius = 0.0; // Indicates no asteroid exists in the position.
   color[0] = 0;
   color[1] = 0;
   color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char valueR, 
		unsigned char valueG, unsigned char valueB)
{
   centerX = x;
   centerY = y;
   centerZ = z; 
   radius = r;
   color[0] = valueR;
   color[1] = valueG;
   color[2] = valueB;
}
	
// Function to draw asteroid.
void Asteroid::draw()
{
   if (radius > 0.0) // If asteroid exists.
   {
      glPushMatrix();
      glTranslatef(centerX, centerY, centerZ);
      glColor3ubv(color);
      glutWireSphere(radius, (int)radius*6, (int)radius*6);
      glPopMatrix();
   }
}

Asteroid arrayAsteroids[ROWS][COLUMNS]; // Global array of asteroids.

// Quadtree node class.
class QuadtreeNode
{
public:
   QuadtreeNode(float x, float z, float s);
   int numberAsteroidsIntersected(); // Return the number of asteroids intersecting the square.
   void addIntersectingAsteroidsToList(); // Add the intersected asteoroids to the local list of asteroids.

   void build(); // Recursive routine to split a square that intersects more than one asteroid; 
                 // if it intersects at most one asteroid leave it as a leaf and add the intersecting 
                 // asteroid, if any, to a local list of asteroids.

   void drawAsteroids(float x1, float z1, float x2, float z2,  // Recursive routine to draw the asteroids
					  float x3, float z3, float x4, float z4); // in a square's list if the square is a
															   // leaf and it intersects the frustum (which
                                                               // is specified by the input parameters); 
															   // if the square is not a leaf, the routine
                                                               // recursively calls itself on its children.
                                                              
private: 
   float SWCornerX, SWCornerZ; // x and z co-ordinates of the SW corner of the square.
   float size; // Side length of square.
   QuadtreeNode *SWChild, *NWChild, *NEChild, *SEChild; // Children nodes.
   list<Asteroid> asteroidList; // Local list of asteroids intersecting the square - only filled for leaf nodes.

   friend class Quadtree;
};

// QuadtreeNode constructor.
QuadtreeNode::QuadtreeNode(float x, float z, float s)
{
   SWCornerX = x; SWCornerZ = z; size = s;
   SWChild = NWChild = NEChild = SEChild = NULL;
   asteroidList.clear();
}

// Return the number of asteroids intersecting the square.
int QuadtreeNode::numberAsteroidsIntersected()
{
   int numVal = 0;
   int i, j;
   for (j=0; j<COLUMNS; j++)
      for (i=0; i<ROWS; i++)
	     if (arrayAsteroids[i][j].getRadius() > 0.0) 
	        if ( checkDiscRectangleIntersection( SWCornerX, SWCornerZ, SWCornerX+size, SWCornerZ-size,
                 arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterZ(), 
				 arrayAsteroids[i][j].getRadius() )
			   )
		       numVal++;
   return numVal;
}

// Add the asteoroids intersecting the square to a local list of asteroids.
void QuadtreeNode::addIntersectingAsteroidsToList()
{
   int i, j;
   for (j=0; j<COLUMNS; j++)
      for (i=0; i<ROWS; i++)
	     if (arrayAsteroids[i][j].getRadius() > 0.0) 
	        if ( checkDiscRectangleIntersection( SWCornerX, SWCornerZ, SWCornerX+size, SWCornerZ-size, 
				 arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterZ(), 
				 arrayAsteroids[i][j].getRadius() )
			   )
		       asteroidList.push_back( Asteroid(arrayAsteroids[i][j]) );
}

// Recursive routine to split a square that intersects more than one asteroid; if it intersects
// at most one asteroid leave it as a leaf and add the intersecting asteroid, if any, to a local 
// list of asteroids.
void QuadtreeNode::build()
{
   if ( this->numberAsteroidsIntersected() <= 1 ) this->addIntersectingAsteroidsToList();
   else
   {
      SWChild = new QuadtreeNode(SWCornerX, SWCornerZ, size/2.0);
      NWChild = new QuadtreeNode(SWCornerX, SWCornerZ - size/2.0, size/2.0);
      NEChild = new QuadtreeNode(SWCornerX + size/2.0, SWCornerZ - size/2.0, size/2.0);
      SEChild = new QuadtreeNode(SWCornerX + size/2.0, SWCornerZ, size/2.0);
	  
	  SWChild->build(); NWChild->build(); NEChild->build(); SEChild->build(); 
   }
}

// Recursive routine to draw the asteroids in a square's list if the square is a
// leaf and it intersects the frustum (which is specified by the input parameters);
// if the square is not a leaf, the routine recursively calls itself on its children.
void QuadtreeNode::drawAsteroids(float x1, float z1, float x2, float z2, 
					             float x3, float z3, float x4, float z4)
{
   // If the square does not intersect the frustum do nothing.
   if ( checkQuadrilateralsIntersection(x1, z1, x2, z2, x3, z3, x4, z4,
								        SWCornerX, SWCornerZ, SWCornerX, SWCornerZ-size,
								        SWCornerX+size, SWCornerZ-size, SWCornerX+size, SWCornerZ) )
   {
      if (SWChild == NULL) // Square is leaf.
	  {
         // Define local iterator to traverse asteroidList and initialize.
         list<Asteroid>::iterator asteroidListIterator;
         asteroidListIterator = asteroidList.begin();

         // Draw all the asteroids in asteroidList.
         while(asteroidListIterator != asteroidList.end() )
		 {
            asteroidListIterator->draw();
	        asteroidListIterator++;
		 }	  
	  }
	  else 
	  {
	     SWChild->drawAsteroids(x1, z1, x2, z2, x3, z3, x4, z4); 
		 NWChild->drawAsteroids(x1, z1, x2, z2, x3, z3, x4, z4); 
		 NEChild->drawAsteroids(x1, z1, x2, z2, x3, z3, x4, z4); 
		 SEChild->drawAsteroids(x1, z1, x2, z2, x3, z3, x4, z4);
	  }
   }
}

// Quadtree class.
class Quadtree
{
public:
   Quadtree() { header = NULL; } // Constructor.
   void initialize(float x, float z, float s); // Initialize quadtree by splitting nodes
                                                     // till each leaf node intersects at
                                                     // most one asteroid.

   void drawAsteroids(float x1, float z1, float x2, float z2,  // Routine to draw all the asteroids in the  
					  float x3, float z3, float x4, float z4); // asteroid list of each leaf square that
                                                               // intersects the frustum.

private:
   QuadtreeNode *header;
};

// Initialize quadtree by splitting nodes till each leaf node intersects at most one asteroid.
void Quadtree::initialize(float x, float z, float s)
{
   header = new QuadtreeNode(x, z, s);
   header->build();
}

// Routine to draw all the asteroids in the asteroid list of each leaf square that intersects the frustum.
void Quadtree::drawAsteroids(float x1, float z1, float x2, float z2, 
					         float x3, float z3, float x4, float z4)
{
   header->drawAsteroids(x1, z1, x2, z2, x3, z3, x4, z4); 
}

Quadtree asteroidsQuadtree; // Global quadtree.

// Initialization routine.
void setup(void) 
{
   int i, j;
   float initialSize;

   spacecraft = glGenLists(1);
   glNewList(spacecraft, GL_COMPILE);
	  glPushMatrix();
	  glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
	  glColor3f (1.0, 1.0, 1.0); 
      glutWireCone(5.0, 10.0, 10, 10);
	  glPopMatrix();
   glEndList();
      
   // Initialize global arrayAsteroids.
   for (j=0; j<COLUMNS; j++)
      for (i=0; i<ROWS; i++)
	     if (rand()%100 < FILL_PROBABILITY)
		 // If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot which
		 // indicates that there is no asteroid there because the default's radius is 0.
		 {
			// Position the asteroids depending on if there is an even or odd number of columns
			// so that the spacecraft faces the middle of the asteroid field.
			if (COLUMNS%2) // Odd number of columns.
	           arrayAsteroids[i][j] = Asteroid( 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, 3.0, 
			                                    rand()%256, rand()%256, rand()%256);
			else // Even number of columns.
			   arrayAsteroids[i][j] = Asteroid( 15.0 + 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, 3.0, 
			                                    rand()%256, rand()%256, rand()%256);
		 }

   // Initialize global asteroidsQuadtree - the root square bounds the entire asteroid field.
   if (ROWS <= COLUMNS) initialSize = (COLUMNS - 1)*30.0 + 6.0;
   else initialSize = (ROWS - 1)*30.0 + 6.0;
   asteroidsQuadtree.initialize( -initialSize/2.0, -37.0, initialSize );

   glEnable(GL_DEPTH_TEST);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1, 
						       float x2, float y2, float z2, float r2)
{
   return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision( float x, float z, float a)
{
   int i,j;

   // Check for collision with each asteroid.
   for (j=0; j<COLUMNS; j++)
      for (i=0; i<ROWS; i++)
		 if (arrayAsteroids[i][j].getRadius() > 0 ) // If asteroid exists.
            if ( checkSpheresIntersection( x - 5 * sin( (PI/180.0) * a), 0.0, 
		         z - 5 * cos( (PI/180.0) * a), 7.072, 
		         arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(), 
		         arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius() ) )
		       return 1;
   return 0;
}

// Drawing routine.
void drawScene(void)
{ 
   int i, j;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Begin left viewport.
   glViewport (0, 0, width/2.0,  height); 
   glLoadIdentity();
   
   // Write text in isolated (i.e., before gluLookAt) translate block.
   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(5.0, 25.0, -30.0);
   if (isFrustumCulled) writeBitmapString((void*)font, "Frustum culling on!");
   else writeBitmapString((void*)font, "Frustum culling off!");
   glColor3f(1.0, 0.0, 0.0);
   glRasterPos3f(-28.0, 25.0, -30.0);
   if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
   glPopMatrix();
   
   // Fixed camera.
   gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 
   if (!isFrustumCulled)
  // Draw all the asteroids in arrayAsteroids.
   {
      for (j=0; j<COLUMNS; j++)
         for (i=0; i<ROWS; i++)
            arrayAsteroids[i][j].draw();
   }
   else // Draw only asteroids in leaf squares of the quadtree that intersect the fixed frustum
	    // with apex at the origin.
      asteroidsQuadtree.drawAsteroids(-5.0, -5.0, -250.0, -250.0, 250.0, -250.0, 5.0, -5.0 );

   // Draw spacecraft.
   glPushMatrix();
   glTranslatef(xVal, 0.0, zVal);
   glRotatef(angle, 0.0, 1.0, 0.0);
   glCallList(spacecraft);
   glPopMatrix();
   // End left viewport.

   // Begin right viewport.
   glViewport(width/2.0, 0, width/2.0, height);
   glLoadIdentity();

   // Write text in isolated (i.e., before gluLookAt) translate block.
   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(5.0, 25.0, -30.0);
   if (isFrustumCulled)  writeBitmapString((void*)font, "Frustum culling on.");
   else writeBitmapString((void*)font, "Frustum culling off.");
   glColor3f(1.0, 0.0, 0.0);
   glRasterPos3f(-28.0, 25.0, -30.0);
   if (isCollision)  writeBitmapString((void*)font, "Cannot - will crash!");
   glPopMatrix();

   // Draw a vertical line on the left of the viewport to separate the two viewports
   glColor3f(1.0, 1.0, 1.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(-5.0, -5.0, -5.0);
      glVertex3f(-5.0, 5.0, -5.0);
   glEnd();
   glLineWidth(1.0);

   // Locate the camera at the tip of the cone and pointing in the direction of the cone.
   gluLookAt(xVal - 10 * sin( (PI/180.0) * angle), 
	         0.0, 
			 zVal - 10 * cos( (PI/180.0) * angle), 
	         xVal - 11 * sin( (PI/180.0) * angle),
			 0.0,
             zVal - 11 * cos( (PI/180.0) * angle), 
             0.0, 
			 1.0, 
			 0.0);

   if (!isFrustumCulled)
   // Draw all the asteroids in arrayAsteroids.
   {
      for (j=0; j<COLUMNS; j++)
         for (i=0; i<ROWS; i++)
            arrayAsteroids[i][j].draw();
   }
   else // Draw only asteroids in leaf squares of the quadtree that intersect the frustum
	    // "carried" by the spacecraft with apex at its tip and oriented with its axis
		// along the spacecraft's axis.
      asteroidsQuadtree.drawAsteroids( xVal - 7.072 * sin( (PI/180.0) * (45.0 + angle) ),
	                                   zVal - 7.072 * cos( (PI/180.0) * (45.0 + angle) ),
								       xVal - 353.6 * sin( (PI/180.0) * (45.0 + angle) ),
	                                   zVal - 353.6 * cos( (PI/180.0) * (45.0 + angle) ),
                                       xVal + 353.6 * sin( (PI/180.0) * (45.0 - angle) ),
									   zVal - 353.6 * cos( (PI/180.0) * (45.0 - angle) ),
									   xVal + 7.072 * sin( (PI/180.0) * (45.0 - angle) ),
	                                   zVal - 7.072 * cos( (PI/180.0) * (45.0 - angle) )
	                                 );
   // End right viewport.

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
   glMatrixMode(GL_MODELVIEW);

   // Pass the size of the OpenGL window.
   width = w;
   height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
	  case ' ':
	     isFrustumCulled = 1 - isFrustumCulled;
		 glutPostRedisplay();
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

   // Compute next position.
   if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
   if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
   if( key == GLUT_KEY_UP)
   {
	  tempxVal = xVal - sin(angle * PI/180.0); 
	  tempzVal = zVal - cos(angle * PI/180.0);
   }
   if( key == GLUT_KEY_DOWN)
   {
	  tempxVal = xVal + sin(angle * PI/180.0); 
	  tempzVal = zVal + cos(angle * PI/180.0);
   }

   // Angle correction.
   if (tempAngle > 360.0) tempAngle -= 360.0;
   if (tempAngle < 0.0) tempAngle += 360.0;

   // Move spacecraft to next position only if there will not be collision with an asteroid.
   if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle) )
   {
      isCollision = 0;
      xVal = tempxVal;
	  zVal = tempzVal;
	  angle = tempAngle;
   }
   else isCollision = 1;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "ALERT: The OpenGL window may take a while to come up because" << endl
		<< "of the time to build the quadtree!" << endl
		<<  endl;
   cout << "Interaction:" << endl;
   cout << "Press the left/right arrow keys to turn the craft." << endl
        << "Press the up/down arrow keys to move the craft." << endl
		<< "Press space to toggle between frustum culling enabled and disabled." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(800, 400);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("spaceTravelFrustumCulled.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}

