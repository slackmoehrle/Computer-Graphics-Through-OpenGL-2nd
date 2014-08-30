/////////////////////////////////////////////////////////////////////////////////
// invalidTriangulation.cpp
//
// This program shows an invalid triangulation of a rectangle by three triangles.
//
// Interaction:
// Press the space bar to toggle between the two different orders of drawing 
// triangles ABC and DBC.
//
// Sumanta Guha
/////////////////////////////////////////////////////////////////////////////////

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
static int isABCFirst = 1; // Is triangle ABC drawn first?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Successive vertex co-ordinate vectors.
static float vertices[5][3] =  
{	
    {20.0, 80.0, 0.0}, // A
	{20.0, 20.0, 0.0}, // B
    {80.0, 20.0, 0.0}, // C
    {80.0, 80.0, 0.0}, // D
    {50.0, 50.0, 0.0}  // E
};

// Successive vertex color vectors.
static float colors[5][3]
 =  
{
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0}
};
// End globals.

// Function to draw triangle ABC
void drawABC(void)
{
   glBegin(GL_TRIANGLES);
      glColor3fv(colors[0]);
      glVertex3fv(vertices[0]);
      glColor3fv(colors[1]);
      glVertex3fv(vertices[1]);
      glColor3fv(colors[2]);
      glVertex3fv(vertices[2]);
	glEnd();
}

// Function to draw triangle DBC
void drawDBC(void)
{
   glBegin(GL_TRIANGLES);
      glColor3fv(colors[3]);
      glVertex3fv(vertices[3]);
      glColor3fv(colors[1]);
      glVertex3fv(vertices[1]);
      glColor3fv(colors[2]);
      glVertex3fv(vertices[2]);
	glEnd();
}

// Function to draw triangle DAE
void drawDAE(void)
{
   glBegin(GL_TRIANGLES);
      glColor3fv(colors[3]);
      glVertex3fv(vertices[3]);
      glColor3fv(colors[0]);
      glVertex3fv(vertices[0]);
      glColor3fv(colors[4]);
      glVertex3fv(vertices[4]);
	glEnd();
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);

   if (isABCFirst)
   {
      drawABC();
      drawDBC();
	  glColor3f(0.0, 0.0, 0.0);
	  glRasterPos3f(10.0, 90.0, 0.0);
      writeBitmapString((void*)font, "ABC drawn first in code, DBC next!");
   }
   else
   {
      drawDBC();
      drawABC();
	  glColor3f(0.0, 0.0, 0.0);
	  glRasterPos3f(10.0, 90.0, 0.0);
      writeBitmapString((void*)font, "DBC drawn first in code, ABC next!");
   }
   drawDAE();

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
         if (isABCFirst == 0) isABCFirst = 1;
         else isABCFirst = 0;
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
   cout << "Press the space bar to toggle between the two different orders of drawing triangles ABC and DBC." << endl;
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
   glutCreateWindow("invalidTriangulation.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}

