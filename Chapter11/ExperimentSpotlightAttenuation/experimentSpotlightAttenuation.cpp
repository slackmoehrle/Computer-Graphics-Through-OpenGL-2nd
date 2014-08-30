//////////////////////////////////////////////////////////////////////////////////////        
// experimentSpotlightAttenuation.cpp
// (modifying spotlight.cpp)
//
// Interaction:
// Press the page up/down keys to increase/decrease the spotlight cone angle.
// Press the arrow keys to move the spotlight.
// Press 't/T' to decrease/increase the spotlight's attenuation.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////

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
static float spotAngle = 10.0; // Spotlight cone half-angle.
static float xMove = 0.0, zMove = 0.0; // Movement components.
static float spotExponent = 2.0; // Spotlight exponent = attenuation.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

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
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float globAmb[] = { 0.05, 0.05, 0.05, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties shared by all the spheres.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // Enable color material mode:
   // The ambient and diffuse color of the front faces will track the color set by glColor().
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// Drawing routine.
void drawScene()
{
   float lightPos[] = { 0.0, 3.0, 0.0, 1.0 }; // Spotlight position.
   float spotDirection[] = {0.0, -1.0, 0.0}; // Spotlight direction.   

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // Write message.
   glDisable(GL_LIGHTING);
   glColor3f(1.0, 1.0, 1.0);
   floatToString(theStringBuffer, 4, spotExponent);
   glRasterPos3f(-1.0, 1.0, -2.0);
   writeBitmapString((void*)font, "Spotlight attenuation: ");  
   writeBitmapString((void*)font, theStringBuffer);
   glEnable(GL_LIGHTING);

   gluLookAt (0.0, 4.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 
   glPushMatrix();
   glTranslatef(xMove, 0.0, zMove); // Move the spotlight.
   
   // Draw the spotlight cone in wireframe after disabling lighting
   glPushMatrix();
   glDisable(GL_LIGHTING);
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   glColor3f(1.0, 1.0, 1.0);
   glutWireCone(3.0 * tan( spotAngle/180.0 * PI ), 3.0, 20, 20);
   glEnable(GL_LIGHTING);
   glPopMatrix();
   
   // Spotlight properties including position.
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);  
   glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);    
   glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

   glPopMatrix();

   // Draw 10 x 10 array of multi-colored spheres.
   int i,j;
   for (i = 0; i < 9; i++)
      for (j = 0; j < 9; j++)
      {
	     glPushMatrix();
		 glTranslatef(-4.0+i, 0.0, -4.0+j);

         // Ambient and diffuse colors of the spheres specified to alternate.
		 if ((i+j)%3 == 0) glColor4f(1.0, 0.0, 0.0, 1.0);
		 else if ((i+j)%3 == 1) glColor4f(0.0, 1.0, 0.0, 1.0);
		 else glColor4f(0.0, 0.0, 1.0, 1.0);

		 glutSolidSphere (0.5, 20.0, 16.0);
         glPopMatrix(); 
	  }
   
   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
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
	  case 't':
 		 if (spotExponent > 0.0) spotExponent -= 0.1;
         glutPostRedisplay();
		 break;
	  case 'T':
 		 spotExponent += 0.1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_PAGE_DOWN)
   {
      if (spotAngle > 0.0) spotAngle -= 1.0;
   }
   if( key == GLUT_KEY_PAGE_UP)
   {
      if (spotAngle < 90.0) spotAngle += 1.0;
   }
   if (key == GLUT_KEY_UP)
   {
      if (zMove > -4.0) zMove -= 0.1;
   }
   if (key == GLUT_KEY_DOWN)
   {
      if (zMove < 4.0) zMove += 0.1;
   }
   if (key == GLUT_KEY_LEFT)
   {
      if (xMove > -4.0) xMove -= 0.1;
   }
   if (key == GLUT_KEY_RIGHT)
   {
      if (xMove < 4.0) xMove += 0.1;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the page up/down arrow keys to increase/decrease the spotlight cone angle." << endl
	    << "Press the arrow keys to move the spotlight." << endl
		<< "Press 't/T' to decrease/increase the spotlight's attenuation." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("experimentSpotlightAttenuation.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
   
   glutMainLoop();
}
