//////////////////////////////////////////////////////////////////////////////////////////////
// rationalBezierCurve1.cpp
// 
// This program shows a red rational Bezier curve on the plane z=1 specified by 4 red
// control points (except that the selected control point is drawn white). The plane z=1 
// is represented as a triangular grid. The weights of the control points are shown at 
// the upper-left.
//
// For each control point (x, y, 1) the corresponding lifted control point (xw, yw, w)
// is drawn in green (except that the one corresponding to the selected 2D control point
// is drawn white) and of a bigger size. The polynomial Bezier curve in 3-space 
// specified by the 4 lifted control points is drawn green.
//
// The origin of 3-space is a blue point. Blue radial lines are drawn from the origin
// through each control point on z=1 and its corresponding lifted control point. A cone,
// consisting of several light gray lines, is also drawn on the green lifted polynomial
// Bezier curve. The intersection of this cone with the z=1 plane is the red rational
// Bezier curve.
//
// Interaction:
// Press space to select a control point.
// Press the up/down arrow keys to increase/decrease the weight of the selected control point.
// Press delete to reset the weights.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////////////////

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
static int numVal = 1; // Index of selected control point.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Control points on the z=1 plane.
static float controlPoints[4][3] = 
{
	{7.0, 2.0, 1.0}, {5.0, 0.0, 1.0}, {3.0, 7.0, 1.0}, {0.0, 3.0, 1.0}
};

// Weights
static float weights[4] = {1.5, 1.5, 1.5, 1.5};

// Control points in 3D space.
static float controlPointsLifted[4][3];

// Control points in homogeneous co-ordinates.
static float controlPointsHomogeneous[4][4];
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
   glRasterPos3f(-5.0, 10.0, 3.0);
   writeBitmapString((void*)font, "weights[0]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[1]);
   glRasterPos3f(-6.75, 11.25, 3.0);
   writeBitmapString((void*)font, "weights[1]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[2]);
   glRasterPos3f(-9.0, 12.75, 3.0);
   writeBitmapString((void*)font, "weights[2]: ");
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, weights[3]);
   glRasterPos3f(-11.75, 14.75, 3.0);
   writeBitmapString((void*)font, "weights[3]: ");
   writeBitmapString((void*)font, theStringBuffer);
}

// Routine to lift 3D control points to projective 3-space by specfying homogeneous co-ordinates.
void computeControlPointsHomogeneous(void)
{
   int i, j;
   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < 3; j++)
	  {
	     controlPointsHomogeneous[i][j] = controlPoints[i][j] * weights[i];
	  }
      controlPointsHomogeneous[i][3] = weights[i];
   }
}

// Routine to lift the control point (x, y, 1) on the z=1 plane to projective 2-space by 
// specifying homogenous co-ordinates (xw, yw, w).
void computeControlPointsLifted(void)
{
   int i, j;
   for (i = 0; i < 4; i++) 
      for (j = 0; j < 3; j++)
	     controlPointsLifted[i][j] = controlPoints[i][j] * weights[i];
}

