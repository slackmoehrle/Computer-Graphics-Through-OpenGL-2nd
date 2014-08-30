//////////////////////////////////////////////////////      
// sphereInBox1.cpp
//
// This program draws a green sphere inside a red box. 
// There is a single positional light source.
// 
// Interaction:
// Press up/down arrow keys to open/close the box.
//
// Sumanta Guha.
//////////////////////////////////////////////////////      

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

#define ONE_BY_ROOT_THREE 0.57735

using namespace std;

// Begin globals.
static int step = 0; // Steps in open/closing the box lid.

// Box vertex co-ordinate vectors. 
static float vertices[] = 
{
	1.0, -1.0, 1.0, 
	1.0, 1.0, 1.0, 
	1.0, 1.0, -1.0, 
	1.0, -1.0, -1.0, 
	-1.0, -1.0, 1.0, 
	-1.0, 1.0, 1.0, 
	-1.0, 1.0, -1.0, 
	-1.0, -1.0, -1.0
};

// Vertex indices of triangle strip through center of box - consisting of 3 box sides.
static unsigned char stripIndices0[] = {5, 6, 4, 7, 0, 3, 2, 1}; 

// Vertex indices of triangle strip making back side of box.
static unsigned char stripIndices1[] = {6, 2, 7, 3}; 

// Vertex indices of triangle strip making front side of box.
static unsigned char stripIndices2[] = {0, 1, 4, 5}; 

// Vertex indices of triangle strip making top side of box.
static unsigned char stripIndices3[] = {6, 5, 2, 1}; 

// Box vertex normal vectors = normalized unit vector pointing from origin to vertex.
static float normals[] = 
{
	ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, 
	ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, 
	ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, 
	ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, 
	-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, 
	-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, 
	-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, 
	-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE
};
// End globals.

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

   // Light property vectors.
   float lightAmb[] = {0.0, 0.0, 0.0, 1.0};
   float lightDifAndSpec[] = {1.0, 1.0, 1.0, 1.0};
   float lightPos[] = {0.0, 1.5, 3.0, 1.0};
   float globAmb[] = {0.2, 0.2, 0.2, 1.0};

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Enable two vertex arrays: position and normal.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);

   // Specify locations for the position and normal arrays.
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glNormalPointer(GL_FLOAT, 0, normals);
}

// Drawing routine.
void drawScene(void)
{
   // Material property vectors.
   float matAmbAndDif1[] = {0.9, 0.0, 0.0, 1.0};
   float matAmbAndDif2[] = {0.0, 0.9, 0.0, 1.0};
   float matSpec[] = {1.0, 1.0, 1.0, 1.0};
   float matShine[] = {50.0};

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // Position the box for viewing.
   gluLookAt(0.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Material properties of the box.
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

   // Five sides of the box, excluding the top. The normal at each 
   // vertex is along the line from the origin to that vertex.
   glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, stripIndices0);
   glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, stripIndices1);
   glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, stripIndices2);
   
   // The rotated lid (top side) of the box. 
   glPushMatrix();
   glTranslatef(0.0, 1.0, -1.0);
   glRotatef((float)step, -1.0, 0.0, 0.0);
   glTranslatef(0.0, -1.0, 1.0);
   glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, stripIndices3);
   glPopMatrix();

   // Material properties of the sphere (only the front is ever seen).
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);

   // Cull the back faces of the sphere.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // Sphere.
   glutSolidSphere(1.0, 40, 40);
   
   glDisable(GL_CULL_FACE);

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
   if(key == GLUT_KEY_UP) if (step < 180) step++;;
   if(key == GLUT_KEY_DOWN) if (step > 0) step--;;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press up/down arrow keys to open/close the box." << endl;  
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
   glutCreateWindow ("sphereInBox1.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
