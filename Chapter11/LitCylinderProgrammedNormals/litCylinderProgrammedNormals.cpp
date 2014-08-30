//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// litCylinderProgrammedNormals.cpp
//
// This programs modifies litCylinder.cpp to use (approximate) normals computed directly from the functions
// f(), g(), h() defining the surface of the cylinder. Normals are calculated as follows: 
//
// First, for the vertex with parameter (i,j) take one approximate tangent t1(i,j) to be the chord joining the
// vertices with parameters (i-1,j) and (i+1,j) and the other approximate tangent t2(i,j) to be the chord 
// joining the vertices with parameters (i,j-1) and (i,j+1) (care to be taken when i=0 or p, and j = 0 or q).  
// The approximate (unnormalized) normal is then the cross-product t1(i,j) x t2(i,j).
//
// Interaction:
// Press x, X, y, Y, z, Z to turn the cylinder.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

using namespace std;

// Globals.
static int p = 30; // Number of grid columns.
static int q = 10; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the cylinder.
static float *normals = NULL; // Normal array of the mapped sample on the cylinder.
static float Xangle = 150.0, Yangle = 60.0, Zangle = 0.0; // Angles to rotate the cylinder.

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder.
float f(int i, int j)
{
   return ( cos( (-1 + 2*(float)i/p) * PI ) );
}

float g(int i, int j)
{
   return ( sin( (-1 + 2*(float)i/p) * PI ) );
}

float h(int i, int j)
{
   return ( -1 + 2*(float)j/q );
}

// Fuctions to map the grid vertex (u_i,v_j) to the chord (ft1(u_i,v_j), gt1(u_i,v_j), ht1(u_i,v_j)) which
// joins the two vertices with one higher and one lower i parameter value (if exists) of the mesh vertex 
// (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder. This chord approximates a tangent at the mesh vertex 
// (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)).
float ft1(int i, int j)
{
   if ( (i>0) && (i<p) ) return ( f(i+1,j) - f(i-1,j) );
   else if (i==p)        return ( f(i,j) - f(i-1,j) );
   else /*(i==0)*/       return ( f(i+1,j) - f(i,j) );
}

float gt1(int i, int j)
{
   if ( (i>0) && (i<p) ) return ( g(i+1,j) - g(i-1,j) );
   else if (i==p)        return ( g(i,j) - g(i-1,j) );
   else /*(i==0)*/       return ( g(i+1,j) - g(i,j) );
}

float ht1(int i, int j)
{
   if ( (i>0) && (i<p) ) return ( h(i+1,j) - h(i-1,j) );
   else if (i==p)        return ( h(i,j) - h(i-1,j) );
   else /*(i==0)*/       return ( h(i+1,j) - h(i,j) );
}

// Fuctions to map the grid vertex (u_i,v_j) to the chord (ft2(u_i,v_j), gt2(u_i,v_j), ht2(u_i,v_j)) which
// joins the two vertices with one higher and one lower j parameter value (if exists) of the mesh vertex 
// (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder. This chord approximates a tangent at the mesh vertex 
// (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)).
float ft2(int i, int j)
{
   if ( (j>0) && (j<q) ) return ( f(i,j+1) - f(i,j-1) );
   else if (j==q)        return ( f(i,j) - f(i,j-1) );
   else /*(j==0)*/       return ( f(i,j+1) - f(i,j) );
}

float gt2(int i, int j)
{
   if ( (j>0) && (j<q) ) return ( g(i,j+1) - g(i,j-1) );
   else if (j==q)        return ( g(i,j) - g(i,j-1) );
   else /*(j==0)*/       return ( g(i,j+1) - g(i,j) );
}

float ht2(int i, int j)
{
   if ( (j>0) && (j<q) ) return ( h(i,j+1) - h(i,j-1) );
   else if (j==q)        return ( h(i,j) - h(i,j-1) );
   else /*(j==0)*/       return ( h(i,j+1) - h(i,j) );
}

// Fuctions to map the grid vertex (u_i,v_j) to the cross-product (fun(u_i,v_j), gun(u_i,v_j), hun(u_i,v_j))
// of the the two approximate tangents (ft1(), gt1(), ht1()) and (ft2(), gt2(), ht2()). Therefore,
// (fun(), gun(), hun()) is an approximate (unnormalized) normal to the cylinder at
// mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)).
float fun(int i, int j)
{
	return ( gt1(i,j)*ht2(i,j) - ht1(i,j)*gt2(i,j) );
}

float gun(int i, int j)
{
	return ( ht1(i,j)*ft2(i,j) - ft1(i,j)*ht2(i,j) );
}

float hun(int i, int j)
{
	return ( ft1(i,j)*gt2(i,j) - gt1(i,j)*ft2(i,j) );
}

// Fuction to map the grid vertex (u_i,v_j) to the length nl(u_i,v_j) of the approximate
// (unnormalized) normal (fun(), gun(), hun()).
float nl(int i, int j)
{
	return sqrt(fun(i,j)*fun(i,j) + gun(i,j)*gun(i,j) + hun(i,j)*hun(i,j));
}

// Fuctions to map the grid vertex (u_i,v_j) to the approximate normal (fn(u_i,v_j), gn(u_i,v_j), hn(u_i,v_j)) to 
// the cylinder at the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)), obtained by dividing the approximated
// unnormalized normal (fun(), gun(), hun()) by its length nl().
// WARNING: No check for division by zero!!
float fn(int i, int j)
{
   return ( fun(i,j)/nl(i,j) );
}

float gn(int i, int j)
{
    return ( gun(i,j)/nl(i,j) );
}

float hn(int i, int j)
{
    return ( hun(i,j)/nl(i,j) );
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
   gluLookAt (0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate scene.
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
   glutCreateWindow("litCylinderProgrammedNormals.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
 
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
  
   glutMainLoop();
}

