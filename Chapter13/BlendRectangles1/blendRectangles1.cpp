//////////////////////////////////////////////////////////////////////////////////////          
// blendRectangles1.cpp
//
// This program draws two rectangles that are blended in their intersection. The order
// in which the rectangles are drawn can be changed.
//
// Interaction:
// Press space to toggle between either rectangle being drawn first.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

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

// Globals.
static int isBlueFirst = 1; // Blue rectangle drawn first?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Function to draw a blue rectangle at z = 0.1.
void drawBlueRectangle(void)
{
   glColor4f(0.0, 0.0, 1.0, 0.5); // Translucent: alpha value = 0.5.
   glBegin(GL_POLYGON);
      glVertex3f(-40.0, -35.0, 0.1);
      glVertex3f(40.0, -35.0, 0.1);
      glVertex3f(40.0, -20.0, 0.1);
      glVertex3f(-40.0, -20.0, 0.1);
   glEnd();
}

// Function to draw a red rectangle at z = 0.5.
void drawRedRectangle(void)
{
   glColor4f(1.0, 0.0, 0.0, 0.5); // Translucent: alpha value = 0.5.
   glBegin(GL_POLYGON);
      glVertex3f(30.0, 35.0, 0.5);
      glVertex3f(15.0, 35.0, 0.5);
      glVertex3f(15.0, -45.0, 0.5);
      glVertex3f(30.0, -45.0, 0.5);
   glEnd();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glDisable(GL_DEPTH_TEST); // Disable depth testing.

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   if (isBlueFirst) // Draw blue rectangle first.
   {
	  drawBlueRectangle();
      drawRedRectangle();
	  glColor3f(0.0, 0.0, 0.0);
	  glRasterPos3f(-45.0, 40.0, 0.0);
      writeBitmapString((void*)font, "Blue rectangle drawn first, red rectangle second in code!");
   }
   else // Draw red rectangle first.
   {
	  drawRedRectangle();
	  drawBlueRectangle();
	  glColor3f(0.0, 0.0, 0.0);
	  glRasterPos3f(-45.0, 40.0, 0.0);
      writeBitmapString((void*)font, "Red rectangle drawn first, blue rectangle second in code!");
   }

   glFlush();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
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
         if (isBlueFirst) isBlueFirst = 0;
		 else isBlueFirst = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between either rectangle being drawn first." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("blendRectangles1.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
