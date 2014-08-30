////////////////////////////////////////////////////////////////////////////////////          
// clippingPlanes.cpp
// 
// This program augments circularAnnuluses.cpp with two clipping planes.
//
// Interaction:
// Press the space bar to toggle between wireframe and filled for the lower annulus.
// Press '0' to enable/disable clipping plane 0.
// Press '1' to enable/disable clipping plane 1.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
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

#define PI 3.14159265
#define N 40.0 // Number of vertices on boundary of disc.

using namespace std;

// Globals.
static int isWire = 0; // Is wireframe?
static int isClip0 = 0; // Is clipping plane 0 enabled?
static int isClip1 = 0; // Is clipping plane 1 enabled?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}  

// Function to draw a disc with center at (X, Y, Z), radius R, parallel to the
// xy-plane.
void drawDisc(float R, float X, float Y, float Z)
{
   float t;
   int i;

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f( X, Y, Z);
      for(i = 0; i <= N; ++i)
	  {
         t = 2 * PI * i / N; 
         glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
	  }
   glEnd();
}

// Drawing routine.
void drawScene(void)
{  
   float angle;
   int i;
   double eqn0[4] = {0.0, 0.0, -1.0, 0.25}; // Data for clipping plane 0.
   double eqn1[4] = {1.0, 0.5, 0.0, -60.0}; // Data for clipping plane 1.

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glPolygonMode(GL_FRONT, GL_FILL);

   glClipPlane(GL_CLIP_PLANE0, eqn0); // Specify clipping plane 0.
   glClipPlane(GL_CLIP_PLANE1, eqn1); // Specify clipping plane 1.

   if (isClip0) glEnable(GL_CLIP_PLANE0); // Clip points s.t. z > 0.25.
   else glDisable(GL_CLIP_PLANE0);

   if (isClip1) glEnable(GL_CLIP_PLANE1); // Clip points s.t. x > 75.0.
   else glDisable(GL_CLIP_PLANE1);

   // Upper left circular annulus: the white disc overwrites the red disc.
   glColor3f(1.0, 0.0, 0.0);
   drawDisc(20.0, 25.0, 75.0, 0.0);
   glColor3f(1.0, 1.0, 1.0);
   drawDisc(10.0, 25.0, 75.0, 0.0);

   // Upper right circular annulus: the white disc floats above the red disc blocking it.
   glEnable(GL_DEPTH_TEST); // Enable depth testing. 
   glColor3f(1.0, 0.0, 0.0);
   drawDisc(20.0, 75.0, 75.0, 0.0);
   glColor3f(1.0, 1.0, 1.0);
   drawDisc(10.0, 75.0, 75.0, 0.5); // Compare this z-value with that of the red disc.
   glDisable(GL_DEPTH_TEST); // Disable depth testing.

   // Lower circular annulus: with a true hole.
   if (isWire) glPolygonMode(GL_FRONT, GL_LINE);else glPolygonMode(GL_FRONT, GL_FILL);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i <= N; ++i)
	  {
         angle = 2 * PI * i / N; 
         glVertex3f(50 + cos(angle) * 10.0, 30 + sin(angle) * 10.0, 0.0);
         glVertex3f(50 + cos(angle) * 20.0, 30 + sin(angle) * 20.0, 0.0);
	  }
   glEnd();

   // Write labels.
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(15.0, 51.0, 0.0);
   writeBitmapString((void*)font, "Overwritten");
   glRasterPos3f(69.0, 51.0, 0.0);
   writeBitmapString((void*)font, "Floating");
   glRasterPos3f(38.0, 6.0, 0.0);
   writeBitmapString((void*)font, "The real deal!");

   glFlush();
}

// Initialization routine.
void setup (void) 
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
   switch(key) 
   {
      case ' ':
         if (isWire == 0) isWire = 1;
         else isWire = 0;
         glutPostRedisplay();
         break;	
      case '0':
         if (isClip0 == 0) isClip0 = 1;
         else isClip0 = 0;
         glutPostRedisplay();
         break;	  
      case '1':
         if (isClip1 == 0) isClip1 = 1;
         else isClip1 = 0;
         glutPostRedisplay();
         break;	  
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to toggle between wireframe and filled" << endl 
	    << "for the lower annulus." << endl
		<< "Press '0' to enable/disable clipping plane 0." << endl
		<< "Press '1' to enable/disable clipping plane 1." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("clippingPlanes.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
