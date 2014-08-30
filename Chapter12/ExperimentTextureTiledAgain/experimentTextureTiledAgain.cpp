///////////////////////////////////////////////////////////////////////
// experimentTextureTiledAgain.cpp
// (modifying loadTextures.cpp)
//
// Interaction:
// Press the left and right arrow keys to rotate the square.
// Press space to toggle between textures.
// Press delete to reset.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
///////////////////////////////////////////////////////////////////////

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
static unsigned int texture[2]; // Array of texture indices.
static unsigned char chessboard[64][64][4]; // Storage for chessboard image.
static float angle = 0.0; // Angle to rotate textured square.
static int id = 0; // Currently displayed texture id.

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1];
   
   // Load the image.
   image[0] = getbmp("../../Textures/launch.bmp"); 

   // Create texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 

   // Specify image data for currently active texture object.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Routine to load a program-generated image as a texture. 
void loadProceduralTextures()			
{
   // Create texture object texture[1]. 
   glBindTexture(GL_TEXTURE_2D, texture[1]); 

   // Specify image data for currently active texture object.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessboard);

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

// Create 64 x 64 RGBA image of a chessboard.
void createChessboard(void)
{
   int i, j;
   for (i = 0; i < 64; i++) 
      for (j = 0; j < 64; j++) 
         if ( ( ((i/8)%2) && ((j/8)%2) ) || ( !((i/8)%2) && !((j/8)%2) ) )
		 {
            chessboard[i][j][0] = 0x00;
            chessboard[i][j][1] = 0x00;
            chessboard[i][j][2] = 0x00;
            chessboard[i][j][3] = 0xFF;
		 }
		 else
		 {
            chessboard[i][j][0] = 0xFF;
            chessboard[i][j][1] = 0xFF;
            chessboard[i][j][2] = 0xFF;
            chessboard[i][j][3] = 0xFF;
		 }
}

// Initialization routine.
void setup(void)
{    
   glClearColor(0.8, 0.8, 0.8, 0.0); 

   // Create texture ids.
   glGenTextures(2, texture); 

   // Load external texture. 
   loadExternalTextures();

   // Generate procedural texture.
   createChessboard();

   // Load procedural texture.
   loadProceduralTextures();

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
   gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glRotatef(angle, 0.0, 1.0, 0.0); 

   // Activate texture object.
   glBindTexture(GL_TEXTURE_2D, texture[id]);        

   // Map the texture onto a square polygon.
   glBegin(GL_POLYGON);
      glTexCoord2f(-0.5, 0.0); glVertex3f(-10.0, -10.0, 0.0);
      glTexCoord2f(2.0, 0.0); glVertex3f(10.0, -10.0, 0.0);
      glTexCoord2f(2.0, 2.0); glVertex3f(10.0, 10.0, 0.0);
      glTexCoord2f(-0.5, 2.0); glVertex3f(-10.0, 10.0, 0.0);
   glEnd();


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
      case ' ':
         id++;
		 if (id == 2) id = 0; 
         glutPostRedisplay();
         break;
      case 127:
		 angle = 0.0; 
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT) 
   {
      angle -= 5.0;
      if (angle < 0.0) angle += 360.0;
   }
   if (key == GLUT_KEY_RIGHT) 
   {
      angle += 5.0;
      if (angle > 360.0) angle -= 360.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left and right arrow keys to rotate the square." << endl
	    << "Press space to toggle between textures." << endl
        << "Press delete to reset." << endl;
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
   glutCreateWindow("experimentTextureTiledAgain.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 

   glutMainLoop();
}
