///////////////////////////////////////////////////
// lineStipple.cpp
//
// This program draws a line with various stipples.
//
// Interaction:
// Press the space bar to cycle between stipples.
// 
// Sumanta Guha
///////////////////////////////////////////////////

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
static int stippleID = 0; // Stipple ID.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   glEnable(GL_LINE_STIPPLE); // Enable line stippling.

   // Speficy the stipple pattern and write a label.
   glRasterPos3f(30.0, 40.0, 0.0);
   switch(stippleID) 
   {
      case 0: 
         glDisable(GL_LINE_STIPPLE);
		 writeBitmapString((void*)font, "No stipple");
         break;	
      case 1: 
		 glLineStipple(1, 0x5555); 
		 writeBitmapString((void*)font, "glLineStipple(1, 0x5555)");
         break;	  
      case 2:
		 glLineStipple(1, 0x0101);
		 writeBitmapString((void*)font, "glLineStipple(1, 0x0101)");
         break;
      case 3: 
		 glLineStipple(1, 0x00FF); 
		 writeBitmapString((void*)font, "glLineStipple(1, 0x00FF)");
         break;	  
      case 4:
		 glLineStipple(5, 0x5555);
		 writeBitmapString((void*)font, "glLineStipple(5, 0x5555)");
         break;
      default:
         break;
   }

   // Draw one straight line segment.
   glBegin(GL_LINES);
      glVertex3f(25.0, 50.0, 0.0);
	  glVertex3f(75.0, 50.0, 0.0);
   glEnd();

   glDisable(GL_LINE_STIPPLE); // Disable line stippling.

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
         if (stippleID < 4) stippleID++;
         else stippleID = 0;
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
   cout << "Press the space bar to cycle between stipples." << endl;
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
   glutCreateWindow("lineStipple.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}