// Routine to restore weights to original values.
void restoreWeights(void)
{
   int i;
   for (i=0; i<4; i++)
   {
	  weights[i] = 1.5;
   }
   computeControlPointsHomogeneous();
   computeControlPointsLifted();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   computeControlPointsHomogeneous();
   computeControlPointsLifted();
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   glTranslatef(-8.3, -5.8, -7.0);
   glRotatef(300.0, 1.0, 0.0, 0.0);
   glScalef(1.0, 1.0, 5.0);
   
   // Draw the plane z=1 as a triangular grid.
   glColor3f(0.8, 0.8, 0.8);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   for(float y=-5.0; y<10.0; y+=1.0)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(float x=-5.0; x<100.0; x+=1.0)
	  {
	     glVertex3f(x, y, 1.0);
	     glVertex3f(x, y+1.0, 1.0);
	  }
      glEnd();
   }

   // Draw the polynomial Bezier curve in 3-space in green.
   glColor3f(0.0, 1.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, controlPointsLifted[0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(100, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 100);
   glDisable(GL_MAP1_VERTEX_3);

   // Draw the rational Bezier curve on the plane in red.
   glColor3f(1.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 4, controlPointsHomogeneous[0]);
   glEnable(GL_MAP1_VERTEX_4);
   glMapGrid1f(100, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 100);
   glDisable(GL_MAP1_VERTEX_4);

   // Draw the cone through the polynomial Bezier curve in 3-space in a light gray
   // to intesect the rational Bezier curve on z=1..
   glColor3f(0.2, 0.2, 0.2);
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, controlPointsLifted[0]);
   glEnable(GL_MAP1_VERTEX_3);
   glBegin(GL_LINES);
      for (i = 0; i <= 20; i++)
	  {	 
		 glVertex3f(0.0, 0.0, 0.0);
         glEvalCoord1f( (float)i/20.0 );
	  }
   glEnd();
   glDisable(GL_MAP1_VERTEX_3);
   glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 4, controlPointsHomogeneous[0]);
   glEnable(GL_MAP1_VERTEX_4);
   glBegin(GL_LINES);
      for (i = 0; i <= 20; i++)
	  {	 
		 glVertex3f(0.0, 0.0, 0.0);
         glEvalCoord1f( (float)i/20.0 );
	  }
   glEnd();
   glDisable(GL_MAP1_VERTEX_4);

   // Draw the control points on the plane as red dots.
   glPointSize(5.0);
   glColor3f(1.0, 0.0, 0.0);	  
   glBegin(GL_POINTS);
   for (i = 0; i < 4; i++) 
      glVertex3fv(controlPoints[i]);
   glEnd();

   // Draw the lifted control points in 3-space as green dots.
   glPointSize(7.0);
   glColor3f(0.0, 1.0, 0.0);	  
   glBegin(GL_POINTS);
   for (i = 0; i < 4; i++) 
      glVertex3fv(controlPointsLifted[i]);
   glEnd();

   // Highlight selected control point and lifted control point drawing them black.
   glColor3f(0.0, 0.0, 0.0);
   glPointSize(5.0);
   glBegin(GL_POINTS);
	  glVertex3fv(controlPoints[numVal]); 
   glEnd();
      glPointSize(7.0);
   glBegin(GL_POINTS);
      glVertex3fv(controlPointsLifted[numVal]); 
   glEnd();

   // Draw blue origin.
   glColor3f(0.0, 0.0, 1.0);
   glPointSize(5.0);
   glBegin(GL_POINTS);
      glVertex3f(0.0, 0.0, 0.0);     
   glEnd();

   // Draw blue lines from the origin through the control points on the plane 
   // to the lifted control points.
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_LINES);
	   for (i = 0; i < 4; i++) 
	  {	 
		 glVertex3f(0.0, 0.0, 0.0);
         glVertex3fv(controlPointsLifted[i]);
         glVertex3f(0.0, 0.0, 0.0);
         glVertex3fv(controlPoints[i]);
	  }
   glEnd();

   writeData();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(120.0, (float)w/(float)h, 1.0, 50.0);
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
	     restoreWeights();
	     glutPostRedisplay();
         break;
      case ' ':
		 if (numVal < 3) numVal++;
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
   if(key == GLUT_KEY_UP) weights[numVal] += 0.01f;
   if(key == GLUT_KEY_DOWN) if ( weights[numVal] > 0.02 ) weights[numVal] -= 0.01f;
   computeControlPointsHomogeneous();
   computeControlPointsLifted();
	
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to select a control point." << endl
        << "Press the up/down arrow keys to increase/decrease the weight of the selected control point." << endl
        << "Press delete to reset the weights." << endl;
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
   glutCreateWindow("rationalBezierCurve1.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
  
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
   
   glutMainLoop(); 
}

