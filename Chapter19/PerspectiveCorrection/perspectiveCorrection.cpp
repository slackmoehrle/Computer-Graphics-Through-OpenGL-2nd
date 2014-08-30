/////////////////////////////////////////////////////////////////////////////////////////////         
// perspectiveCorrection.cpp
//
// This program illustrates the perspective correction that is required when coloring the 
// pixels of a primitive by interpolation, following the projection transformation.
// It shows a thick line joining a red vertex at its left end to a green vertex at its right.
// On top of the line is a big point which can be slid along the line and its color changed
// as well. Messages indicate the fraction of the way the point is from the left end of the
// line to the right, and the fraction of the way its color is from red to green.
// 
// Interaction:
// Press the left/right arrow keys to move the point.
// Press the up/down arrow keys to change its color.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////////////// 

#include <iostream>
#include <fstream>

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
static float t = 0.0; // Fraction of distance point is from the left end of the line.
static float u = 0.0; // Fraction of "distance" point color is from red to green.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
}

// Write data.
void writeData(void)
{
   glColor3f(0.0, 0.0, 0.0);
   
   floatToString(theStringBuffer, 5, t);
   glRasterPos3f(-1.1, 1.05, -2.0);
   writeBitmapString((void*)font, "Fraction of the distance the point is from left end to right: ");
   writeBitmapString((void*)font, theStringBuffer);
   
   floatToString(theStringBuffer, 5, u);
   glRasterPos3f(-1.1, 1.0, -2.0);
   writeBitmapString((void*)font, "Fraction of the way the point's color is from red to green:   ");  
   writeBitmapString((void*)font, theStringBuffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity(); 

   writeData();

   // Draw the fixed line.
   glLineWidth(5.0);
   glBegin(GL_LINES);
       glColor3f(1.0, 0.0, 0.0); glVertex3f(0.0, 0.0, -1.0); 
	   glColor3f(0.0, 1.0, 0.0); glVertex3f(1.0, 0.0, -2.0); 
   glEnd();

   // Draw the point which can be moved and its color change.
   glPointSize(5.0);
   glBegin(GL_POINTS);
       glColor3f(1-u, u, 0.0); glVertex3f(t*0.5, 0.015, -1.0); 
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
   gluPerspective(60.0, 1.0, 1.0, 5.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT) if (t < 1.0) t += 0.004;
   if (key == GLUT_KEY_LEFT) if (t > 0.0) t -= 0.004;
   if (key == GLUT_KEY_UP) if (u < 1.0) u += 0.004;
   if (key == GLUT_KEY_DOWN) if (u > 0.0) u -= 0.004;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left/right arrow keys to move the point." << endl
        << "Press the up/down arrow keys to change its color." << endl;
}

int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
  
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("perspectiveCorrection.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
 
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}

