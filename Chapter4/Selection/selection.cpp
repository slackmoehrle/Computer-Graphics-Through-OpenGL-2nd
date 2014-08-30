////////////////////////////////////////////////////////////////////////////////////         
// selection.cpp
//
// This program illustrates the use of the selection mode of OpenGL to determine the
// primitives that intersect a specified viewing volume. Hit records are output.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////

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

// Globals.
static int hits; // Number of entries in hit buffer.
static unsigned int buffer[1024]; // Hit buffer.
static int isHitBufferProcessed = 0; // Has the hit buffer been processed?

// Routine to draw rectangle of side length 2, parallel to xy-plane centered at (x, y, z)
// and color (R, G, B).
void drawRectangle(float x, float y, float z, float R, float G, float B)
{
   glColor3f(R, G, B);
   glPushMatrix();
   glTranslatef(0.0, 0.0, z);
   glRectf(x - 1.0, y - 1.0, x + 1.0, y + 1.0);
   glPopMatrix();
}

// Routine to draw the configuration for selection.
void drawConfiguration(void)
{   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 
   gluLookAt(7.5, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

   glutWireCube(10.0); // Outline of the selection volume.

   // MAKE SURE THE RECTANGLES BELOW ARE DRAWN IN THE selectHits() ROUTINE as well!
   drawRectangle(0.0, 0.0, 3.0, 1.0, 0.0, 0.0); // Rectangle 1.
   drawRectangle(0.0, 0.0, -3.0, 0.0, 1.0, 0.0); // Rectangle 2.

   glFlush();
}

// Routine that tests if primitives intersect the selection volume.
void selectHits(void)
{
   glSelectBuffer(1024, buffer); // Specify array to write hit records in selection mode.

   glRenderMode(GL_SELECT); // Enter selection mode.

   // Set the selection volume after saving the viewing volume defined in the resize routine.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix(); // Copy viewing volume.
   glLoadIdentity(); // Load identity.
   glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0); // Load selection volume.
   glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
   glLoadIdentity();

   glInitNames(); // Initializes the name stack to empty.
   glPushName(0); // Puts name 0 on top of stack.
 
   // MAKE SURE THE RECTANGLES BELOW ARE DRAWN IN THE drawConfiguration() ROUTINE as well!
   glLoadName(1); // Replace name 0 with name 1 at top of stack.
   drawRectangle(0.0, 0.0, 3.0, 1.0, 0.0, 0.0); // Rectangle 1 (red).

   glLoadName(2); // Replace name 1 with name 2 at top of stack.
   drawRectangle(0.0, 0.0, -3.0, 0.0, 1.0, 0.0); // Rectangle 2 (green).

   hits = glRenderMode(GL_RENDER); // Return to rendering mode, returning number of hits.

   // Restore viewing volume of the resize routine and return to modelview mode.
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
}

// Output hit records in hit buffer.
void processHitBuffer(int hits, unsigned int buffer[])
{
   unsigned int *ptr, numberNames;

   cout << "Number of hits = " << hits << endl << endl;
   ptr = buffer;
   for (int i = 0; i < hits; i++) 
   {
      cout << "Hit record " << i << ":" << endl;
      numberNames = *ptr;
	  cout << "Number of names in hit record = " << numberNames << endl;
	  ptr++;
      cout << "Min z-value of hit primitives = " << (float)*ptr/0xffffffff << endl; // Normalize to between 0 and 1.
	  ptr++;
      cout << "Max z-value of hit primitives = " << (float)*ptr/0xffffffff << endl; // Normalize to between 0 and 1. 
      ptr++;
	  cout << "List of names in hit record (copied from name stack): " ;
      for (int j = 0; j < (int)numberNames; j++) 
	  {	
         cout << *ptr << "  "; 
		 ptr++;
      }
	  cout << endl << endl;
   }
}
  
// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   
   drawConfiguration(); // Configuration is drawn to the frame buffer.
   selectHits(); // Hits are recorded in the hit buffer (nothing is drawn).
   if (isHitBufferProcessed == 0) // To ensure the hit buffer is output only once even if the
	                              // the screen is redrawn.
   {
      processHitBuffer(hits, buffer); // Hit buffer contents are output.
	  isHitBufferProcessed = 1;
   }
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);  
   glEnable(GL_DEPTH_TEST); // Enable depth testing.
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

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("selection.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();   
}

