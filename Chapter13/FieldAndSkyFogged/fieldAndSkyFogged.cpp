////////////////////////////////////////////////////////////////////////////////////////////
// fieldAndSkyFogged.cpp
//
// This program, based on fieldAndSky.cpp, adds controllable fog and a movable black sphere.
//
// Interaction:
// Press the space bar to cycle through the fog modes.
// Press the page up and down arrow keys to change the fog end if the mode is
// linear and the fog density if the mode is exponential or doubly exponential.
// Press the up and down arrow keys to move the black sphere.
// Press delete to reset the sphere.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
////////////////////////////////////////////////////////////////////////////////////////////

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

// Globals.
static unsigned int texture[2]; // Array of texture indices.
static float d = 0.0; // Distance parameter of ball.
static int isFog = 0; // Is fog on?
static int fogMode = GL_LINEAR; // Fog mode.
static float fogDensity = 0.01; // Fog density.
static float fogStart = 0.0; // Fog start z value.
static float fogEnd = 100.0; // Fog end z value.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static char theStringBuffer[10]; // String buffer.

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

// Write data.
void writeData(void)
{
   glColor3f(1.0, 1.0, 1.0); 
   
   glRasterPos3f(-5.0, 1.0, -6.0);

   if (isFog == 0) writeBitmapString((void*)font, "No fog");
   else if ( (isFog == 1) && (fogMode == GL_LINEAR) ) 
   {
	   floatToString(theStringBuffer, 4, fogEnd);
       writeBitmapString((void*)font, "Fog GL_LINEAR: start = 0.0, end = ");
	   writeBitmapString((void*)font, theStringBuffer);
   }
   else if ( (isFog == 1) && (fogMode == GL_EXP) ) 
   {
	   floatToString(theStringBuffer, 4, fogDensity);
       writeBitmapString((void*)font, "Fog GL_EXP: density = ");
	   writeBitmapString((void*)font, theStringBuffer);
   }
   else if ( (isFog == 1) && (fogMode == GL_EXP2) ) 
   {
	   floatToString(theStringBuffer, 4, fogDensity);
       writeBitmapString((void*)font, "Fog GL_EXP2: density = ");
	   writeBitmapString((void*)font, theStringBuffer);
   }
}   

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[2]; 
   
   // Load the textures.
   image[0] = getbmp("../../Textures/grass.bmp");
   image[1] = getbmp("../../Textures/sky.bmp");   

   // Bind grass image to texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

   // Bind sky image to texture object texture[1]
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);		
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);				
}

// Initialization routine.
void setup(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Create texture ids.
   glGenTextures(2, texture);

   // Load external textures.
   loadExternalTextures();

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// Drawing routine.
void drawScene(void)
{
   float fogColor[4] = {0.5, 0.5, 0.5, 1.0};

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Fog controls.
   if (isFog) glEnable(GL_FOG); else glDisable(GL_FOG);
   glFogfv(GL_FOG_COLOR, fogColor);
   glFogi(GL_FOG_MODE, fogMode);
   glFogf(GL_FOG_START, fogStart);
   glFogf(GL_FOG_END, fogEnd);
   glFogf(GL_FOG_DENSITY, fogDensity);
   glHint(GL_FOG_HINT, GL_NICEST);

   gluLookAt(0.0, 10.0, 15.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);

   // Map the grass texture onto a rectangle along the xz-plane.
   glBindTexture(GL_TEXTURE_2D, texture[0]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, 100.0);
      glTexCoord2f(8.0, 0.0); glVertex3f(100.0, 0.0, 100.0);
      glTexCoord2f(8.0, 8.0); glVertex3f(100.0, 0.0, -100.0);
      glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, 0.0, -100.0);
   glEnd();

   // Map the sky texture onto a rectangle parallel to the xy-plane.
   glBindTexture(GL_TEXTURE_2D, texture[1]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
   glEnd();

   // Draw black sphere.
   glTranslatef(0.0, 5.0, 5.0-d);
   glDisable(GL_TEXTURE_2D);
   glColor3f(0.0, 0.0, 0.0);
   glutSolidSphere(2.0, 40, 40);
   glEnable(GL_TEXTURE_2D);

   // Write data.
   glLoadIdentity();
   glDisable(GL_FOG);
   writeData();

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
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
      case 127:
         d = 0.0;
		 glutPostRedisplay();
         break;
      case ' ':
		  if (isFog == 0) { isFog = 1; fogMode = GL_LINEAR; }
		  else if (fogMode == GL_LINEAR) fogMode = GL_EXP;
          else if (fogMode == GL_EXP) fogMode = GL_EXP2;
		  else isFog = 0;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP) d += 0.2;
   if (key == GLUT_KEY_DOWN) d -= 0.2;
   if (key == GLUT_KEY_PAGE_UP) 
   {
      if (isFog && (fogMode != GL_LINEAR) ) fogDensity += 0.01;
	  if (isFog && (fogMode == GL_LINEAR) ) fogEnd += 5.0;
   }
   if (key == GLUT_KEY_PAGE_DOWN) 
   {
	  if (isFog && (fogMode != GL_LINEAR) ) if (fogDensity > 0.01) fogDensity -= 0.01;
      if (isFog && (fogMode == GL_LINEAR) ) if (fogEnd > 50.0) fogEnd -= 5.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to cycle through the fog modes." << endl
        << "Press the page up and down arrow keys to change the fog end if the mode is" << endl
        << "linear and the fog density if the mode is exponential or doubly exponential." << endl
        << "Press the up and down arrow keys to move the black sphere." << endl
		<< "Press delete to reset the sphere." << endl;
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
   glutCreateWindow("fieldAndSkyFogged.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop();
}
