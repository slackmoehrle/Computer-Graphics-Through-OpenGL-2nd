//////////////////////////////////////////////////////////////////////////////////////////////
// rationalBezierCurve2.cpp
// 
// This program draws a quadratic rational Bezier curve specified by control points at
// (1,0), (1.1) and (0,1). The weights are shown at lower left. A circle of unit radius 
// is drawn as well.
//
// Interaction:
// Press the up/down arrow keys to increase/decrease the weight of the control point at (1,1).
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
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

#define PI 3.14159265358979324

using namespace std;

// Begin globals.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Control points.
static float controlPoints[3][3] = 
{
	{40.0, 0.0, 0.0}, {40.0, 40.0, 0.0}, {0.0, 40.0, 0.0}
};

// Weights
static float weights[3] = {1.0, 1.5, 1.0};

// Control points in homogeneous co-ordinates.
static float controlPointsHomogeneous[3][4];
// End globals.

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
   
   floatToString(theStringBuffer, 4, weights[0]);
   glRasterPos3f(-45.0, -40.0, 0.0);
   writeBitmapString((void*)font, "weights[0]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[1]);
   glRasterPos3f(-45.0, -42.0, 0.0);
   writeBitmapString((void*)font, "weights[1]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[2]);
   glRasterPos3f(-45.0, -44.0, 0.0);
   writeBitmapString((void*)font, "weights[2]: ");
   writeBitmapString((void*)font, theStringBuffer);

   glRasterPos3f(40.0, -3.0, 0.0);
   writeBitmapString((void*)font, "(1,0)");

   glRasterPos3f(40.0, 42.0, 0.0);
   writeBitmapString((void*)font, "(1,1)");

   glRasterPos3f(0.0, 42.0, 0.0);
   writeBitmapString((void*)font, "(0,1)");
}

// Routine to lift 3D control points to projective 3-space by specfying homogeneous co-ordinates.
void computeControlPointsHomogeneous(void)
{
   int i, j;
   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 3; j++)
	  {
	     controlPointsHomogeneous[i][j] = controlPoints[i][j] * weights[i];
	  }
      controlPointsHomogeneous[i][3] = weights[i];
   }
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   computeControlPointsHomogeneous();
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   int i;
   float t;

   glPointSize(5.0);

   // Draw black x and y axes.
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_LINES);
      glVertex3f(0.0, -50.0, 0.0);
      glVertex3f(0.0, 50.0, 0.0);
      glVertex3f(-50.0, 0.0, 0.0);
      glVertex3f(50.0, 0.0, 0.0);
   glEnd();

   // Draw light gray circle.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_LOOP);
   for(i = 0; i < 40; ++i)
   {
      t = 2 * PI * i / 40;
      glVertex3f(40.0 * cos(t), 40.0 * sin(t), 0.0);
   }
   glEnd();

   // Draw the red rational Bezier curve.
   glColor3f(1.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 3, controlPointsHomogeneous[0]);
   glEnable(GL_MAP1_VERTEX_4);
   glMapGrid1f(100, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 100);

   // Draw the control points as red dots.
   glColor3f(1.0, 0.0, 0.0);	  
   glBegin(GL_POINTS);
   for (i = 0; i < 3; i++) 
      glVertex3fv(controlPoints[i]);
   glEnd();

   writeData();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
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
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) weights[1] += 0.01f;
   if(key == GLUT_KEY_DOWN) if ( weights[1] > 0.02 ) weights[1] -= 0.01f;
   computeControlPointsHomogeneous();
	
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up/down arrow keys to increase/decrease the weight of the control point at (1,1)." << endl;
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
   glutCreateWindow("rationalBezierCurve2.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
 
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop(); 
}

