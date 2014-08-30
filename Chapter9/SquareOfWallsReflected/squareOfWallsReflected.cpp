//////////////////////////////////////////////////////////////////////////////////////        
// squareOfWallsReflected.cpp
//
// This program, based on squareOfWalls.cpp, draws a square of flat walls in 3D space,
// one side of which is in outline and the other is filled. The scene can be reflected
// about the yz-plane.
//
// Interaction: 
// Press space to reflect the scene about the yz-plane.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

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

// Globals.
static int isReflected = 0; // Is there reflection?
static char theStringBuffer[10]; // String buffer.
static int font=(int)GLUT_BITMAP_8_BY_13; // Font selection.

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
   glLoadIdentity();

   glColor3f(0.0, 0.0, 0.0);

   gluLookAt(0.0, 7.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Front faces filled, back faces outlined.
   glPolygonMode(GL_FRONT, GL_FILL);
   glPolygonMode(GL_BACK, GL_LINE);

   // Block to reflect the scene about the yz-plane.
   if (isReflected)
   {
      glRasterPos3f(-4.5, 4.5, -5.1);
      writeBitmapString((void*)font, "Reflected!");
      glScalef(-1.0, 1.0, 1.0);
	  glFrontFace(GL_CW);
   }
   else
   {
      glRasterPos3f(-4.5, 4.5, -5.1);
      writeBitmapString((void*)font, "Original, not reflected!");
	  glFrontFace(GL_CCW);
   }

   // Draw walls as a single triangle strip.
   glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(-5.0, 1.0, 5.0);
      glVertex3f(-5.0, -1.0, 5.0);
      glVertex3f(5.0, 1.0, 5.0);
      glVertex3f(5.0, -1.0, 5.0);
      glVertex3f(5.0, 1.0, -5.0);
      glVertex3f(5.0, -1.0, -5.0);
      glVertex3f(-5.0, 1.0, -5.0);
      glVertex3f(-5.0, -1.0, -5.0);
      glVertex3f(-5.0, 1.0, 5.0);
      glVertex3f(-5.0, -1.0, 5.0);
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
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case ' ':
         if (isReflected) isReflected = 0;
		 else isReflected = 1;
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
   cout << "Press space to reflect the scene about the yz-plane." << endl;
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
   glutCreateWindow("squareOfWallsReflected.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
