////////////////////////////////////////////////////////////////////////////////////////
// mipmapLevels.cpp
//
// This program shows a square textured with mipmaps of a different color at each level.
// The square can be moved to see the mipmaps of different levels as they are applied.
//
// Interaction:
// Press the up and down arrow keys to move the square.
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

using namespace std;

// Globals.
static unsigned int texture[1]; // Array of texture indices.
static float d = 0.0; // Distance parameter in gluLookAt().
static unsigned char mipmapRes64[64][64][4]; // Storage for mipmap.
static unsigned char mipmapRes32[32][32][4]; // Storage for mipmap.
static unsigned char mipmapRes16[16][16][4]; // Storage for mipmap.
static unsigned char mipmapRes8[8][8][4]; // Storage for mipmap.
static unsigned char mipmapRes4[4][4][4]; // Storage for mipmap.
static unsigned char mipmapRes2[2][2][4]; // Storage for mipmap.
static unsigned char mipmapRes1[1][1][4]; // Storage for mipmap.

// Create mipmap images that are colored squares starting from 64 x 64 down to 1 x 1.
void createMipmaps(void)
{
   int i, j;

   for (i = 0; i < 64; i++) 
      for (j = 0; j < 64; j++) 
		 {
            mipmapRes64[i][j][0] = 0x00;
            mipmapRes64[i][j][1] = 0x00;
            mipmapRes64[i][j][2] = 0xFF;
            mipmapRes64[i][j][3] = 0xFF;
		 }

   for (i = 0; i < 32; i++) 
      for (j = 0; j < 32; j++) 
		 {
            mipmapRes32[i][j][0] = 0x00;
            mipmapRes32[i][j][1] = 0xFF;
            mipmapRes32[i][j][2] = 0xFF;
			mipmapRes32[i][j][3] = 0xFF;
		 }

   for (i = 0; i < 16; i++) 
      for (j = 0; j < 16; j++) 
		 {
            mipmapRes16[i][j][0] = 0xFF;
            mipmapRes16[i][j][1] = 0x00;
            mipmapRes16[i][j][2] = 0xFF;
			mipmapRes16[i][j][3] = 0xFF;
		 }

   for (i = 0; i < 8; i++) 
      for (j = 0; j < 8; j++) 
		 {
            mipmapRes8[i][j][0] = 0xFF;
            mipmapRes8[i][j][1] = 0xFF;
            mipmapRes8[i][j][2] = 0x00;
			mipmapRes8[i][j][3] = 0xFF;
		 }

   for (i = 0; i < 4; i++) 
      for (j = 0; j < 4; j++) 
		 {
            mipmapRes4[i][j][0] = 0xFF;
            mipmapRes4[i][j][1] = 0x00;
            mipmapRes4[i][j][2] = 0x00;
			mipmapRes4[i][j][3] = 0xFF;
		 }

   for (i = 0; i < 2; i++) 
      for (j = 0; j < 2; j++) 
		 {
            mipmapRes2[i][j][0] = 0x00;
            mipmapRes2[i][j][1] = 0xFF;
            mipmapRes2[i][j][2] = 0x00;
			mipmapRes2[i][j][3] = 0xFF;
		 }

   for (i = 0; i < 1; i++) 
      for (j = 0; j < 1; j++) 
		 {
            mipmapRes1[i][j][0] = 0x00;
            mipmapRes1[i][j][1] = 0x00;
            mipmapRes1[i][j][2] = 0x00;
			mipmapRes1[i][j][3] = 0xFF;
		 }
}

// Routine to load mipmaps. 
void loadMipmaps()			
{
   // Create texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 

   // Set texture parameters.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify mipmaps to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes64);
   glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes32);
   glTexImage2D(GL_TEXTURE_2D, 2, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes16);
   glTexImage2D(GL_TEXTURE_2D, 3, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes8);
   glTexImage2D(GL_TEXTURE_2D, 4, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes4);
   glTexImage2D(GL_TEXTURE_2D, 5, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes2);
   glTexImage2D(GL_TEXTURE_2D, 6, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapRes1);
}

// Initialization routine.
void setup(void)
{    
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Create texture index array.
   glGenTextures(1, texture); 

   // Create and load mipmaps.
   createMipmaps();
   loadMipmaps();

  // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 6.0 + d, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Map texture onto a square polygon.
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
   glEnd();

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 2000.0);
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
   if (key == GLUT_KEY_DOWN) 
   {
      if (d > 0.0) d -= 0.2;
   }
   if (key == GLUT_KEY_UP) 
   {
      d += 0.2;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up and down arrow keys to move the square." << endl;
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
   glutInitWindowPosition(5, 5);
   glutCreateWindow("mipmapLevels.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 

   glutMainLoop();
}
