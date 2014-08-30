///////////////////////////////////////////////////////////////////////////////////    
// turnFilm1.cpp
//
// This program draws two power lines as long white lines along y = 2, z = 2 and
// y = -2, z= 2, and simulates turning the film in front of the OpenGL camera from 
// the z = 1 plane to the x = 1 plane.
//
// Interaction:
// Initially the film is along the z = 1 plane.
// Press the right arrow key to turn the film towards the x = 1 plane.
// Press the left arrow key to turn the film back towards the z = 1 plane.
//
// Sumanta Guha.
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

// Globals.
static int t = 0; // Angle of turn of "film".

// Drawing routine.
void drawScene()
{
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   // Turn the film an angle of t from the z = 1 plane towards the x = 1 plane
   gluLookAt(0.0, 0.0, 0.0, sin( (float)t*PI/180.0 ), 0.0, cos( (float)t*PI/180.0 ), 0.0, 1.0, 0.0);
   
   glColor3f(0.0, 0.0, 0.0);

   glBegin(GL_LINES);
      glVertex3f(-1000.0, 2.0, 2.0);
      glVertex3f(1000.0, 2.0, 2.0);
      glVertex3f(-1000.0, -2.0, 2.0);
      glVertex3f(1000.0, -2.0, 2.0);
   glEnd();
   
   glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
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
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT) if (t < 90) t++;
   if (key == GLUT_KEY_LEFT) if (t > 0) t--;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Initially the film is along the z = 1 plane." << endl
        << "Press the right arrow key to turn the film towards the x = 1 plane." << endl
        << "Press the left arrow key to turn the film back towards the z = 1 plane." << endl;
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
   glutInitWindowPosition (100, 100);
   glutCreateWindow("turnFilm1.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}
