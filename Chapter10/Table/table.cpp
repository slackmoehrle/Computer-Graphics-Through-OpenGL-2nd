/////////////////////////////////////////////////////////////////////////////////////////
// table.cpp
//
// A table is created as a surface of revolution and approximated with a triangular mesh.
// The sectional curve is a polygonal line of 7 segments lying on the xy-plane.
//
// Interaction:
// Press left/right arrow keys to increase/decrease the number of grid columns.
// Press up/down arrow keys to increase/decrease the number of grid rows.
// Press x, X, y, Y, z, Z to turn the torus.
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

#define PI 3.14159265358979324
#define R 2.0
#define r 0.5

using namespace std;

// Globals.
static int p = 6; // Number of grid columns.
static int q = 4; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the table.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate the table.

// Function computes the x co-ordinate of the sectional curve in terms of the
// length t measured from one end of the curve.
float xc(float t)
{
   if       ( t <= 4.0 ) return t;
   else if  ( t <= 5.0 ) return 4.0;
   else if  ( t <= 8.0 ) return 9.0 - t;
   else if ( t <= 22.0 ) return 1.0;
   else if ( t <= 31.0 ) return t - 21.0;
   else if ( t <= 32.0 ) return 10.0;
   else                  return 42.0 - t;
}

// Function computes the y co-ordinate of the sectional curve in terms of the
// length t measured from one end of the curve.
float yc(float t)
{
   if       ( t <= 4.0 ) return -8.0;
   else if  ( t <= 5.0 ) return t - 12.0;
   else if  ( t <= 8.0 ) return -7.0;
   else if ( t <= 22.0 ) return t - 15.0;
   else if ( t <= 31.0 ) return 7.0;
   else if ( t <= 32.0 ) return t - 24.0;
   else                  return 8.0;
}

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the table.
float f(int i, int j)
{
   return ( xc( (float)i/p * 42.0 ) * cos( (-1 + 2*(float)j/q) * PI ) );
}

float g(int i, int j)
{
   return ( yc( (float)i/p * 42.0 ) ); 
}

float h(int i, int j)
{
   return ( xc( (float)i/p * 42.0 ) * sin( (-1 + 2*(float)j/q) * PI ) );
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         vertices[k++] = f(i,j);
         vertices[k++] = g(i,j);
         vertices[k++] = h(i,j);
      }
}

// Initialization routine.
void setup(void)
{
   glEnableClientState(GL_VERTEX_ARRAY);

   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Drawing routine.
void drawScene(void)
{
   int  i, j;
   vertices = new float[3*(p+1)*(q+1)]; // Dynamic array allocation with new value of p and q. 

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(0.0, 0.0, 0.0);

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Fill the vertex array.
   fillVertexArray();

   // Make the approximating triangular mesh.
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i <= p; i++)
      {
         glArrayElement( (j+1)*(p+1) + i );
         glArrayElement( j*(p+1) + i );
	  }
      glEnd();
   }

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

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT) if (p > 3) p -= 1;
   if (key == GLUT_KEY_RIGHT) p += 1;
   if (key == GLUT_KEY_DOWN) if (q > 3) q -= 1;
   if (key == GLUT_KEY_UP) q += 1;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press left/right arrow keys to increase/decrease the number of grid columns." << endl
        << "Press up/down arrow keys to increase/decrease the number of grid rows." << endl
        << "Press x, X, y, Y, z, Z to turn the torus." << endl;
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
   glutCreateWindow("table.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}

