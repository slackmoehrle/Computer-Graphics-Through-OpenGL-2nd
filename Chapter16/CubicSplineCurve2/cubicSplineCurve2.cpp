///////////////////////////////////////////////////////////////////////////////////
// cubicSplineCurve2.cpp
//
// This program draws the cubic B-spline approximation of 30 movable control points
// over a fixed standard knot vector.
//
// Interaction: 
//
// Press the shift and backspace key to cycle through the control points.
// The selected control point (in red) is moved using the arrow keys.
// Press delete to reset control points.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////

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

using namespace std;

// Begin globals.
static int selectedControlPoint = 0; // Selected control point number.
static float ctrlpoints[30][3]; // Control points.

// Standard knot vector.
static float knots[34] = 
{0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 
7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 
16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 
25.0, 26.0, 27.0, 27.0, 27.0, 27.0};

static GLUnurbsObj *nurbsObject; // Pointer to NURBS object.
// End globals.

// Reset control points.
void resetControlPoints(void)
{
   int i;
   float t;
   for (i = 0; i < 30; i++)  
   {
      t = 2 * PI * i / 30.0;
      ctrlpoints[i][0] = 30.0 * cos(t);
      ctrlpoints[i][1] = 30.0 * sin(t);
	  ctrlpoints[i][2] = 0.0;
   }
   selectedControlPoint = 0;
}

// Initialization routine.			
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Create NURBS object.
   nurbsObject = gluNewNurbsRenderer();
   gluNurbsProperty(nurbsObject, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
   gluNurbsProperty(nurbsObject, GLU_SAMPLING_TOLERANCE, 10.0);

   resetControlPoints();
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
   
   // Draw the spline curve.
   glColor3f(0.0, 0.0, 0.0);
   gluBeginCurve(nurbsObject);
   gluNurbsCurve(nurbsObject, 34, knots, 3, ctrlpoints[0], 4, GL_MAP1_VERTEX_3);
   gluEndCurve(nurbsObject);

   // The following code displays the control points as dots.
   glPointSize(5.0);
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 30; i++) 
         glVertex3fv(ctrlpoints[i]);
   glEnd();

   // Highlight the selected control point.
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
	  glVertex3fv(ctrlpoints[selectedControlPoint]);     
   glEnd();

   glPopMatrix();
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
      case ' ':
		 if (selectedControlPoint < 29) selectedControlPoint++;
		 else selectedControlPoint = 0;
         glutPostRedisplay();
         break;
      case 8:
		 if (selectedControlPoint > 0) selectedControlPoint--;
		 else selectedControlPoint = 29;
         glutPostRedisplay();
         break;
      case 127:	
         resetControlPoints();
		 glutPostRedisplay();
         break;
      default:
         break;
  }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) ctrlpoints[selectedControlPoint][1] += 1.0;
   if(key == GLUT_KEY_DOWN) ctrlpoints[selectedControlPoint][1] -= 1.0;
   if(key == GLUT_KEY_LEFT) ctrlpoints[selectedControlPoint][0] -= 1.0;
   if(key == GLUT_KEY_RIGHT) ctrlpoints[selectedControlPoint][0] += 1.0;
	
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the shift and backspace key to cycle through the control points." << endl
        << "The selected control point (in red) is moved using the arrow keys." << endl
        << "Press delete to reset control points." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
  
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("cubicSplineCurve2.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
   
   glutMainLoop();
}

