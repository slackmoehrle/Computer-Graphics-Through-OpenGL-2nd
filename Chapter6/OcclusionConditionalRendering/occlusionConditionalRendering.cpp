/////////////////////////////////////////////////////////////////////////
// occlusionConditionalRendering.cpp
//
// Requires OpenGL 3.3 or higher.
//
// This program modifies occlusion.cpp to invoke conditional rendering. 
// It implements an occlusion query to check if the bounding box of a 
// sphere is visible and draw the sphere on condition its bounding box
// is visible.
//
// Interaction:
// Press the arrow keys to move the sphere and its bounding box.
// Press r to reset their location.
// Press the space bar to toggle between showing/hiding the bounding box.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////

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
static float Xvalue = 0.0, Yvalue = 0.0; // Co-ordinates of the sphere.
static unsigned int query; // Query id. 
static unsigned int boxVisible = 1; // Is the bounding box visible?

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST); 
   glLoadIdentity();

   // Draw a fixed obscuring green rectangle.
   glPushMatrix();
   glColor3f(0.0, 1.0, 0.0);
   glTranslatef(0.0, 0.0, -3.0);
   glRectf(-0.5, -0.5, 0.5, 0.5);
   glPopMatrix();

   glColor3f(1.0, 0.0, 0.0);
   glPushMatrix();
   
   glTranslatef(Xvalue, Yvalue, -5.0);

   // Draw a red cube, which bounds a sphere, with an occlusion query active. 
   // If boxVisible is true, then the cube is rendered hiding the sphere; if false, 
   // (a) the cube is not displayed, and 
   // (b) it does not alter depth values.
   glBeginQuery(GL_SAMPLES_PASSED, query); // Begin query operation, i.e., start counting fragments which 
                                           // pass the depth test.
   if (!boxVisible)
   {
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Make color buffer not writable.
      glDepthMask(GL_FALSE); // Make depth buffer not writable.
   }
   glutSolidCube(1.0);
   if (!boxVisible)
   {
      glDepthMask(GL_TRUE); // Make depth buffer writable.
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Make color buffer writable.
   }
   glEndQuery(GL_SAMPLES_PASSED); // End query operation, i.e., stop counting fragments.

   // Conditional rendering of the sphere. The GL_QUERY_WAIT parameter
   // says to wait for the query result to return.
   glBeginConditionalRender(query, GL_QUERY_WAIT); 
   glutWireSphere(0.5, 16, 10);
   glEndConditionalRender();

   glPopMatrix();

   glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);  
   glGenQueries(1, &query); // Generate a query object, putting its id in query.
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 'r':
         Xvalue = Yvalue = 0.0;
         glutPostRedisplay();
         break;
      case ' ':
         if (boxVisible) boxVisible = 0; else boxVisible = 1;
         glutPostRedisplay();
         break;
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
   if(key == GLUT_KEY_UP) Yvalue += 0.1;
   if(key == GLUT_KEY_DOWN) Yvalue -= 0.1;
   if(key == GLUT_KEY_LEFT) Xvalue -= 0.1;
   if(key == GLUT_KEY_RIGHT) Xvalue += 0.1;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the arrow keys to move the sphere and its bounding box." << endl
        << "Press r to reset their location." << endl
        << "Press the space bar to toggle between showing/hiding the bounding box." << endl;
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
   glutCreateWindow("occlusionConditionalRendering.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}
