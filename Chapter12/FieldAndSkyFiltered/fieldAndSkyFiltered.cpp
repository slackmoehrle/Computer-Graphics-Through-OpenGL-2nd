////////////////////////////////////////////////////////////////////////////////////////
// fieldAndSkyFiltered.cpp
//
// This program is based on fieldAndSky.cpp. In addition to the viewpoint being movable, 
// the grass texture filter can be set at different levels.
//
// Interaction:
// Press the up and down arrow keys to move the viewpoint over the field.
// Press the left and right arrow keys to cycle through the filters.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
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
static unsigned int texture[7]; // Array of texture indices.
static float d = 0.0; // Distance parameter in gluLookAt().
static int filter = 0; // Filter id.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[7]; 
   
   // Load the images.
   image[0] = getbmp("../../Textures/grass.bmp");
   image[1] = getbmp("../../Textures/sky.bmp");   

   // Bind grass image to texture index[0] with specified mag and min filters. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Bind grass image to texture index[1] with specified mag and min filters. 
   glBindTexture(GL_TEXTURE_2D, texture[1]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Bind grass image to texture index[2] with specified mag and min filters. 
   // Use mipmapping.
   glBindTexture(GL_TEXTURE_2D, texture[2]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Bind grass image to texture index[3] with specified mag and min filters.
   // Use mipmapping.
   glBindTexture(GL_TEXTURE_2D, texture[3]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Bind grass image to texture index[4] with specified mag and min filters. 
   // Use mipmapping.
   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Bind grass image to texture index[5] with specified mag and min filters. 
   // Use mipmapping.
   glBindTexture(GL_TEXTURE_2D, texture[5]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Bind sky image to texture index[6]
   glBindTexture(GL_TEXTURE_2D, texture[6]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);		
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Write messages.
void writeData(void)
{
   switch(filter) 
   {
      case 0:
         glRasterPos3f(-4.9, 4.8, -5.1); writeBitmapString((void*)font, "Grass mag filter: GL_NEAREST");
         glRasterPos3f(-4.9, 4.5, -5.1); writeBitmapString((void*)font, "Grass min filter: GL_NEAREST");
         break;
      case 1:
         glRasterPos3f(-4.9, 4.8, -5.1); writeBitmapString((void*)font, "Grass mag filter: GL_LINEAR");
         glRasterPos3f(-4.9, 4.5, -5.1); writeBitmapString((void*)font, "Grass min filter: GL_LINEAR");
         break;
      case 2:
         glRasterPos3f(-4.9, 4.8, -5.1); writeBitmapString((void*)font, "Grass mag filter: GL_LINEAR");
         glRasterPos3f(-4.9, 4.5, -5.1); writeBitmapString((void*)font, "Grass min filter: GL_NEAREST_MIPMAP_NEAREST");
         break;
      case 3:
         glRasterPos3f(-4.9, 4.8, -5.1); writeBitmapString((void*)font, "Grass mag filter: GL_LINEAR");
         glRasterPos3f(-4.9, 4.5, -5.1); writeBitmapString((void*)font, "Grass min filter: GL_LINEAR_MIPMAP_NEAREST");
         break;
      case 4:
         glRasterPos3f(-4.9, 4.8, -5.1); writeBitmapString((void*)font, "Grass mag filter: GL_LINEAR");
         glRasterPos3f(-4.9, 4.5, -5.1); writeBitmapString((void*)font, "Grass min filter: GL_NEAREST_MIPMAP_LINEAR");
         break;
      case 5:
         glRasterPos3f(-4.9, 4.8, -5.1); writeBitmapString((void*)font, "Grass mag filter: GL_LINEAR");
         glRasterPos3f(-4.9, 4.5, -5.1); writeBitmapString((void*)font, "Grass min filter: GL_LINEAR_MIPMAP_LINEAR");
         break;
      default:
         break;
   }
}

// Initialization routine.
void setup(void)
{    
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);

   // Create texture ids.
   glGenTextures(7, texture);

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
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_TEXTURE_2D); 
   glLoadIdentity();
   gluLookAt(0.0, 10.0, 15.0 + d, 0.0, 10.0, d, 0.0, 1.0, 0.0);

   // Map the grass texture onto a rectangle along the xz-plane.
   glBindTexture(GL_TEXTURE_2D, texture[filter]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, 100.0);
      glTexCoord2f(8.0, 0.0); glVertex3f(100.0, 0.0, 100.0);
      glTexCoord2f(8.0, 8.0); glVertex3f(100.0, 0.0, -100.0);
      glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, 0.0, -100.0);
   glEnd();

   // Map the sky texture onto a rectangle parallel to the xy-plane.
   glBindTexture(GL_TEXTURE_2D, texture[6]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
   glEnd();

   // Write data.
   glDisable(GL_TEXTURE_2D); 
   glLoadIdentity();
   glColor3f(0.0, 0.0, 0.0);
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
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP) 
   {
      if (d > -50.0) d -= 0.1;
   }
   if (key == GLUT_KEY_DOWN) 
   {
      if (d < 15.0) d += 0.1;
   }
   if (key == GLUT_KEY_RIGHT) 
   {
      filter++;
	  if (filter == 6) filter = 0; 
   }
   if (key == GLUT_KEY_LEFT) 
   {
      filter--;
      if (filter == -1) filter = 5; 
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up and down arrow keys to move the viewpoint over the field." << endl 
	    << "Press the left and right arrow keys to cycle through the filters." << endl;
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
   glutCreateWindow("fieldAndSkyFiltered.cpp");
   glewInit();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 

   glutMainLoop();
}
