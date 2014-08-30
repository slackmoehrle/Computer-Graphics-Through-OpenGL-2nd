//////////////////////////////////////////////////////////////////////////////////////////
// billboard.cpp
//
// This program illustrates billboarding. A viewpoint can move through a scene with a two
// trees textured onto a rectangle. There is the option to turn billboarding on and off.
//
// Interaction:
// Press the space key to toggle between billboarding on and off.
// Press the up and down arrow keys to move the viewpoint.
// 
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
//////////////////////////////////////////////////////////////////////////////////////////

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

#include "getbmp.h"

using namespace std;

#define PI 3.14159265358979324

// Globals.
static unsigned int texture[1]; // Array of texture indices.
static float d = 40.0; // Distance of the trees image parallel to the line of sight.
static float b = 20.0; // Displacement of the trees image left of line of sight.
static int isBillboard = 0; // Is billboarding on?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1]; 
   
   // Load the image.
   image[0] = getbmp("../../Textures/trees.bmp");

   // Bind trees image to texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 				
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
   glClearColor (1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);

   // Create texture ids.
   glGenTextures(1, texture);

   // Load external textures.
   loadExternalTextures();

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glLoadIdentity();

   // Write message.
   glDisable(GL_TEXTURE_2D);
   glPushMatrix();
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(-1.0, 4.0, -5.1); 
   if (isBillboard) writeBitmapString((void*)font, "Billboarding on!");
   else writeBitmapString((void*)font, "Billboarding off!");
   glPopMatrix();
   glEnable(GL_TEXTURE_2D);
   
   // BEGIN: Place the trees image.
   glPushMatrix(); 
   glTranslatef(-b, 0.0, -d);

   // If billboarding on, rotate the trees image so that it is normal to the viewing direction.
   if (isBillboard) glRotatef( atan(b/d)*(180.0/PI), 0.0, 1.0, 0.0);

   // Map the trees texture onto a vertical rectangle.
   glBindTexture(GL_TEXTURE_2D, texture[0]);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-2.5, -5.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(2.5, -5.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(2.5, 5.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-2.5, 5.0, 0.0);
   glEnd();

   glPopMatrix(); 
   // END: Place the trees image.

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-10.0, 10.0, -5.0, 5.0, 5.0, 100.0);
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
      case ' ':
         if (isBillboard) isBillboard = 0;
		 else isBillboard = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN)  d += 0.2;
   if (key == GLUT_KEY_UP) d -= 0.2;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space key to toggle between billboarding on and off." << endl
	    << "Press the up and down arrow keys to move the viewpoint." << endl;
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
   glutInitWindowPosition(10, 10);
   glutCreateWindow("billboard.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
