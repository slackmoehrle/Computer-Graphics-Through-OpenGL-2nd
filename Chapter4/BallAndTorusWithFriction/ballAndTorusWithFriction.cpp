///////////////////////////////////////////////////////////////////
// ballAndTorusWithFriction.cpp
//
// This program modifies ballAndTorus.cpp to simulate the motion of
// the ball in a viscous medium subject to an accelarating force.
//
// Interaction:
// Keep space key pressed to apply force to the ball. Release to stop application.
// Press up/down arrow keys to increase/decrease the applied force.
// Press page up/down keys to increase/decrease the viscous drag.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////  

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
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int animationPeriod = 100; // Time interval between frames.
static float velocity = 0.0; // Velocity of ball.
static float drag = 0.005; // Drag co-efficient.
static float applied_acceleration = 0.02; // Acceleration (force) applied when up key is pressed.
float acceleration = 0.0; // Acceleration of ball.
static int isForce = 0; // Force being applied (= up key pressed)?
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
   
   floatToString(theStringBuffer, 5, applied_acceleration);
   glRasterPos3f(-4.5, 4.5, -5.1);
   writeBitmapString((void*)font, "Applied force: ");  
   writeBitmapString((void*)font, theStringBuffer);
   
   floatToString(theStringBuffer, 6, drag);
   glRasterPos3f(-4.5, 4.2, -5.1);
   writeBitmapString((void*)font, "Viscous drag: ");
   writeBitmapString((void*)font, theStringBuffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   writeData();

   glTranslatef(0.0, 0.0, -25.0);
       
   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Fixed torus.
   glColor3f(0.0, 1.0, 0.0);
   glutWireTorus(2.0, 12.0, 20, 20);

   // Begin revolving ball.
   glRotatef(longAngle, 0.0, 0.0, 1.0);
   
   glTranslatef(12.0, 0.0, 0.0);
   glRotatef(latAngle, 0.0, 1.0, 0.0);
   glTranslatef(-12.0, 0.0, 0.0);
   
   glTranslatef(20.0, 0.0, 0.0);

   glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(2.0, 10, 10);
   // End revolving ball.

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   if (isForce) 
   {
	  acceleration = applied_acceleration - drag*velocity; // Acceleration from force applied minus frictional
	                                                       // deceleration proportional to velocity.
      isForce = 0; // Set force to 0 - this makes it necessary to keep pressing the up key in order 
	               // to continue applying force.	  
   }
   else
      acceleration = -drag*velocity; // Only frictional deceleration proportional to velocity.
   
   velocity += acceleration; // Euler's method: change in velocity in unit time = acceleration.
  
   latAngle += 5.0 * velocity; // Change in latAngle is proportional to velocity.
   if (latAngle > 360.0) latAngle -= 360.0;
   longAngle += 1.0 * velocity; // Change in longAngle is proportional to velocity.
   if (longAngle > 360.0) longAngle -= 360.0;
   
   glutPostRedisplay();
   glutTimerFunc(animationPeriod, animate, 1);
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   glutTimerFunc(5, animate, 1);
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
         isForce = 1;  
         glutPostRedisplay();
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) applied_acceleration += 0.005;;
   if(key == GLUT_KEY_DOWN) if (applied_acceleration > 0.01) applied_acceleration -= 0.005;  
   if(key == GLUT_KEY_PAGE_UP) drag += 0.0005;  
   if(key == GLUT_KEY_PAGE_DOWN) if (drag > 0.001) drag -= 0.0005;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Keep space key pressed to apply force to the ball. Release to stop application." << endl
        << "Press up/down arrow keys to increase/decrease the applied force." << endl
        << "Press page up/down keys to increase/decrease the viscous drag." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow ("ballAndTorusWithFriction.cpp"); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();   
}

