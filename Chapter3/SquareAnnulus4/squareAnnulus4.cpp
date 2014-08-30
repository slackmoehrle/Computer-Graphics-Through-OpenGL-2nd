//////////////////////////////////////////////////////////////////////////////
// squareAnnulus4.cpp
// 
// This program draws a square annulus as a triangle strip with vertices 
// whose co-ordinates are specified in one vertex array and colors in another. 
// Both arrays are accessed simulataneously using a single glDrawElemements()
// call with parameter GL_TRIANGLE_STRIP.
//
// Interaction:
// Press the space bar to toggle between wireframe and filled.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////

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

// Begin globals.
static int isWire = 0; // Is wireframe?

// Vertex co-ordinate vectors.
static float vertices[] =  
{	
   30.0, 30.0, 0.0,
   10.0, 10.0, 0.0, 
   70.0, 30.0, 0.0,
   90.0, 10.0, 0.0,
   70.0, 70.0, 0.0,
   90.0, 90.0, 0.0,
   30.0, 70.0, 0.0,
   10.0, 90.0, 0.0
};

// Vertex color vectors.
static float colors[] =  
{
   0.0, 0.0, 0.0,
   1.0, 0.0, 0.0,
   0.0, 1.0, 0.0,
   0.0, 0.0, 1.0,
   1.0, 1.0, 0.0,
   1.0, 0.0, 1.0,
   0.0, 1.0, 1.0,
   1.0, 0.0, 0.0
};

// Triangle strip vertex indices in order.
static unsigned int stripIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1};
// End globals.

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Draw square annulus. glDrawElements() "pulls up" data for 10 vertices in one command -
   // more efficient than calling glArrayElement() 10 times.
   glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, stripIndices);

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   // Specify locations for the co-ordinates and color arrays.
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glColorPointer(3, GL_FLOAT, 0, colors);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case ' ':
         if (isWire == 0) isWire = 1;
         else isWire = 0;
         glutPostRedisplay();
         break;	  
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to toggle between wireframe and filled." << endl;
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
   glutCreateWindow("squareAnnulus4.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}

