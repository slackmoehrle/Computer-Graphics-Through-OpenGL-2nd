/////////////////////////////////////////////////////////////////////////       
// experimentAutomaticNormalize.cpp
// (modifying sizeNormal.cpp)
//
// Interaction:
// Press up/down arrow keys to increase/decrease length of normal.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////// 

#include <iostream>

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
static float d = 1.0; // Length of normal.

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = { 0.0, 1.5, 3.0, 1.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode:
   // The ambient and diffuse color of the front faces will track the color set by glColor()
   // (only the front face of the triangle is visible).
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

   glEnable(GL_NORMALIZE);
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate scene.
   glRotatef (30.0, 0.0, 1.0, 0.0);

   // Draw triangle.
   glBegin(GL_TRIANGLES);
      glNormal3f(0.0, 0.0, 1.0);
	  glColor4f(0.0, 0.0, 1.0, 1.0); // Ambient and diffuse color.
      glVertex3f(-1.0, -1.0, 0.0);
      
	  glNormal3f(0.0, 0.0, 1.0);
      glColor4f(0.0, 1.0, 0.0, 1.0); // Ambient and diffuse color.
	  glVertex3f(1.0, -1.0, 0.0);
      
	  glNormal3f(0.0, 0.0, d);
      glColor4f(1.0, 0.0, 0.0, 1.0); // Ambient and diffuse color.
	  glVertex3f(0.0, 1.0, 0.0);
   glEnd();
  
   // Draw arrow indicating normal.
   glDisable(GL_LIGHTING);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
	  glVertex3f(0.0, 1.0, 0.0);
	  glVertex3f(0.0, 1.0, d);
	  glVertex3f(0.0, 1.0, d);
	  glVertex3f(0.1, 1.0, d-0.1);
	  glVertex3f(0.0, 1.0, d);
	  glVertex3f(-0.1, 1.0, d-0.1);
   glEnd();
   glEnable(GL_LIGHTING);

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
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
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
   if (key == GLUT_KEY_UP) d += 0.1;
   if (key == GLUT_KEY_DOWN)
   {
      if (d > 0.0) d -= 0.1;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << " Press up/down arrow keys to increase/decrease length of normal." << endl;
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
   glutCreateWindow ("experimentAutomaticNormalize.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
