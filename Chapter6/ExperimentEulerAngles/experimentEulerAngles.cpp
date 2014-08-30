////////////////////////////////////////////////////////////////////////////////////       
// experimentEulerAngles.cpp
// (modifying eulerAngles.cpp)
//
// Interaction:
// Press the x, X, y, Y, z, Z to rotate the white L about the axes.
// Press delete to reset.
//
//Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

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
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Euler angles.
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

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
	sprintf(destStr,"%f",val);
	destStr[precision] = '\0';
}

// Routine to write the Euler angles.
void writeData(void)
{
   char buffer [33];
   
   glColor3f(0.0, 0.0, 0.0);
   
   floatToString(buffer, 4, Xangle);	
   glRasterPos3f(-1.0, 0.9, -2.0);
   writeBitmapString((void*)font1, "alpha = ");
   writeBitmapString((void*)font1, buffer);

   floatToString(buffer, 4, Yangle);	
   glRasterPos3f(-1.0, 0.8, -2.0);
   writeBitmapString((void*)font1, "beta = ");
   writeBitmapString((void*)font1, buffer);

   floatToString(buffer, 4, Zangle);	
   glRasterPos3f(-1.0, 0.7, -2.0);
   writeBitmapString((void*)font1, "gamma = ");
   writeBitmapString((void*)font1, buffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   // Write the current Euler angles.
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

   // Change Euler angles.
   glRotatef(Xangle, 1.0, 0.0, 0.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Zangle, 0.0, 0.0, 1.0);

   // Draw blue L.
   glColor3f(0.0, 0.0, 1.0);
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
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 180.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle <= -180.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 180.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle <= -180.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 180.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle <= -180.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      case 127:
         Zangle = Yangle = Xangle = 0.0;
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
   cout << "Press the x, X, y, Y, z, Z to rotate the white L about the axes." << endl
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
   glutCreateWindow("experimentEulerAngles.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
