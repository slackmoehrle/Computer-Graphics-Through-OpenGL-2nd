/////////////////////////////////////////////////////////////////      
// DDA.cpp
//
// This program implements the DDA line rasterizer. The raster is
// simulated by the OpenGL window.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////// 

#include <cstdlib>
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

// Draw a pixel as a point.
void pickPixel(int x, int y)
{
   glBegin(GL_POINTS);
      glVertex2i(x, y);
   glEnd();
}

// Round a float to a nearest integer.
int round(float z)
{
	 int y;
     y = z > 0.0 ? int(z + 0.5) : int(z - 0.5);
	 return y;
}

// DDA line rasterizer.
void DDA(int i1, int j1, int i2, int j2) // Assume i2 > i1.
{
   float y = j1;
   float m = float(j2 - j1)/(i2 - i1); // Assume -1 <= m <= 1.
   for(int x = i1; x <= i2; x++)
   {
      pickPixel(x, round(y));
	  y += m;
   }
}

// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);

   DDA(100, 100, 300, 200);

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

   // The height/width of the ortho-box match that of the OpenGL window,
   // resulting in a pixel-to-pixel correspondence.
   gluOrtho2D(0.0, 500.0, 0.0, 500.0);
   
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
   glutCreateWindow("DDA.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
