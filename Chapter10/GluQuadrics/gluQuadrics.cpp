//////////////////////////////////////////////////////
// gluQuadrics.cpp
//   
// This program draws the four different GLU quadrics.
//
// Interaction:
// Press the left and right arrow keys to cycle through the quadrics.
// Press x, X, y, Y, z, Z to turn the quadric,
//
// Sumanta Guha
//////////////////////////////////////////////////////

#include <cstdlib>
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
static float Xangle = 30.0, Yangle = 30.0, Zangle = 30.0; // Angles to rotate quadric.
static int numVal = 0; // The ID of the quadric to draw.
static GLUquadricObj *qobj; // Create a pointer to a quadric object.
static char *objName; // Pointer to object name.

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Routine to write object name.
void writeObjectName()
{
   glPushMatrix();
   glTranslatef(-2.5, 10.0, -20.0);
   glScalef(0.01, 0.01, 0.01);
   writeStrokeString(GLUT_STROKE_ROMAN, objName);
   glPopMatrix();
}

// Initialization routine.
void setup(void) 
{
   // Create the new quadric object.
   qobj = gluNewQuadric();

   // Specify that quadrics are drawn in wireframe.
   gluQuadricDrawStyle(qobj, GLU_LINE); 

   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   
   glColor3f(0.0, 0.0, 0.0);

   glLoadIdentity();             
   gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPushMatrix();
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Draw the quadrics.
   switch(numVal) 
   {
      case 0:
		 gluSphere(qobj, 0.75, 15, 10);
		 objName = "Sphere";
         break;
      case 1:
	     gluCylinder(qobj, 0.5, 0.1, 1.0, 15, 5);
		 objName = "Tapered Cylinder";
         break;
      case 2:
	     gluDisk(qobj, 0.4, 1.0, 10, 5);
		 objName = "Annular Disc";
         break;
      case 3:
	     gluPartialDisk(qobj, 0.4, 1.0, 10, 5, 30.0, 270.0);
		 objName = "Partial Annular Disc";
         break;
      default:
         break;
   }
   glPopMatrix();

   writeObjectName();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {  
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
   if(key == GLUT_KEY_LEFT)
   {
      if (numVal > 0) numVal--;
      else numVal = 3;
   }
   if(key == GLUT_KEY_RIGHT)
   {
      if (numVal < 3) numVal++;
      else numVal = 0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the the left and right arrow keys to cycle through the quadrics." << endl
        << "Press x, X, y, Y, z, Z to turn the quadric." << endl;
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
   glutCreateWindow("gluQuadrics.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
