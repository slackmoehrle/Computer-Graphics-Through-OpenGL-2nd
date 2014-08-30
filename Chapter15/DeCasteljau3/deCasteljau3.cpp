//////////////////////////////////////////////////////////////////////////
// deCasteljau3.cpp
//
// This program illustrates de Casteljau's algorithm to create a quadratic
// Bezier curve approximating 3 control points.
//
// Interaction: 
// Press the left/right arrows to decrease/increase the curve parameter u. 
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////

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

// Begin globals.
static float u = 0.0; // Curve parameter.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Control points.
static float ctrlpoints[3][3] = 
{
	{ -40.0, -20.0, 0.0}, { 0.0, 40.0, 0.0}, {40.0, -20.0, 0.0}
};
// End globals.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
} 

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);

   // Write u value.
   glColor3f(0.0, 0.0, 0.0);
   floatToString(theStringBuffer, 4, u);
   glRasterPos3f(-40.0, 40.0, 0.0);
   writeBitmapString((void*)font, "u = ");
   writeBitmapString((void*)font, theStringBuffer);

   // The Bezier curve is drawn from 0 to parameter value.
   glColor3f(1.0, 0.0, 1.0);
   glLineWidth(2.0);
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(100, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, (int)(u*100));
   glLineWidth(1.0);

   // The control points as dots.
   glPointSize(5.0);
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 3; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();

   // The lines joining the control points.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i < 3; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();

   // The point interpolating between the first two control points.
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 2, &ctrlpoints[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glEvalCoord1f(u);
   glEnd();

   // The point interpolating between the last two control points.
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 2, &ctrlpoints[1][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      glEvalCoord1f(u);
   glEnd();

   // The line joining the two points drawn above.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f( (1-u) * ctrlpoints[0][0] + u * ctrlpoints[1][0],
		          (1-u) * ctrlpoints[0][1] + u * ctrlpoints[1][1],
				   0.0 );
      glVertex3f( (1-u) * ctrlpoints[1][0] + u * ctrlpoints[2][0],
		          (1-u) * ctrlpoints[1][1] + u * ctrlpoints[2][1],
				   0.0 );
   glEnd();

   // The point interpolating between the two points drawn above.
   glColor3f(1.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_POINTS);
      glEvalCoord1f(u);
   glEnd();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
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
	  default:
	     break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_LEFT)
      if ( u > 0.02 ) u-= 0.02;
   if(key == GLUT_KEY_RIGHT)
      if ( u < 0.98 ) u += 0.02;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left/right arrows to decrease/increase the curve parameter u."<< endl;
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
   glutCreateWindow("deCasteljau3.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
