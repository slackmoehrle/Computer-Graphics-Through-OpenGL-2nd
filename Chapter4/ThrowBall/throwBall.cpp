///////////////////////////////////////////////////////////////////////////////////      
// throwBall.cpp
//
// This program shows the motion of a ball subject to gravity. The gravitational
// acceleration and initial velocity of the ball are changeable.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press right/left arrow kes to increase/decrease the initial horizontal velocity.
// Press up/down arrow keys to increase/decrease the initial vertical velocity.
// Press page up/down keys to increase/decrease gravitational acceleration.
// Press r to reset."
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////

#include <cmath>
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

#define PI 3.14159265

using namespace std;

// Globals.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static float t = 0.0; // Time parameter.
static float h = 0.5; // Horizontal component of initial velocity.
static float v = 4.0; // Vertical component of initial velocity.
static float g = 0.2;  // Gravitational accelaration.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to onvert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
}

// Write data.
void writeData(void)
{
   glColor3f(0.0, 0.0, 0.0);
   
   floatToString(theStringBuffer, 4, h);
   glRasterPos3f(-4.5, 4.5, -5.1);
   writeBitmapString((void*)font, "Horizontal component of initial velocity: ");
   writeBitmapString((void*)font, theStringBuffer);
   
   floatToString(theStringBuffer, 4, v);
   glRasterPos3f(-4.5, 4.2, -5.1);
   writeBitmapString((void*)font, "Vertical component of initial velocity: ");  
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, g);
   glRasterPos3f(-4.5, 3.9, -5.1);
   writeBitmapString((void*)font, "Gravitation: ");  
   writeBitmapString((void*)font, theStringBuffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   writeData();

   // Place scene in frustum.
   glTranslatef(-15.0, -15.0, -25.0);

   // Apply equations of motion to transform sphere.
   glTranslatef(h*t, v*t - (g/2.0)*t*t, 0.0);

   // Sphere.
   glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(2.0, 10, 10);

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      t += 1.0;
	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
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
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
		 }
		 break;
	  case 'r':
         isAnimate = 0;
		 t = 0.0;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) v += 0.05;;
   if(key == GLUT_KEY_DOWN) if (v > 0.1) v -= 0.05;
   if(key == GLUT_KEY_RIGHT) h += 0.05; 
   if(key == GLUT_KEY_LEFT) if (h > 0.1) h -= 0.05;  
   if(key == GLUT_KEY_PAGE_UP) g += 0.05;  
   if(key == GLUT_KEY_PAGE_DOWN) if (g > 0.1) g -= 0.05;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
        << "Press right/left arrow kes to increase/decrease the initial horizontal velocity." << endl
        << "Press up/down arrow keys to increase/decrease the initial vertical velocity." << endl
        << "Press page up/down keys to increase/decrease gravitational acceleration." << endl
        << "Press r to reset." << endl;
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
   glutCreateWindow("throwBall.cpp"); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop(); 
}

