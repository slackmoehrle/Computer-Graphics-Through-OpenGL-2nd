////////////////////////////////////////////////////////////////////////////////////        
// litTriangle.cpp
//
// This program draws one triangle lit by a single light source. The user can choose
// to enable or disable two-sided lighting.
//
// Interaction:
// Press space to toggle between turning two-sided lighting on and off.
// Press the left-right keys to rotate the triangle.
//
//Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////

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
static int twoSided = 1; // Whether two-sided lighting enabled.
static float Yangle = 0.0; // Angle to rotate triangle. 
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

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
   float matAmbAndDif1[] = {0.9, 0.0, 0.0, 1.0};
   float matAmbAndDif2[] = {0.0, 0.0, 0.9, 1.0};
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
   glMaterialfv(GL_BACK,  GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
}

// Drawing routine.
void drawScene()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // Set two-sided lighting value and write message.
   glDisable(GL_LIGHTING);
   glColor3f(0.0, 0.0, 0.0);
   if (twoSided) 
   {
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 
      glRasterPos3f(-1.0, 1.0, -2.0);
      writeBitmapString((void*)font, "Two-sided lighting on!");
   }
   else
   {
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); 
      glRasterPos3f(-1.0, 1.0, -2.0);
      writeBitmapString((void*)font, "Two-sided lighting off!");
   }
   glEnable(GL_LIGHTING);

   gluLookAt(0.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate triangle.
   glRotatef (Yangle, 0.0, 1.0, 0.0);

   // Draw triangle.
   glBegin(GL_TRIANGLES);
      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(-1.0, -1.0, 0.0);
	  glVertex3f(1.0, -1.0, 0.0);
	  glVertex3f(0.0, 1.0, 0.0);
   glEnd();

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
	  case ' ':
		 if (twoSided) twoSided = 0; else twoSided = 1;
		 glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT)
   {
      Yangle += 5.0;
	  if (Yangle > 360.0) Yangle -= 360.0;
   }
   if (key == GLUT_KEY_LEFT)
   {
	  Yangle -= 5.0;
      if (Yangle < 0.0) Yangle += 360.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between turning two-sided lighting on and off." << endl
        << "Press the left-right keys to rotate the triangle." << endl;
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
   glutCreateWindow("litTriangle.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
  
   glutMainLoop();
}
