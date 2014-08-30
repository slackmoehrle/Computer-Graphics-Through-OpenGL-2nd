//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// litDoublyCurledCone.cpp
//
// This programs builds on doublyCurledCone.cpp by coloring the cone and adding a single directional light source.
//
// Interaction:
// Press x, X, y, Y, z, Z to turn the cylinder.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
#define constantA 0.78539816339744831 // pi/4
#define constanta 0.05

using namespace std;

// Globals.
static int p = 50; // Number of grid columns.
static int q = 50; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the cone.
static float *normals = NULL; // Normal array of the mapped sample on the cylinder.
static float Xangle = 330.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate the cone.

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cone.
float f(int i, int j)
{
   return ( (float)j/q * 
			cos(constantA + constanta*4*(float)i/p * PI) * 
			cos(4*(float)i/p * PI) );
}

float g(int i, int j)
{
   return ( (float)j/q *  
			cos(constantA + constanta*4*(float)i/p * PI) * 
			sin(4*(float)i/p * PI) );
}

float h(int i, int j)
{
   return ( (float)j/q * 
			sin(constantA + constanta*4*(float)i/p * PI) );
}

// Function to map the grid vertex (u_i, v_j) to the magnitude of the cross-product of the of the partial derivatives at
// the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)).
float normn(int i, int j)
{
   float x;
   x =      (float)j/q * 
	        sqrt
	        ( constanta * constanta +
              cos(constantA + constanta*4*(float)i/p * PI) *
			  cos(constantA + constanta*4*(float)i/p * PI) );
   if (x == 0) return 0.001; else return x; // Prevent division by 0, if magnitude is 0.
}

// Fuctions to map the grid vertex (u_i,v_j) to the normal (fn(u_i,v_j), gn(u_i,v_j), hn(u_i,v_j)) to the cone
// at the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)).
float fn(int i, int j)
{
   return ( (-1) * constanta * 
	        (float)j/q * 
			sin(4*(float)i/p * PI) +
            (float)j/q * 
			sin(constantA + constanta*4*(float)i/p * PI) *
			cos(constantA + constanta*4*(float)i/p * PI) *
			cos(4*(float)i/p * PI) ) 
			/ normn(i, j);
}

float gn(int i, int j)
{
   return ( constanta * 
	        (float)j/q * 
			cos(4*(float)i/p * PI) +
            (float)j/q * 
			sin(constantA + constanta*4*(float)i/p * PI) *
			cos(constantA + constanta*4*(float)i/p * PI) *
			sin(4*(float)i/p * PI) ) 
			/ normn(i, j);
}

float hn(int i, int j)
{
   return ( (-1) * (float)j/q * 
			cos(constantA + constanta*4*(float)i/p * PI) *
			cos(constantA + constanta*4*(float)i/p * PI) ) 
			/ normn(i, j); 
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

// Routine to fill the normal array with normal vectors at the mapped sample points.
void fillNormalArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         normals[k++] = fn(i,j);
         normals[k++] = gn(i,j);
         normals[k++] = hn(i,j);
      }
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
   float lightPos[] = { 0.0, 1.5, 3.0, 0.0 };
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

   // Material property vectors.
   float matAmbAndDif1[] = {0.9, 0.0, 0.0, 1.0};
   float matAmbAndDif2[] = {0.0, 0.9, 0.0, 1.0};
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);

   vertices = new float[3*(p+1)*(q+1)];  
   normals = new float[3*(p+1)*(q+1)]; 

   // Fill the vertex and normal arrays.
   fillVertexArray();
   fillNormalArray();

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glNormalPointer(GL_FLOAT, 0, normals);
}

// Drawing routine.
void drawScene(void)
{
   int  i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate the scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);
   
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
   gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
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
   cout << "Press x, X, y, Y, z, Z to turn the cylinder." << endl;
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
   glutCreateWindow("litDoublyCurledCone.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
  
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop();
}

