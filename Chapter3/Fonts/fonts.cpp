//////////////////////////////////////////////////////////////////////////    
// fonts.cpp
// 
// This program displays various fonts available through the GLUT library.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////

#include <cstdlib>
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

using namespace std;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
  
// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);

   glRasterPos3f(10.0, 90.0, 0.0);
   writeBitmapString(GLUT_BITMAP_8_BY_13, "GLUT_BITMAP_8_BY_13");

   glRasterPos3f(10.0, 85.0, 0.0);
   writeBitmapString(GLUT_BITMAP_9_BY_15, "GLUT_BITMAP_9_BY_15");

   glRasterPos3f(10.0, 80.0, 0.0);
   writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, "GLUT_BITMAP_TIMES_ROMAN_10");

   glRasterPos3f(10.0, 70.0, 0.0);
   writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "GLUT_BITMAP_TIMES_ROMAN_24");

   glRasterPos3f(10.0, 60.0, 0.0);
   writeBitmapString(GLUT_BITMAP_HELVETICA_10, "GLUT_BITMAP_HELVETICA_10");

   glRasterPos3f(10.0, 55.0, 0.0);
   writeBitmapString(GLUT_BITMAP_HELVETICA_12, "GLUT_BITMAP_HELVETICA_12");

   glRasterPos3f(10.0, 45.0, 0.0);
   writeBitmapString(GLUT_BITMAP_HELVETICA_18, "GLUT_BITMAP_HELVETICA_18");

   glPushMatrix();
   glTranslatef(10.0, 10.0, 0.0);
   glRotatef(45.0, 0.0, 0.0, 1.0);
   glScalef(0.025, 0.025, 0.025);
   writeStrokeString(GLUT_STROKE_ROMAN, "GLUT_STROKE_ROMAN");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(40.0, 40.0, 0.0);
   glRotatef(-30.0, 0.0, 0.0, 1.0);
   glScalef(0.025, 0.025, 0.025);
   writeStrokeString(GLUT_STROKE_MONO_ROMAN, "GLUT_STROKE_MONO_ROMAN");
   glPopMatrix();

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

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("fonts.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
