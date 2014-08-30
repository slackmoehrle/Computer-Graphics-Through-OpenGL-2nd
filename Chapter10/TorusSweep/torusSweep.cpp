////////////////////////////////////////////////////////////////////////////////////////////////////
// torusSweep.cpp
//
// Modification of torus.cpp showing a torus being swept out by a circle as a surface of revolution.
//
// Interaction:
// Press space to toggle animation on/off.
// Press x, X, y, Y, z, Z to turn the torus.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////////////////////

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
static int p = 1; // Number of grid columns.
static int q = 12; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the torus.
static float Xangle = 150.0, Yangle = 30.0, Zangle = 0.0; // Angles to rotate the torus.
static float t = 0.0; // Defines sector of the torus drawn from 0 to tPI.
static int isAnimate = 0; // Animated?
static int animationPeriod = 250; // Time interval between frames.

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the torus.
float f(int i, int j)
{
   return ( ( R + r * cos( (-1 + 2*(float)j/q) * PI ) ) * cos( (float)i/p * t * 2 * PI ) );
}

float g(int i, int j)
{
   return ( ( R + r * cos( (-1 + 2*(float)j/q) * PI ) ) * sin( (float)i/p * t * 2 * PI ) ); 
}

float h(int i, int j)
{
   return ( r * sin( (-1 + 2*(float)j/q) * PI ) );
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

void setup(void)
{
   glEnableClientState(GL_VERTEX_ARRAY);
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Initialization routine.
void drawScene(void)
{
   int  i, j;
   vertices = new float[3*(p+1)*(q+1)]; // Dynamic array allocation with new value of p and q. 


   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glLineWidth(1.0);
   glColor3f(0.0, 0.0, 0.0);

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

   // Make the leading loop thick red.
   glLineWidth(4.0);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_LINE_LOOP);
   for(j = 0; j < q; j++)
      glArrayElement( (j+1)*(p+1) + p );
   glEnd();
   
   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      if ( t < 1.0) 
	  {
	  t += 0.05;
	  p += 1;
	  }

	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
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
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
		 }
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
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle animation on/off." << endl
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
   glutCreateWindow("torusSweep.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}

