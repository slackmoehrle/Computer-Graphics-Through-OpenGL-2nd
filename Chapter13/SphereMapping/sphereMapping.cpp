///////////////////////////////////////////////////////////////////////
// sphereMappping.cpp
//
// This program demonstrates sphere mapping by generating sphere mapped
// texture co-ordinates on a movable cone.
//
// Interaction:
// Press the up/down arrow keys to move the cone.
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
static unsigned int texture[1]; // Array of texture indices.
static int t = 0; // Parameter to control motion of cone.
static GLUquadricObj *qobj; // Create a pointer to a quadric object.

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1]; 
   
   // Load the texture.
   image[0] = getbmp("../../Textures/launch.bmp");

   // Bind launch image to texture index[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
}

// Initialization routine.
void setup(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Create texture index array and load external textures.
   glGenTextures(1, texture);
   loadExternalTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

   // Specify automatic texture generation for a sphere map.
   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

   // Create the new quadric object, drawn filled with a normal generated per vertex.
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL); 
   gluQuadricNormals(qobj, GLU_SMOOTH); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
 
   // Map the launch texture onto a rectangle parallel to the xy-plane.
   glBindTexture(GL_TEXTURE_2D, texture[0]);      
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -100.0, -90.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -100.0, -90.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 100.0, -90.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 100.0, -90.0);
   glEnd();

   // Enable automatic texture generation (using the background launch
   // as it is the currently bound texture).
   glEnable(GL_TEXTURE_GEN_S);	
   glEnable(GL_TEXTURE_GEN_T);	

   // Draw a cone.
   glTranslatef(10.0, 55.0 - t*0.5, -70.0 + t*0.5);
   glRotatef(-135.0, 1.0, 0.0,0.0);
   gluCylinder(qobj, 0.0, 10.0, 15.0, 50, 50);

   // Disable automatic texture generation.
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);

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
   if (key == GLUT_KEY_DOWN) if (t < 130) t++; 
   if (key == GLUT_KEY_UP) if (t > 0) t--;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up/down arrow keys to move the cone." << endl;
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
   glutCreateWindow("sphereMapping.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
  
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
