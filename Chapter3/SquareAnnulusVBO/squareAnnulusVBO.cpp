///////////////////////////////////////////////////////////////////////////
// squareAnnulusVBO.cpp
// 
// This program modifies squareAnnulus4.cpp to store the vertex and color 
// arrays in one vertex buffer object (VBO) and the index array in another. 
// Simple color animation is made with help of a timer function by mapping 
// to the vertex buffer and changing color values.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////

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

#define VERTICES 0
#define INDICES 1

using namespace std;

// Begin globals.
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

static unsigned int buffer[2]; // Array of buffer ids.
// End globals.

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   // Get a pointer to the vertex buffer.
   float* bufferData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

   // Randomly change the color values.
   for (int i = 0; i < sizeof(colors)/sizeof(float); i++)
	  bufferData[sizeof(vertices)/sizeof(float) + i] = (float)rand()/(float)RAND_MAX;
      
   // Release the vertex buffer.
   glUnmapBuffer(GL_ARRAY_BUFFER);

   // Draw square annulus.
   glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, 0);

   glutSwapBuffers();
}

// Timer function.
void animate(int someValue)
{
   glutPostRedisplay();
   glutTimerFunc(500, animate, 1);
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glGenBuffers(2, buffer); // Generate buffer ids.

   // Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   // Bind vertex buffer and reserve space.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
   
   // Copy vertex coordinates data into first half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
   
   // Copy vertex color data into second half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

   // Bind and fill indices buffer.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stripIndices), stripIndices, GL_STATIC_DRAW);

   // Specify vertex and color pointers to the start of the respective data.
   glVertexPointer(3, GL_FLOAT, 0, 0);
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));

   glutTimerFunc(5, animate, 1);
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
 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("squareAnnulusVBO.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}

