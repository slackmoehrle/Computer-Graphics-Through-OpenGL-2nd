///////////////////////////////////////////////////////////////////////
// pointSprite.cpp
//
// This program shows how to create and manipulate point sprites.
//
// Interaction:
// Press space to toggle between animation on and off.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
///////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
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
static int isAnimate = 0; // Animated?
static float size = 0.0; // Point sprite size.
static float angle = 0.0; // Angle to rotate point sprite.
static float width; // Width of OpenGL window.
static float height; // Height of OpenGL window.

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1];
   
   // Load the image.
   image[0] = getbmp("../../Textures/star.bmp"); 

  // Bind star image to texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Initialization routine.
void setup(void)
{    
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Create texture ids.
   glGenTextures(1, texture); 

   // Load external texture.
   loadExternalTextures();

   // Turn on OpenGL texturing and activate a texture.
   glEnable(GL_TEXTURE_2D);

   // Enable texture interpolation across point sprites.
   glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);  

   // Set origin of the texture coordinate system on the point sprite.
   glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT); 
   
   // Enable point sprites.
   glEnable(GL_POINT_SPRITE); 
}

// Routine to draw a point sprite at a location on a circle.
void drawPoint(float pointSize, float circleRadius, float circleAngle)
{
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(circleRadius * cos(circleAngle), circleRadius * sin(circleAngle), 0.0);
   glEnd();
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Draw six point sprites moving one after another in a circle, sizes changing.
   for (int i = 0; i < 6; i++)
   {
      drawPoint((i%2) ? 90.0 - size : size, (width <= height) ? width/40.0 : height/40.0, angle + (float)i * PI/3.0);
   }

   glutSwapBuffers();	
}

// Timer function for glutTimerFunc().
void animate(int someValue)
{
   if (isAnimate)
   {
      (size >= 60.0) ? size = 20.0 : size += 0.5; 
      (angle >= 360.0) ? angle = 0.0 : angle +=PI/120.0;
      glutPostRedisplay();
	  glutTimerFunc(10, animate, 1);
   }
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

   width = w;
   height = w;
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
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl;
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
   glutCreateWindow("pointSprite.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
  
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
