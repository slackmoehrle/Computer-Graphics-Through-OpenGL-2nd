////////////////////////////////////////////////////////////////////////////
// manipulateProjectionMatrix.cpp
//
// This program accesses the projection matrix stack. It is modified from
// manipulateModelviewMatrix.cpp. Matrix data is written to the C++ window.
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

using namespace std;

// Array to store the current projection matrix in column-major order.
static float projectionMatrixData[16];
// End globals.

// Print the matrix in conventional rectangular form.
void displayProjectionMatrix(void)
{
   cout << "The current projection matrix is:" << endl;
   cout << projectionMatrixData[0] << "   ";
   cout << projectionMatrixData[4] << "   ";
   cout << projectionMatrixData[8] << "   ";
   cout << projectionMatrixData[12] << endl;
   cout << projectionMatrixData[1] << "   ";
   cout << projectionMatrixData[5] << "   ";
   cout << projectionMatrixData[9] << "   ";
   cout << projectionMatrixData[13] << endl;
   cout << projectionMatrixData[2] << "   ";
   cout << projectionMatrixData[6] << "   ";
   cout << projectionMatrixData[10] << "   ";
   cout << projectionMatrixData[14] << endl;
   cout << projectionMatrixData[3] << "   ";
   cout << projectionMatrixData[7] << "   ";
   cout << projectionMatrixData[11] << "   ";
   cout << projectionMatrixData[15] << endl << endl;
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);  
   glLoadIdentity(); 

   gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glScalef(1.0, 2.0, 1.0);  
   
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
   
   // Read and print the current projection matrix.
   glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrixData);
   displayProjectionMatrix();

   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   // Read and print the current projection matrix.
   glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrixData);
   displayProjectionMatrix();
   
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
   glutCreateWindow ("manipulateProjectionMatrix.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
 
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop();
}
