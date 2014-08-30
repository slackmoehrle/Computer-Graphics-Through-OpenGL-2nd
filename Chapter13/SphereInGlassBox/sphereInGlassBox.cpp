////////////////////////////////////////////////////////////////////////////////////////////        
// sphereInGlassBox.cpp
//
// This program builds on sphereInBox2.cpp with the sides of the box given low alpha values
// to make them appear translucent. The box side normals are unaveraged (straight).
//
// Interaction:
// Press up/down arrow keys to open/close the box.
// Press the x, X, y, Y, z, Z keys to rotate the box.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////////// 

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
static unsigned int aSide; // List index.
static int step = 0; // Steps in open/closing box lid.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.

// Initialization routine.
void setup(void)
{
   aSide = glGenLists (1); // Return a list index.

   // A side of the box in a display list.
   glNewList(aSide, GL_COMPILE);
   glBegin(GL_POLYGON);
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(1.0, -1.0, 1.0);
	  glVertex3f(-1.0, -1.0, 1.0);
	  glVertex3f(-1.0, -1.0, -1.0);
	  glVertex3f(1.0, -1.0, -1.0);
   glEnd();
   glEndList();

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
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
}

// Drawing routine.
void drawScene()
{
   // Material property vectors.
   float matAmbAndDif1[] = {0.9, 0.0, 0.0, 0.5}; // Alpha value of box sides = 0.5.
   float matAmbAndDif2[] = {0.0, 0.9, 0.0, 1.0}; // Alpha value of sphere = 1.0.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   // Position the box for viewing.
   gluLookAt(0.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

      // Rotate scene.
   glRotatef (Zangle, 0.0, 0.0, 1.0);
   glRotatef (Yangle, 0.0, 1.0, 0.0);
   glRotatef (Xangle, 1.0, 0.0, 0.0);

   // Material properties of the sphere (only the front is ever seen).
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);

   // Opaque sphere.
   glutSolidSphere (1.0, 40, 40);

   glDepthMask(GL_FALSE); // Make depth buffer read-only.

   // Material properties of the box.
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

   // Six translucent sides of the box, including the top, drawn with calls to the display list.
   glCallList(aSide); // Side 1.
   
   glPushMatrix();
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glCallList(aSide); // Side 2.
   glPopMatrix();

   glPushMatrix();
   glRotatef(270.0, 1.0, 0.0, 0.0);
   glCallList(aSide); // Side 3.
   glPopMatrix();

   glPushMatrix();
   glRotatef(90.0, 0.0, 0.0, 1.0);
   glCallList(aSide); // Side 4.
   glPopMatrix();

   glPushMatrix();
   glRotatef(270.0, 0.0, 0.0, 1.0);
   glCallList(aSide); // Side 5.
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 1.0, -1.0);
   glRotatef((float)step, -1.0, 0.0, 0.0);
   glTranslatef(0.0, -1.0, 1.0);
   glRotatef(180.0, 1.0, 0.0, 0.0);
   glCallList(aSide); // Top.
   glPopMatrix();

   glDepthMask(GL_TRUE); // Make depth buffer writable.

   glutSwapBuffers();

   glutPostRedisplay();
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
   if(key == GLUT_KEY_UP) if (step < 180) step++;;
   if(key == GLUT_KEY_DOWN) if (step > 0) step--;;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press up/down arrow keys to open/close the box." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the box." << endl;
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
   glutCreateWindow ("sphereInGlassBox.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
