///////////////////////////////////////////////////////////////////////////      
// ballAndTorusPicking.cpp
//
// This program, based on ballAndTorus.cpp illustrates picking with 
// use of the OpenGL pick matrix and selection mechanisms.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Click left mouse button in OpenGL window to select either ball or torus.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////

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
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static int highlightFrames = 10; // Number of frames to keep highlight.
static int isSelecting = 0; // In selection mode?
static int hits; // Number of entries in hit buffer.
static unsigned int buffer[1024]; // Hit buffer.
static unsigned int closestName = 0; // Name of closest hit.
 
// Drawing routine.
void drawBallAndTorus(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -25.0);
       
   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Fixed torus.
   if (isSelecting) glLoadName(1); // Name of torus is 1.
   if ((highlightFrames > 0) && (closestName == 1)) glColor3f(1.0, 0.0, 0.0); // Highlight if selected.
   else glColor3f(0.0, 1.0, 0.0);
   glutWireTorus(2.0, 12.0, 20, 20);

   // Begin revolving ball.
   glRotatef(longAngle, 0.0, 0.0, 1.0);
   
   glTranslatef(12.0, 0.0, 0.0);
   glRotatef(latAngle, 0.0, 1.0, 0.0);
   glTranslatef(-12.0, 0.0, 0.0);
   
   glTranslatef(20.0, 0.0, 0.0);

   if (isSelecting) glLoadName(2); // Name of ball is 2. 
   if ((highlightFrames > 0) && (closestName == 2)) glColor3f(1.0, 0.0, 0.0); // Highlight if selected. 
   else glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(2.0, 10, 10);
   // End revolving ball.

   if (isSelecting) glPopName(); // Clear name stack.

   glutSwapBuffers();
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   
   // Draw ball and torus in rendering mode.
   isSelecting = 0;
   drawBallAndTorus();
}

// Process hit buffer to find record with smallest min-z value.
void findClosestHit(int hits, unsigned int buffer[])
{
   unsigned int *ptr, minZ;
   
   minZ= 0xffffffff; // 2^32 - 1
   ptr = buffer;
   closestName = 0;
   for (int i = 0; i < hits; i++)					
   {
      ptr++;
	  if (*ptr < minZ) 
	  {
	     minZ = *ptr;
		 ptr += 2;
		 closestName = *ptr;
		 ptr++;
	  }
	  else ptr += 3;
   }
   if (closestName != 0) highlightFrames = 10;
}

// The mouse callback routine.
void pickFunction(int button, int state, int x, int y)
{
   int viewport[4]; // Viewport data.

   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return; // Don't react unless left button is pressed.
   
   glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data.

   glSelectBuffer(1024, buffer); // Specify buffer to write hit records in selection mode
   (void) glRenderMode(GL_SELECT); // Enter selection mode.

   // Save the viewing volume defined in the resize routine.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   
   // Define a viewing volume corresponding to selecting in 3 x 3 region around the cursor.
   glLoadIdentity();
   gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); // Copied from the reshape routine.
   
   glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
   glLoadIdentity();

   glInitNames(); // Initializes the name stack to empty.
   glPushName(0); // Puts name 0 on top of stack.

   // Determine hits by calling drawBallAndTorus() so that names are assigned.
   isSelecting = 1;
   drawBallAndTorus();

   hits = glRenderMode(GL_RENDER); // Return to rendering mode, returning number of hits.

   // Restore viewing volume of the resize routine and return to modelview mode.
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

   // Determine closest of the hit objects (if any).
   findClosestHit(hits, buffer);

   glutPostRedisplay();
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      latAngle += 5.0;
	  if (latAngle > 360.0) latAngle -= 360.0;
      longAngle += 1.0;
	  if (longAngle > 360.0) longAngle -= 360.0;
   }
   if (highlightFrames > 0) highlightFrames--;
   
   glutPostRedisplay();
   glutTimerFunc(animationPeriod, animate, 1);
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);  
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   glutTimerFunc(5, animate, 1);
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
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
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

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}


// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl << endl
        << "Click left mouse button in OpenGL window to select either ball or torus." << endl;
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
   glutCreateWindow ("ballAndTorusPicking.cpp"); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMouseFunc(pickFunction); 

   glewExperimental = GL_TRUE; 
   glewInit(); 

   setup(); 

   glutMainLoop();  
}

