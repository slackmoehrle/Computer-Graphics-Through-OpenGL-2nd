//////////////////////////////////////////////////////////////////////////////
// bezierCurveTangent.cpp
// 
// This program illustrates how two Bezier curves meet smoothly if they share
// a tangent at the common endpoint - the tangent at an endpoint is given by 
// the line joining it to the adjacent control point.
// 
// Interaction:
// Press space to select a control point of the second Bezier curve. 
// Press the arrow keys to move the selected control point.
//
//  Sumanta Guha
//////////////////////////////////////////////////////////////////////////////

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

// Begin globals. 
static int numVal = 0;

// Control points for the first curve.
static float controlPoints1[4][3] = {
	{ -40.0, -40.0, 0.0}, { -30.0, 20.0, 0.0}, 
	{-10.0, 0.0, 0.0}, {0.0, 20.0, 0.0}};

// Control points for the second curve.
static float controlPoints2[4][3] = {
	{ 0.0, 10.0, 0.0}, { 20.0, 40.0, 0.0}, 
	{20.0, 10.0, 0.0}, {40.0, -10.0, 0.0}};
// End globals.

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);

   // Draw the first curve in green.
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, controlPoints1[0]);
   glEnable(GL_MAP1_VERTEX_3);
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
   for (i = 0; i <= 30; i++) 
      glEvalCoord1f( (float)i/30.0 );
   glEnd();

   // Draw the second curve in blue.
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, controlPoints2[0]);
   glEnable(GL_MAP1_VERTEX_3);
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++) 
         glEvalCoord1f( (float)i/30.0 );
   glEnd();

 
   glPointSize(5.0);
   glBegin(GL_POINTS);

   // Control points of the first curve.
   glColor3f(0.0, 1.0, 0.0);
   for (i = 0; i < 4; i++) 
      glVertex3fv(controlPoints1[i]);

   // Control points of the second curve.
   glColor3f(0.0, 0.0, 1.0);
   for (i = 0; i < 4; i++) 
      glVertex3fv(controlPoints2[i]);

   // Selected control point.
   glColor3f(1.0, 0.0, 0.0);
   glVertex3fv(controlPoints2[numVal]);     
   
   glEnd();

   // Control polygon of first curve.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
   for (i = 0; i < 4; i++) 
	  glVertex3fv(controlPoints1[i]);
   glEnd();

   // Control polygon of second curve.
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_LINE_STRIP);
   for (i = 0; i < 4; i++) 
	  glVertex3fv(controlPoints2[i]);
   glEnd();

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
	     if (numVal < 3) numVal++;
	     else numVal = 0;
         glutPostRedisplay();
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) controlPoints2[numVal][1] += 1.0;
   if(key == GLUT_KEY_DOWN) controlPoints2[numVal][1] -= 1.0;
   if(key == GLUT_KEY_LEFT) controlPoints2[numVal][0] -= 1.0;
   if(key == GLUT_KEY_RIGHT) controlPoints2[numVal][0] += 1.0;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to select a control point of the second Bezier curve." << endl
        << "Press the arrow keys to move the selected control point." << endl; 
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
   glutCreateWindow("bezierCurveTangent.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
