/////////////////////////////////////////////////////////////////////////////////////////      
// threeQuarterSphere.cpp
// 
// This program is based on hemisphere.cpp. A half-hemisphere is attached to the bottom
// of a hemisphere to make three-quarters of a sphere. 
//
// The ordering of the vertices is such that the outside of the top (full) hemisphere
// is front-facing (filled), while the outside of the half-hemisphere is back-facing 
// (outlined).
//
// Interaction:
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
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

#define PI 3.14159265

using namespace std;

// Globals.
static float R = 5.0; // Radius of hemisphere.
static int p = 20; // Number of longitudinal slices.
static int q = 5; // Number of latitudinal slices.
static float Xangle = 120.0, Yangle = 60.0, Zangle = 0.0; // Angles to rotate hemisphere.

// Drawing routine.
void drawScene(void)
{
   int  i, j;

   glClear (GL_COLOR_BUFFER_BIT);

   glLoadIdentity();

   // Command to push the hemisphere, which is drawn centered at the origin, 
   // into the viewing frustum.
   glTranslatef(0.0, 0.0, -10.0);

   // Commands to turn the hemisphere.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);
   
   // Front faces are outlined, back faces filled.
   glPolygonMode(GL_FRONT, GL_FILL);
   glPolygonMode(GL_BACK, GL_LINE); 
   glColor3f(0.0, 0.0, 0.0);

   // glFrontFace(GL_CCW);

   // Top hemisphere.
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP); // One latitudinal strip.
         for(i = 0; i <= p; i++)
		 {
            glVertex3f( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        R * sin( (float)(j+1)/q * PI/2.0 ),
					    R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
            glVertex3f( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        R * sin( (float)j/q * PI/2.0 ),
					    R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );         
		 }
      glEnd();
   }

   // glFrontFace(GL_CW);

   /*
   glPolygonMode(GL_FRONT, GL_LINE);
   glPolygonMode(GL_BACK, GL_FILL);
   */

   // Bottom half-hemisphere.
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
         for(i = 0; i <= p/2; i++) // Half a latitudinal strip.
         // for(i = p/2; i >= 0; i--)
		 {
            glVertex3f( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        -R * sin( (float)(j+1)/q * PI/2.0 ), // Negative y-value.
					    R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
            glVertex3f( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        -R * sin( (float)j/q * PI/2.0 ), // Negative y-value.
					    R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) ); 
		 }
      glEnd();
   }

   /*
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
         for(i = 0; i <= p/2; i++) // Half a latitudinal strip.
		 {
            glVertex3f( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        -R * sin( (float)j/q * PI/2.0 ), // Negative y-value.
					    R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) ); 
            glVertex3f( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        -R * sin( (float)(j+1)/q * PI/2.0 ), // Negative y-value.
					    R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
		 }
      glEnd();
   }
   */

   glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
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
   glutCreateWindow("threeQuarterSphere.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop(); 
}

