/////////////////////////////////////////////////////////////////////////////////       
// interpolateEulerAngles.cpp
//
// This program draws an L that is rotated from one configuration to another by
// interpolating between the corresponding Euler angle values. Two different sets
// of values are used to represent one end configuration and the program shows
// the correpsondingly distinct paths - one with a yellow L, one with a white L.
//
// Interaction:
// Press the left and right arrow keys to step through the paths.
// Press delete to reset.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////  

#include <cstdlib>
#include <iostream>
#include <fstream>

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
static float angle = 0.0; // Angle value.
static long font1 = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static long font2 = (long)GLUT_STROKE_ROMAN; // Font selection.

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Routine to convert integer to char string.
void intToString(char * destStr, int val) 
{
	sprintf(destStr,"%d",val);
}

// Routine to write the intermediate angle value.
void writeData(void)
{
   char buffer [33];
   
   glColor3f(0.0, 0.0, 0.0);
	
   intToString(buffer, angle);
   glRasterPos3f(-1.0, 0.9, -2.0);
   writeBitmapString((void*)font1, "angle = ");
   writeBitmapString((void*)font1, buffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   // Write the intermediate angle values.
   writeData();

   gluLookAt(5.0, 5.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Draw the co-ordinate axes.
   glLineWidth(2.0);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
	   glVertex3f(-6.0, 0.0, 0.0);
       glVertex3f(6.0, 0.0, 0.0);
       glVertex3f(5.75, 0.0, 0.25);
       glVertex3f(6.0, 0.0, 0.0);
       glVertex3f(5.75, 0.0, -0.25);
       glVertex3f(6.0, 0.0, 0.0);

	   glVertex3f(0.0, -6.0, 0.0);
       glVertex3f(0.0, 6.0, 0.0);
       glVertex3f(0.25, 5.75, 0.0);
       glVertex3f(0.0, 6.0, 0.0);
       glVertex3f(-0.25, 5.75, 0.0);
       glVertex3f(0.0, 6.0, 0.0);

	   glVertex3f(0.0, 0.0, -6.0);
       glVertex3f(0.0, 0.0, 6.0);
	   glVertex3f(0.25, 0.0, 5.75);
	   glVertex3f(0.0, 0.0, 6.0);
	   glVertex3f(-0.25, 0.0, 5.75);
	   glVertex3f(0.0, 0.0, 6.0);
   glEnd();
   glLineWidth(1.0);

   // Label the co-ordinate axes.
   glPushMatrix();
   glTranslatef(6.1, 0.0, 0.0);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString((void*)font2, "X");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 6.1, 0.0);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString((void*)font2, "Y");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 0.0, 6.1);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString((void*)font2, "Z");
   glPopMatrix();

   glPushMatrix();
   
   // Interpolate the x Euler angles from 0 to -90 and the y and z Euler angles from 0 to 90.
   glRotatef(-1*angle, 1.0, 0.0, 0.0);
   glRotatef(angle, 0.0, 1.0, 0.0);
   glRotatef(angle, 0.0, 0.0, 1.0);

   // Draw red L. 
   glColor3f(1.0, 0.0, 0.0);
   glLineWidth(3.0);
   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, 0.0);
	  glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.5, 2.0);
   glEnd();
   glLineWidth(1.0);

   glPopMatrix();

   glPushMatrix();

   // Interpolate only the y Euler angle from 0 to 90.
   glRotatef(angle, 0.0, 1.0, 0.0);

   // Draw green L.
   glColor3f(0.0, 1.0, 0.0);
   glLineWidth(3.0);
   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, 0.0);
	  glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.5, 2.0);
   glEnd();
   glLineWidth(1.0);
   
   glPopMatrix();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
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
	  case 127:
		 angle = 0.0;
		 glutPostRedisplay();
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT) if (angle > 0.0) angle -= 5.0;
   if (key == GLUT_KEY_RIGHT) if (angle < 90.0) angle += 5.0;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left and right arrow keys to step through the paths." << endl
	    << "Press delete to reset." << endl;
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
   glutCreateWindow("interpolateEulerAngles.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
