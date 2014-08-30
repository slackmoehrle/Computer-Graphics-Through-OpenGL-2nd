/////////////////////////////////////////////////////////////////////////////////////         
// multipleLists.cpp
//
// This program demonstrates the definition and execution of multiple  display lists.  
// 
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////////

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
static unsigned int base; // Display lists base index.
static unsigned int offsets[6] = {0, 2, 1, 1, 0, 2}; // Array of display list offsets.

// Initialization routine.
static void setup(void)
{
   // Return the first of an available block of three successive  list indexes - 
   //to be used as base value.
   base = glGenLists(3); 

   // Specify the base to be used in subsequent display list calls.
   glListBase(base); 

   // Create a display list offset 0 from the base value.
   glNewList(base, GL_COMPILE);
      // Red triangle.
      glColor3f (1.0, 0.0, 0.0); 
      glBegin(GL_TRIANGLES);
      glVertex2f(10.0, 10.0);
      glVertex2f(20.0, 10.0);
      glVertex2f(20.0, 40.0);
      glEnd();
      glTranslatef(15.0, 0.0, 0.0); 
   glEndList();

   // Create a display list offset 1 from the base value.
   glNewList(base + 1, GL_COMPILE);
      // Green  rectangle.
      glColor3f(0.0, 1.0, 0.0); 
      glBegin(GL_POLYGON);
      glVertex2f(10.0, 10.0);
      glVertex2f(20.0, 10.0);
      glVertex2f(20.0, 40.0);
      glVertex2f(10.0, 40.0);
      glEnd();
      glTranslatef(15.0, 0.0, 0.0); 
   glEndList();

   // Create a display list offset 2 from the base value.
   glNewList(base + 2, GL_COMPILE);
      // Blue pentagon.
      glColor3f(0.0, 0.0, 1.0);  
      glBegin(GL_POLYGON);
      glVertex2f(10.0, 10.0);
      glVertex2f(20.0, 10.0);
      glVertex2f(20.0, 20.0);
      glVertex2f(15.0, 40.0);
      glVertex2f(10.0, 20.0);
      glEnd();
      glTranslatef(15.0, 0.0, 0.0); 
   glEndList();

   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Drawing routine.
void drawScene(void)
{ 
   glClear(GL_COLOR_BUFFER_BIT); 

   glLoadIdentity();

   // Execute 6 display lists whose offsets from the base 
   // are in the array offsets.
   glCallLists(6, GL_INT, offsets); 

   glFlush();
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
   glutCreateWindow("multipleLists.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
