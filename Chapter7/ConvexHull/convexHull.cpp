/////////////////////////////////////////////////////////////////////
// convexHull.cpp
//
// This program draws the convex hull of a set of 8 points on the 
// plane, each of which can be moved. 
//
// The convex hull is determined as the union of all triangles with
// vertices from the given points. This process is O(n^3) and 
// asymptotically inefficient.
//
// Interaction:
// Press space to select a point. 
// Press the arrow keys to move the selected point.
//
// Sumanta Guha
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
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

using namespace std;

// Start globals. 
static int numVal = 1; // Index of selected point.

// Set of 8 points.
static float points[8][3] = 
{
	{20.0, 20.0, 0.0}, {10.0, 50.0, 0.0}, 
	{20.0, 80.0, 0.0}, {50.0, 90.0, 0.0},
	{80.0, 80.0, 0.0}, {90.0, 50.0, 0.0},
	{80.0, 20.0, 0.0}, {50.0, 10.0, 0.0}
};
// End globals.

// Compute the hull as the union of all triangles with vertices from
// the given set of points (inefficient - see remarks above).
void computeHull(void)
{
   int i,j, k;
   for (i = 0; i < 8; i++)
      for (j = 0; j < 8; j++)
         for (k = 0; k < 8; k++)
		 {
		    glBegin(GL_TRIANGLES);
		       glVertex3fv(points[i]);
			   glVertex3fv(points[j]);
			   glVertex3fv(points[k]);
	        glEnd();
		 }
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   glPointSize(5.0);

   // Draw the convex hull yellow.
   glColor3f(1.0, 1.0, 0.0);
   computeHull();

   // Draw the points black.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 8; i++) glVertex3fv(points[i]);     
   glEnd();

   // Draw the selected point in red.
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3fv(points[numVal]);     
   glEnd();

   glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
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
	     if (numVal < 7) numVal++;
	     else numVal = 0;
         glutPostRedisplay();
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP) if (points[numVal][1] < 100.0) points[numVal][1] += 0.5f;
   if (key == GLUT_KEY_DOWN) if (points[numVal][1] > 0.0) points[numVal][1] -= 0.5f;
   if (key == GLUT_KEY_LEFT) if (points[numVal][0] > 0.0) points[numVal][0] -= 0.5f;
   if (key == GLUT_KEY_RIGHT) if (points[numVal][0] < 100.0) points[numVal][0] += 0.5f;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to select a point." << endl
        << "Press the arrow keys to move the selected point." << endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("convexHull.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();   
}
