////////////////////////////////////////////////////////////////////////////
// manipulateModelviewMatrix.cpp
//
// This program shows access and manipulation of the modelview matrix stack.
// Matrix data is written to the C++ window.
// 
// Sumanta Guha
////////////////////////////////////////////////////////////////////////////

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

#define X 0.70710678 /* one divided by root two */

using namespace std;

// Begin globals.
// Matrix corresponding to rotation by 45 degrees about the z-axis - 
// stored in an array in column-major order.
static float matrixData[16] = 
{
   X, X, 0.0, 0.0, -X, X, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0
};

// Array to store the current modelview matrix in column-major order.
static float modelviewMatrixData[16];
// End globals.

// Print the matrix in conventional rectangular form.
void displayModelviewMatrix(void)
{
   cout << "the current modelview matrix is:" << endl;
   cout << modelviewMatrixData[0] << "   ";
   cout << modelviewMatrixData[4] << "   ";
   cout << modelviewMatrixData[8] << "   ";
   cout << modelviewMatrixData[12] << endl;
   cout << modelviewMatrixData[1] << "   ";
   cout << modelviewMatrixData[5] << "   ";
   cout << modelviewMatrixData[9] << "   ";
   cout << modelviewMatrixData[13] << endl;
   cout << modelviewMatrixData[2] << "   ";
   cout << modelviewMatrixData[6] << "   ";
   cout << modelviewMatrixData[10] << "   ";
   cout << modelviewMatrixData[14] << endl;
   cout << modelviewMatrixData[3] << "   ";
   cout << modelviewMatrixData[7] << "   ";
   cout << modelviewMatrixData[11] << "   ";
   cout << modelviewMatrixData[15] << endl << endl;
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);   
   glLoadIdentity(); 

   // Read and print the current modelview matrix.
   cout << "After the initial glLoadIdentity() statement" << endl;
   glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrixData);
   displayModelviewMatrix();

   gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Read and print the current modelview matrix.
   cout << "After the gluLookAt() statement" << endl;
   glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrixData);
   displayModelviewMatrix();

   // Multiply the current modelview matrix on the right by the
   // matrix corresponding to rotation by 45 degrees about the z-axis.
   glMultMatrixf(matrixData);

   // Read and print the current modelview matrix.
   cout << "After the glMultMatrixf() statement" << endl;
   glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrixData);
   displayModelviewMatrix();

   glScalef(1.0, 2.0, 1.0);  
   
   // Read and print the current modelview matrix.
   cout << "After the glScalef() statement" << endl;
   glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrixData);
   displayModelviewMatrix();

   glutWireCube(5.0);
   glFlush();
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
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("manipulateModelviewMatrix.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
