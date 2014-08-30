///////////////////////////////////////////////////////////        
// helixList.cpp
//
// This program draws several helixes using a display list.
// 
// Sumanta Guha.
///////////////////////////////////////////////////////////

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

#define PI 3.14159265

using namespace std;

// Globals.
static unsigned int aHelix; // List index.

// Initialization routine.
void setup(void) 
{
   float t; // Angle parameter.

   aHelix = glGenLists(1); // Return a list index.

   // Begin create a display list.
   glNewList(aHelix, GL_COMPILE);

   // Draw a helix.
   glBegin(GL_LINE_STRIP);
   for(t = -10 * PI; t <= 10 * PI; t += PI/20.0)
      glVertex3f(20 * cos(t), 20 * sin(t), t);
   glEnd();
   
   glEndList();
   // End create a display list.

   glClearColor(1.0, 1.0, 1.0, 0.0);  
}

// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   
   glColor3f(1.0, 0.0, 0.0);
   glPushMatrix();
   glTranslatef(0.0, 0.0, -70.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();
   
   glColor3f(0.0, 1.0, 0.0);
   glPushMatrix();
   glTranslatef(30.0, 0.0, -70.0);
   glScalef(0.5, 0.5, 0.5);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();
   
   glColor3f(0.0, 0.0, 1.0);
   glPushMatrix();
   glTranslatef(-25.0, 0.0, -70.0);
   glRotatef(90.0, 0.0, 1.0, 0.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(1.0, 1.0, 0.0);
   glPushMatrix();
   glTranslatef(0.0, -20.0, -70.0);
   glRotatef(90.0, 0.0, 0.0, 1.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();
   
   glColor3f(1.0, 0.0, 1.0);
   glPushMatrix();
   glTranslatef(-40.0, 40.0, -70.0);
   glScalef(0.5, 0.5, 0.5);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(0.0, 1.0, 1.0);
   glPushMatrix();
   glTranslatef(30.0, 30.0, -70.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
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
   glutCreateWindow("helixList.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop(); 
}
