//////////////////////////////////////////////////////////////////////////////       
// parabola.cpp
//
// This program draws a line strip with vertices on a parabola.
// Increasing the number of vertices makes the strip approximate the parabola.
//
// Interaction:
// Press +/- to increase/decrease the number of vertices of the strip.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////   

#include <cstdlib>
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

using namespace std;

// Globals.
static int M = 3; // Number of vertices on one half of the parabola.

// Drawing routine.
void drawScene(void)
{
   int i;
   glClear (GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);
   
   // The 2M+1 vertices on the parabola are equally spaced in x-coordinates.
   // The parabola is translated and scaled to center it in the window.
   glBegin(GL_LINE_STRIP);
   for(i = -M; i <= M; ++i)
   { 
      glVertex3f( 50.0 + 50.0 * (float)i/M, 100.0 * (float)(i*i)/(M*M), 0.0 );
   }
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
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      case '+':
         M++;
         glutPostRedisplay();
         break;
      case '-':
         if (M > 2) M--;
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
   cout << "Press +/- to increase/decrease the number of vertices on the parabola." << endl;  
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
   glutCreateWindow("parabola.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop();
}
