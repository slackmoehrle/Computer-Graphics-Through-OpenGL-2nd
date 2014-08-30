////////////////////////////////////////////////////////////////////////
// interpolation.cpp
//
// This program shows the weight of each vertex of a triangle on a 
// movable point inside it by means of a vertical bar. The color of 
// the movable point itself is interpolated from those of the triangle's 
// vertices.
//
// Interaction:
// Press the arrow keys to move the large point.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////////////

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
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static float point[3] = {65.0, 50.0, 0.0}; // Movable point.

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

   // Draw white triangle.
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_TRIANGLES);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);
   glEnd();

   // Draw the vertices of the triangle in different colors.
   glPointSize(6.0);
   glBegin(GL_POINTS);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(20.0, 20.0, 0.0);

      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);

	  glColor3f(0.0, 0.0, 1.0);
      glVertex3f(80.0, 80.0, 0.0);
   glEnd();

   // Draw a movable point with color interpolated from the vertex colors.
   glPointSize(8.0);
   glColor3f( (80.0-point[0])/60.0, (point[0]-point[1])/60.0, (point[1]-20.0)/60.0 );
   glBegin(GL_POINTS);
      glVertex3fv(point);     
   glEnd();

   // Draw lines to indicate weights: a line indicates the 
   // weight of the vertex of the same color.
   glLineWidth(2.0);
   glBegin(GL_LINES);
       glColor3f(1.0, 0.0, 0.0);
	   glVertex3f(7.0, 20.0, 0.0);
       glVertex3f(7.0, 100.0 - point[0], 0.0);

       glColor3f(0.0, 1.0, 0.0);
	   glVertex3f(12.0, 20.0, 0.0);
       glVertex3f(12.0, 20.0 + point[0] - point[1], 0.0);

       glColor3f(0.0, 0.0, 1.0);
	   glVertex3f(17.0, 20.0, 0.0);
       glVertex3f(17.0, point[1], 0.0);
   glEnd();

   // Write labels.
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(6.0, 15.0, 0.0);
   writeBitmapString((void*)font, "Weights");
   glRasterPos3f(1.0, 20.0, 0.0);
   writeBitmapString((void*)font, "0.0");
   glRasterPos3f(1.0, 50.0, 0.0);
   writeBitmapString((void*)font, "0.5");
   glRasterPos3f(1.0, 79.0, 0.0);
   writeBitmapString((void*)font, "1.0");

   glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 0.0, 0.0);
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
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) if (point[1] < point[0]) point[1] += 0.5f;
   if(key == GLUT_KEY_DOWN) if (point[1] > 20.0) point[1] -= 0.5f;
   if(key == GLUT_KEY_LEFT) if (point[1] < point[0]) point[0] -= 0.5f;
   if(key == GLUT_KEY_RIGHT) if (point[0] < 80.0) point[0] += 0.5f;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the arrow keys to move the large point." << endl;        
}   

// Main routine.
int main(int argc, char **argv) 
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3); 
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("interpolation.cpp");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE; 
	glewInit(); 

	setup(); 

	glutMainLoop(); 
}
