///////////////////////////////////////////////////////////      
// relativePlacement.cpp
//
// This program shows composition of transformations and 
// the relative placement of one object w.r.t another.
//
// Interaction:
// Press the up/down arrow keys to process code statements.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////

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
static unsigned int base; // Display lists base index.
static int numVal = 0; // Step index.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

// Draw stick figure with a local co-ordinate system.
void drawMan(void)
{   
   float angle;
   int i;

   glLineWidth(2.0);
   glBegin(GL_LINE_LOOP);
      for(i = 0; i < 20; ++i)
	  {
         angle = 2 * PI * i / 20;
         glVertex2f(0.0 + cos(angle) * 3.0, 7 + sin(angle) * 3.0);
	  }
   glEnd();
   glBegin(GL_LINES);
      glVertex2f(0.0, 4.0);
	  glVertex2f(0.0, -4.0);
	  glVertex2f(0.0, -4.0);
	  glVertex2f(6.0, -10.0);
	  glVertex2f(0.0, -4.0);
	  glVertex2f(-6.0, -10.0);
	  glVertex2f(-6.0, 0.0);
	  glVertex2f(6.0, 0.0);
   glEnd();
   glLineWidth(1.0);

   glRasterPos3f(0.0, 0.0, 0.0);
   writeBitmapString((void*)font, "O");
   glRasterPos3f(7.0, 0.0, 0.0);
   writeBitmapString((void*)font, "x");
   glRasterPos3f(-1.0, 6.0, 0.0);
   writeBitmapString((void*)font, "y");
}

// Draw local co-ordinates grid.
void drawGrid(void)
{
   int i;
   glEnable(GL_LINE_STIPPLE); // Enable line stippling.
   glLineStipple(1, 0x00FF);
   glBegin(GL_LINES);
      for(i = -5; i < 6; ++i)
	  {
	     glVertex2f(5*i, 25.0);
	     glVertex2f(5*i,-25.0);
	  }
      for(i = -5; i < 6; ++i)
	  {
	     glVertex2f(25.0, 5*i);
	     glVertex2f(-25.0, 5*i);
	  }
   glEnd();
   glDisable(GL_LINE_STIPPLE); // Disable line stippling.
}

// Draw and label world co-ordinate axes.
void drawWorldAxes(void)
{
   glColor3f(0.0, 1.0, 1.0);
   glBegin(GL_LINES);
      glVertex2f(-50.0, 0.0);
      glVertex2f(50.0, 0.0);
      glVertex2f(0.0, -50.0);
      glVertex2f(0.0, 50.0);
   glEnd();

   glRasterPos3f(48.0, -2.0, 0.0);
   writeBitmapString((void*)font, "x");

   glRasterPos3f(1.0, 48.0, 0.0);
   writeBitmapString((void*)font, "y");
}

// Write fixed messages.
void writeFixedMessages(void)
{
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(-15.0, 43.0, 0.0);
   writeBitmapString((void*)font, "Press the up/down arrow keys!");

   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(-44.0, -17.0, 0.0);
   writeBitmapString((void*)font, "glScalef(1.5, 0.75, 1.0);");
   glRasterPos3f(-44.0, -20.0, 0.0);
   writeBitmapString((void*)font, "glRotatef(30.0, 0.0, 0.0, 1.0);");
   glRasterPos3f(-44.0, -23.0, 0.0);
   writeBitmapString((void*)font, "glTranslatef(10.0, 0.0, 0.0);");
   glRasterPos3f(-44.0, -26.0, 0.0);
   writeBitmapString((void*)font, "drawRedMan; // Also draw grid in his local co-ordinate system.");
   glRasterPos3f(-44.0, -29.0, 0.0);
   writeBitmapString((void*)font, "glRotatef(45.0, 0.0, 0.0, 1.0);");
   glRasterPos3f(-44.0, -32.0, 0.0);
   writeBitmapString((void*)font, "glTranslatef(20.0, 0.0, 0.0);");
   glRasterPos3f(-44.0, -35.0, 0.0);
   writeBitmapString((void*)font, "drawBlueMan;");
}

// Drawing routine.
void drawScene(void)
{   
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   writeFixedMessages();
   drawWorldAxes();

   glColor3f(0.0, 0.0, 0.0);
   switch(numVal) 
   {
      case 0:
         goto step0;
         break;
      case 1:
	     goto step1;
         break;
      case 2:
	     goto step2;
         break;
      case 3:
	     goto step3;
         break;
      case 4:
	     goto step4;
         break;
      case 5:
	     goto step5;
         break;
      case 6:
	     goto step6;
         break;
      case 7:
	     goto step7;
         break;
      default:
         break;
   }
   // Transformation steps.
   // Text drawing statements are enclosed within push/pop pairs
   // so that the raster position is w.r.t the identity transform.
step7:
   // Scale. 
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -17.0, 0.0);
   writeBitmapString((void*)font, "glScalef(1.5, 0.75, 1.0);");
   glPopMatrix();
   glScalef(1.5, 0.75, 1.0);

step6:
   // Rotate. 
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -20.0, 0.0);
   writeBitmapString((void*)font, "glRotatef(30.0, 0.0, 0.0, 1.0);");
   glPopMatrix();
   glRotatef(30, 0.0, 0.0, 1.0);

step5:
   // Translate.
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -23.0, 0.0);
   writeBitmapString((void*)font, "glTranslatef(10.0, 0.0, 0.0);");
   glPopMatrix();
   glTranslatef(10.0, 0.0, 0.0);

step4:
   // Draw red man.
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -26.0, 0.0);
   writeBitmapString((void*)font, "drawRedMan; // Also draw grid in his local co-ordinate system.");
   glPopMatrix();
   glColor3f(1.0, 0.0, 0.0);
   glCallList(base);
   glCallList(base + 1);
   glColor3f(0.0, 0.0, 0.0);

step3:
   // Rotate.
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -29.0, 0.0);
   writeBitmapString((void*)font, "glRotatef(45.0, 0.0, 0.0, 1.0);");
   glPopMatrix();
   glRotatef(45, 0.0, 0.0, 1.0);

step2:
   // Translate.
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -32.0, 0.0);
   writeBitmapString((void*)font, "glTranslatef(20.0, 0.0, 0.0);");
   glPopMatrix();
   glTranslatef(20.0, 0.0, 0.0);

step1:
   // Draw blue man.
   glPushMatrix();
   glLoadIdentity();
   glRasterPos3f(-44.0, -35.0, 0.0);
   writeBitmapString((void*)font, "drawBlueMan;");
   glPopMatrix();
   glColor3f(0.0, 0.0, 1.0);
   glCallList(base);

step0:
   glFlush();
}

// Initialization routine.
void setup(void)
{
   base = glGenLists(2);

   glNewList(base, GL_COMPILE);
   drawMan();
   glEndList();

   glNewList(base+1, GL_COMPILE);
   drawGrid();
   glEndList();

   glClearColor (0.8, 0.8, 0.8, 1.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
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
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) 
   {
      if (numVal < 7) numVal++; else numVal = 0;
   }
   if(key == GLUT_KEY_DOWN)
   {
      if (numVal > 0) numVal--; else numVal = 7;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up/down arrow keys to process code statements." << endl;  
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
   glutCreateWindow("relativePlacement.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
