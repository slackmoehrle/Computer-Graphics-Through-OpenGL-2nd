//////////////////////////////////////////////////////////////////////
// squareAnnulus1.cpp
//
// This program draws a square annulus as a triangle strip with
// vertices specified using glVertex3f() and colors using glColor3f().
//
// Interaction:
// Press the space bar to toggle between wireframe and filled.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////

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
static int isWire = 0; // Is wireframe?

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Draw square annulus.
   glBegin(GL_TRIANGLE_STRIP);
      glColor3f(0.0, 0.0, 0.0);
      glVertex3f(30.0, 30.0, 0.0);
	  glColor3f(1.0, 0.0, 0.0);
      glVertex3f(10.0, 10.0, 0.0);
	  glColor3f(0.0, 1.0, 0.0);
      glVertex3f(70.0, 30.0, 0.0);
	  glColor3f(0.0, 0.0, 1.0);
      glVertex3f(90.0, 10.0, 0.0);
	  glColor3f(1.0, 1.0, 0.0);
      glVertex3f(70.0, 70.0, 0.0);
	  glColor3f(1.0, 0.0, 1.0);
      glVertex3f(90.0, 90.0, 0.0);
	  glColor3f(0.0, 1.0, 1.0);
      glVertex3f(30.0, 70.0, 0.0);
	  glColor3f(1.0, 0.0, 0.0);
      glVertex3f(10.0, 90.0, 0.0);
	  glColor3f(0.0, 0.0, 0.0);
      glVertex3f(30.0, 30.0, 0.0);
	  glColor3f(1.0, 0.0, 0.0);
      glVertex3f(10.0, 10.0, 0.0);
   glEnd();

   glFlush();
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
   cout << "Press the space bar to toggle between wireframe and filled." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("squareAnnulus1.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
 
}
