////////////////////////////////////////////////////////////////////////////////        
// experimentBlendRectanglesAuthentic.cpp
// (modifying blendRectangles2.cpp)
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////// 

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

// Function to draw a blue rectangle at z = 0.1.
void drawBlueRectangle(void)
{
   glColor4f(0.0, 0.0, 1.0, 0.5); // Translucent: alpha value = 0.5.
   glBegin(GL_POLYGON);
      glVertex3f(-40.0, -35.0, 0.1);
      glVertex3f(40.0, -35.0, 0.1);
      glVertex3f(40.0, -20.0, 0.1);
      glVertex3f(-40.0, -20.0, 0.1);
   glEnd();
}

// Function to draw a green rectangle at z = 0.3.
void drawGreenRectangle(void)
{
   glColor4f(0.0, 1.0, 0.0, 1.0); // Opaque: alpha value = 1.0.
   glBegin(GL_POLYGON);
      glVertex3f(30.0, 20.0, 0.3);
      glVertex3f(15.0, 35.0, 0.3);
      glVertex3f(-40.0, -20.0, 0.3);
      glVertex3f(-25.0, -35.0, 0.3);
   glEnd();
}

// Function to draw a red rectangle at z = 0.5.
void drawRedRectangle(void)
{
   glColor4f(1.0, 0.0, 0.0, 0.5); // Translucent: alpha value = 0.5.
   glBegin(GL_POLYGON);
      glVertex3f(30.0, 35.0, 0.5);
      glVertex3f(15.0, 35.0, 0.5);
      glVertex3f(15.0, -45.0, 0.5);
      glVertex3f(30.0, -45.0, 0.5);
   glEnd();
}

// Initialization routine.
void setup(void)
{  
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
   // gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);

   // Draw opaque objects, only one.
   drawGreenRectangle(); // Green rectangle second closest, opaque.

   glDepthMask(GL_FALSE); // Make depth buffer read-only.
   
   // Draw translucent objects.
   drawBlueRectangle(); // Blue rectangle farthest, translucent.
   drawRedRectangle(); // Red rectangle closest to viewer, translucent.
   
   /*
   // Draw translucent objects in different order.
   drawRedRectangle(); // Red rectangle closest to viewer, translucent.
   drawBlueRectangle(); // Blue rectangle farthest, translucent.
   */

   glDepthMask(GL_TRUE); // Make depth buffer writable.

   glFlush();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
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

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("experimentBlendRectanglesAuthentic.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
