/////////////////////////////////////////////////////////////////////////////////////////////
// rationalBezierCurve3.cpp
// 
// This program draws a rational Bezier curve specified by by 6 white control points (except 
// that the selected control point is drawn red).
// 
// Interaction:
// Press space to select a control point.
// Press the arrow keys to move the selected control point.
// Press the page up/down keys to increase/decrease the weight of the selected control point.
// Press delete to reset the control points.
//
//Sumanta Guha
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

// Begin globals.
static int numVal = 0; // Index of selected control point.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Control points.
static float controlPoints[6][3] = 
{
	{40.0, 0.0, 0.0}, {40.0, 40.0, 0.0}, {0.0, 40.0, 0.0},
	{0.0, 0.0, 0.0}, {-40.0, 0.0, 0.0}, {-40.0, -40.0, 0.0}
};

// Copy of original control points.
static float originalControlPoints[6][3] = 
{
	{40.0, 0.0, 0.0}, {40.0, 40.0, 0.0}, {0.0, 40.0, 0.0},
	{0.0, 0.0, 0.0}, {-40.0, 0.0, 0.0}, {-40.0, -40.0, 0.0}
};

// Weights
static float weights[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

// Control points in homogeneous co-ordinates.
static float controlPointsHomogeneous[6][4];
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
   glRasterPos3f(23.0, -36.0, 0.0);
   writeBitmapString((void*)font, "weights[0]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[1]);
   glRasterPos3f(23.0, -38.0, 0.0);
   writeBitmapString((void*)font, "weights[1]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[2]);
   glRasterPos3f(23.0, -40.0, 0.0);
   writeBitmapString((void*)font, "weights[2]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[3]);
   glRasterPos3f(23.0, -42.0, 0.0);
   writeBitmapString((void*)font, "weights[3]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[4]);
   glRasterPos3f(23.0, -44.0, 0.0);
   writeBitmapString((void*)font, "weights[4]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[5]);
   glRasterPos3f(23.0, -46.0, 0.0);
   writeBitmapString((void*)font, "weights[5]: ");
   writeBitmapString((void*)font, theStringBuffer);
}

// Routine to lift 3D control points to projective 3-space by specfying homogeneous co-ordinates.
void computeControlPointsHomogeneous(void)
{
   int i, j;
   for (i = 0; i < 6; i++)
   {
      for (j = 0; j < 3; j++)
	  {
	     controlPointsHomogeneous[i][j] = controlPoints[i][j] * weights[i];
	  }
      controlPointsHomogeneous[i][3] = weights[i];
   }
}

// Routing to restore control points to original settings.
void restoreControlPoints(void)
{
   int i, j;
   for (i=0; i<6; i++)
   {
	  weights[i] = 1.0;
	  for (j=0; j<3; j++) controlPoints[i][j] = originalControlPoints[i][j];
   }
   computeControlPointsHomogeneous();
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

   glPointSize(5.0);

   // Draw the black rational Bezier curve.
   glColor3f(0.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 6, controlPointsHomogeneous[0]);
   glEnable(GL_MAP1_VERTEX_4);
   glMapGrid1f(100, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 100);

   // Draw the control points as green dots.
   glColor3f(0.0, 1.0, 0.0);	  
   glBegin(GL_POINTS);
   for (i = 0; i < 6; i++) 
      glVertex3fv(controlPoints[i]);
   glEnd();

   // Highlight selected control point drawing it red.
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS); 
	  glVertex3fv(controlPoints[numVal]); 
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
      case 127:	
	     restoreControlPoints();
	     glutPostRedisplay();
         break;
      case ' ':
		 if (numVal < 5) numVal++;
		 else numVal = 0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_LEFT) controlPoints[numVal][0] -= 0.1f;
   if(key == GLUT_KEY_RIGHT) controlPoints[numVal][0] += 0.1f;   
   if(key == GLUT_KEY_UP) controlPoints[numVal][1] += 0.1f;
   if(key == GLUT_KEY_DOWN) controlPoints[numVal][1] -= 0.1f;
   if(key == GLUT_KEY_PAGE_DOWN) if ( weights[numVal] > 0.02 ) weights[numVal] -= 0.01f;
   if(key == GLUT_KEY_PAGE_UP) weights[numVal] += 0.01f;
   computeControlPointsHomogeneous();

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to select a control point." << endl
        << "Press the arrow keys to move the selected control point." << endl
        << "Press the page up/down keys to increase/decrease the weight of the selected control point." << endl
        << "Press delete to reset the control points." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
 
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("rationalBezierCurve3.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
  
   glutMainLoop(); 
}

