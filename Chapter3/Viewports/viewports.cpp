/////////////////////////////////////////////////////////         
// viewports.cpp
//
// This program shows two viewports in one OpenGL window.
//
// Sumanta Guha.
///////////////////////////////////////////////////////// 

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
static int width, height; // OpenGL window size.

// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);

   // Define first viewport.
   glViewport(0, 0, width/2.0,  height); 

   // Begin contents of first viewport.
   // A red square.
   glColor3f(1.0, 0.0, 0.0);	
   glBegin(GL_POLYGON);
      glVertex3f(10.0, 10.0, 0.0);
      glVertex3f(40.0, 10.0, 0.0);
      glVertex3f(40.0, 40.0, 0.0);
      glVertex3f(10.0, 40.0, 0.0);
   glEnd();
   // End contents of first viewport.

   // Define second viewport.
   glViewport(width/2.0, 0, width/2.0, height);

   // Begin contents of second viewport.
   // A blue circle.
   glColor3f(0.0, 0.0, 1.0);
   float angle;
   int i;
   glBegin(GL_LINE_LOOP);
   for(i = 0; i < 30; ++i)
   {
      angle = 2 * PI * i / 30;
      glVertex2f(50.0 + cos(angle) * 40.0, 50.0 + sin(angle) * 40.0);
   }
   glEnd();

   // A vertical black line at the left to separate the viewports.
   glColor3f(0.0, 0.0, 0.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(0.1, 0.0, 0.0);
	  glVertex3f(0.1, 100.0, 0.0);
   glEnd();
   glLineWidth(1.0);
   // End contents of second viewport.

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
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Pass the size of the OpenGL window to globals.
   width = w;
   height = h;
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

// Main routine.
int main(int argc, char **argv) 
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3); 
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("viewports.cpp");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE; 
	glewInit(); 

	setup(); 

	glutMainLoop(); 
}

