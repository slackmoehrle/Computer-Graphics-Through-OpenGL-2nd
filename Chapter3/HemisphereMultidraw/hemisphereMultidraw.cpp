//////////////////////////////////////////////////////////////////////////////////////          
// hemisphereMultidraw.cpp
//
// This program modifies hemisphere.cpp, invoking one glMultiDrawElements() command to 
// draw all the triangle strips which were drawn in a loop in hemisphere.cpp.
//
// Interaction:
// Press P/p to increase/decrease the number of longitudinal slices.
// Press Q/q to increase/decrease the number of latitudinal slices.
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

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
#pragma comment(lib, "glew32.lib") // Compiler directive to include the GLEW library.
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float R = 5.0; // Radius of hemisphere.
static int p = 6; // Number of longitudinal slices.
static int q = 4; // Number of latitudinal slices.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float *vertices = NULL; // Vertex array of sample points on the hemisphere.
static unsigned int **indices = NULL; // Array of index arrays.
static int *count = NULL; // Array of counts.

// Fill the vertex array with co-ordinates of the sample points.
void fillVertexArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         vertices[k++] = R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI );
         vertices[k++] = R * sin( (float)j/q * PI/2.0 );
         vertices[k++] = R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI );
      }
}

// Fill the array of index arrays.
void fillIndices(void)
{
   int i, j;
   for(j = 0; j < q; j++)
   // Note: indexes for the jth index array (given in the loop below) correspond exactly
   // to the points on the jth triangle strip in the drawing loop of hemisphere.cpp.
   {
      for (i = 0; i <= p; i++)
      {
	     indices[j][2*i] = (j+1)*(p+1) + i;
	     indices[j][2*i+1] = j*(p+1) + i;
      }
   }
}

// Fill the array count.
void fillCount(void)
{
   int j;
   for(j = 0; j < q; j++) count[j] = 2*(p+1);
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array.
}

// Drawing routine.
void drawScene(void)
{
   int j;

   glClear (GL_COLOR_BUFFER_BIT);

   glLoadIdentity();

   // Create global arrays.
   vertices = new float[3*(p+1)*(q+1)];  
   indices = new unsigned int*[q];
   for(j = 0; j < q; j++) indices[j] = new unsigned int[2*(p+1)];
   count = new int[q];

   // Initialize global arrays.
   fillVertexArray();
   fillIndices();
   fillCount();

   glVertexPointer(3, GL_FLOAT, 0, vertices); 

   // Command to push the hemisphere, which is drawn centered at the origin, 
   // into the viewing frustum.
   glTranslatef(0.0, 0.0, -10.0);

   // Commands to turn the hemisphere.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Hemisphere properties.
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glColor3f(0.0, 0.0, 0.0);

   // Multidraw command equivalent to the drawing loop for(j = 0; j < q; j++){...} in hemisphere.cpp.
   glMultiDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (const void **)indices, q);

   glFlush();
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
      case 'P':
         p += 1;
         glutPostRedisplay();
         break;
      case 'p':
         if (p > 3) p -= 1;
         glutPostRedisplay();
         break;
      case 'Q':
         q += 1;
         glutPostRedisplay();
         break;
      case 'q':
         if (q > 3) q -= 1;
         glutPostRedisplay();
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
   cout << "Press P/p to increase/decrease the number of longitudinal slices." << endl
        << "Press Q/q to increase/decrease the number of latitudinal slices." << endl
        << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("hemisphereMultidraw.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}

