//////////////////////////////////////////////////////////////////////////////////////          
// hemisphereMultidrawVBO.cpp
//
// This program builds on hemisphereMultidraw.cpp, storing the vertex and index arrays
// in buffer objects.
//
// Interaction:
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
#define RADIUS 5.0 // Radius of hemisphere.
#define LONG_SLICES 20 // Number of longitudinal slices.
#define LAT_SLICES 10 // Number of latitudinal slices.
#define VERTICES 0 // Vertex buffer id.
#define INDICES 1 // Indexes buffer id.

using namespace std;

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float vertices[3 * (LONG_SLICES + 1) * (LAT_SLICES + 1)]; // Vertex array of sample points on the hemisphere.
static unsigned int indices[LAT_SLICES][2*(LONG_SLICES+1)]; // Array of index arrays.
static int count[LAT_SLICES]; // Array of counts.
void* offsets[LAT_SLICES]; // Arrays of buffer offsets.
static unsigned int buffer[2]; // Array of buffer ids.

// Fill the vertex array with co-ordinates of the sample points.
void fillVertexArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= LAT_SLICES; j++)
      for (i = 0; i <= LONG_SLICES; i++)
      {
         vertices[k++] = RADIUS * cos( (float)j/LAT_SLICES * PI/2.0 ) * cos( 2.0 * (float)i/LONG_SLICES * PI );
         vertices[k++] = RADIUS * sin( (float)j/LAT_SLICES * PI/2.0 );
         vertices[k++] = RADIUS * cos( (float)j/LAT_SLICES * PI/2.0 ) * sin( 2.0 * (float)i/LONG_SLICES * PI );
      }
}

// Fill the array of index arrays.
void fillIndices(void)
{
   int i, j;
   for(j = 0; j < LAT_SLICES; j++)
   // Note: indexes for the jth index array (created by the loop below) correspond exactly
   // to the points on the jth triangle strip in the drawing loop of hemisphere.cpp.
   {
      for (i = 0; i <= LONG_SLICES; i++)
      {
	     indices[j][2*i] = (j+1)*(LONG_SLICES + 1) + i;
	     indices[j][2*i+1] = j*(LONG_SLICES + 1) + i;
      }
   }
}

// Fill the array of counts.
void fillCount(void)
{
   int j;
   for(j = 0; j < LAT_SLICES; j++) count[j] = 2*(LONG_SLICES + 1);
}

// Fill the array of buffer offsets.
void fillOffsets(void)
{
   int j;
   for(j = 0; j < LAT_SLICES; j++) offsets[j] = (GLvoid*)(2*(LONG_SLICES+1)*j*sizeof(unsigned int));
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glGenBuffers(2, buffer); // Generate buffer ids.

   glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array.

   // Initialize global arrays.
   fillVertexArray();
   fillIndices();
   fillCount();
   fillOffsets();

   // Bind and fill vertex buffer and set vertex pointer to the start of the buffer.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Bind and fill indices buffer.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Specify vertex pointer to the start of vertex data.
   glVertexPointer(3, GL_FLOAT, 0, 0);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   glLoadIdentity();

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
   glMultiDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (const void **)offsets, LAT_SLICES);

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
   cout << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
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
   glutCreateWindow("hemisphereMultidrawVBO.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
 
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}

