//////////////////////////////////////////////////////////////////////////////////////////
// experimentTurnFilm2RationalBezierApprox.cpp
// (modifying turnFilm2.cpp)
//
// Interaction:
// Initially the film is along the z = 1 plane.
// Press the right arrow key to turn the film towards the x = 1 plane.
// Press the left arrow key to turn the film back towards the z = 1 plane.
// Press space to see the rational Bezier curve.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////////

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
static int t = 0; // Angle of turn of "film".
static int showPolyCurve = 1;

// Fixed control points on the plane (film) z=1.
static float controlPoints[3][3] = 
{
	{1.0, -0.5, 1.0}, {1.0, 1.7, 1.0}, {1.5, -0.0, 1.0}
};

// Projected control points on the turned plane (film).
static float projectedControlPoints[3][3];

// Projected control points on the turned plane (film), plus weights.
static float weightedProjectedControlPoints[3][4];
// End globals.

// Routine to compute projected control points on the turned film.
void computeProjectedControlPoints(void)
{
   int i;
   for (i = 0; i < 3; i++)
   {
       float zValue = controlPoints[i][0] * sin( (float)t*PI/180.0 ) + 
		               controlPoints[i][2] * cos( (float)t*PI/180.0 );
       projectedControlPoints[i][0] = ( controlPoints[i][0] * cos( (float)t*PI/180.0 ) - 
		                              controlPoints[i][2] * sin( (float)t*PI/180.0 ) ) / zValue;
       projectedControlPoints[i][1] = controlPoints[i][1] / zValue;
	   projectedControlPoints[i][2] = 1.0;
   }
}

// Routine to compute weighted projected control points on the turned film.
void computeWeightedProjectedControlPoints(void)
{
   int i;
   for (i = 0; i < 3; i++)
   {
       float zValue = controlPoints[i][0] * sin( (float)t*PI/180.0 ) + 
		               controlPoints[i][2] * cos( (float)t*PI/180.0 );
       weightedProjectedControlPoints[i][0] = controlPoints[i][0] * cos( (float)t*PI/180.0 ) - 
		                                      controlPoints[i][2] * sin( (float)t*PI/180.0 );
       weightedProjectedControlPoints[i][1] = controlPoints[i][1];
	   weightedProjectedControlPoints[i][2] = zValue;
	   weightedProjectedControlPoints[i][3] = zValue;
   }
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   computeProjectedControlPoints();
   computeWeightedProjectedControlPoints();
}

// Drawing routine.
void drawScene()
{
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   // The red dashed curve and the red control points are fixed in world co-ordinates and viewed on the film turned
   // an angle of t from the z = 1 plane towards the x = 1 plane by the following gluLoookAt() command.
   glPushMatrix(); 
   gluLookAt(0.0, 0.0, 0.0, sin( (float)t*PI/180.0 ), 0.0, cos( (float)t*PI/180.0 ), 0.0, 1.0, 0.0);

   glEnable(GL_LINE_STIPPLE);
   glLineStipple(5, 0xAAAA);
   
   // Draw the red dashed polynomial Bezier curve approximating the fixed control points on z=1.
   glColor3f(1.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, controlPoints[0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(100, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 100);;
   glDisable(GL_MAP1_VERTEX_3);

   // Draw the control points as black dots.
   glColor3f(0.0, 0.0, 0.0);
   glPointSize(5.0);  
   glBegin(GL_POINTS);
   for (int i = 0; i < 3; i++) 
      glVertex3fv(controlPoints[i]);
   glEnd();

   glPopMatrix();

   // As the projected control points are computed by rotating them around the y-axis (see the routine
   // computeProjectedControlPoints() above), leaving the film fixed on z=1, the gluLookAt() command to
   // view them points the camera at the z=1 plane.
   glPushMatrix();
   gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);

   glLineStipple(5, 0x5555);

   if (showPolyCurve)
   // Draw the green dashed polynomial Bezier curve approximating the projections of the control points on the
   // turned film.
   {
      glColor3f(0.0, 1.0, 0.0);
      glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, projectedControlPoints[0]);
      glEnable(GL_MAP1_VERTEX_3);
      glMapGrid1f(100, 0.0, 1.0);
      glEvalMesh1(GL_LINE, 0, 100);
      glDisable(GL_MAP1_VERTEX_3);
   }

   else
   // Draw the blue dashed rational Bezier curve approximating the projected control points on the
   // turned film with weights calculated using the original curve.
   {
      glColor3f(0.0, 0.0, 1.0);
      glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 3, weightedProjectedControlPoints[0]);
      glEnable(GL_MAP1_VERTEX_4);
      glMapGrid1f(100, 0.0, 1.0);
      glEvalMesh1(GL_LINE, 0, 100);
      glDisable(GL_MAP1_VERTEX_4);
   }
   glPopMatrix();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(120.0, (float)w/(float)h, 1.0, 1000.0);
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
         showPolyCurve = !showPolyCurve;
		 glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT) if (t < 90) t++;
 
   if (key == GLUT_KEY_LEFT) if (t > 0) t--;
   
   computeProjectedControlPoints();
   computeWeightedProjectedControlPoints();
   
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Initially the film is along the z = 1 plane." << endl
        << "Press the right arrow key to turn the film towards the x = 1 plane." << endl
        << "Press the left arrow key to turn the film back towards the z = 1 plane." << endl
		<< "Press space to see the rational Bezier curve." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("experimentTurnFilm2RationalBezierApprox.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
