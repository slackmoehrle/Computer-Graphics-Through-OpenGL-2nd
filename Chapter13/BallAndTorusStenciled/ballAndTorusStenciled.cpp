///////////////////////////////////////////////////////////////////////////////////////////// 
// ballAndTorusStenciled.cpp
//
// This program, based on ballAndTorusReflected.cpp, demonstrates use of the stencil buffer
// by replacing the reflective checkered floor with a non-reflective surface and a movable
// reflective disc. Reflections are seen only in the disc.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the arrow keys to move the reflective disc.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////////////// 
				          
#include <iostream>
#include <cmath>

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
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static int isAnimate = 0; // Animated?
static int animationPeriod = 75; // Time interval between frames.
static float xVal = 0.0, zVal = 12.0; // Displacement parameters of disc.

// Draw ball flying around torus.
void drawFlyingBallAndTorus(void)
{
   
   glPushMatrix();

   glTranslatef(0.0, 10.0, -15.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);

   // Fixed torus.
   glColor4f(0.0, 1.0, 0.0, 1.0); // High alpha for opacity.
   glutSolidTorus(2.0, 12.0, 80, 80);

      // Begin revolving ball.
   glRotatef(longAngle, 0.0, 0.0, 1.0);
   
   glTranslatef(12.0, 0.0, 0.0);
   glRotatef(latAngle, 0.0, 1.0, 0.0);
   glTranslatef(-12.0, 0.0, 0.0);
   
   glTranslatef(20.0, 0.0, 0.0);

   glColor4f(0.0, 0.0, 1.0, 1.0); // High alpha for opacity.
   glutSolidSphere(2.0, 20, 20);
   // End revolving ball.

   glPopMatrix();
}

// Draw reflective disc centered at the input position.
void drawReflectiveDisc(float x, float z)
{
   glPushMatrix();
   glTranslatef(x, 0.0, z);
   glColor4f(0.0, 0.0, 0.0, 0.5); // Low alpha for blending.
   glNormal3f(0.0, 1.0, 0.0);
   glBegin(GL_POLYGON);
      for(int i = 0; i < 100; ++i)
	  {
         float t = 2*PI*i/100;
         glVertex3f(7.5*sin(t), 0.0, 7.5*cos(t));
	  }
   glEnd();
   glPopMatrix();
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

	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
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
   float lightPos[] = {0.0, 1.0, 0.0, 0.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	// Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters. 

   glClearStencil(0); // Set clearing value for stencil buffer.
   glEnable(GL_STENCIL_TEST); // Enable stencil testing.
}

// Drawing routine.
void drawScene(void)
{
   int i = 0;

   // Clear color, depth *and* stencil buffer.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 5.0, 30.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
 
   // Disable writing of the frame and depth buffers as only the 
   // stencil buffer need be written next.
   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
   glDepthMask(GL_FALSE);
   
   // Create a mask of the disc in the stencil buffer - with 1's in the stencil tags corresponding
   // to the disc and 0's elsewhere - by drawing the disc after setting glStencilFunc() and
   // glStencilOp() appropriately.
   glStencilFunc(GL_ALWAYS, 1, 1); // The stencil test always passes (the reference value and
                                   // mask are both 1).
   glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); // In all cases replace the stencil tag
                                                    // value with the reference value 1.
   drawReflectiveDisc(xVal, zVal); // Draw the disc - as writing the frame and depth buffers 
                                   // has been disabled, only the stencil buffer is updated.

   // Enable writing of the frame and depth buffers - actually drawing now begins.
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
   glDepthMask(GL_TRUE);

   // For the reflections of the ball and torus drawing is allowed only in the
   // area corresponding to the mask of the disc in the stencil buffer.
   glStencilFunc(GL_EQUAL, 1, 1); // The stencil test passes only if the corresponding
                                  // stencil buffer tag is 1.
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // The stencil buffer itself is not updated.	

   // Draw a reflection of the real ball and torus by flipping them about the xz-plane
   // using a scaling transformation.
   glPushMatrix();
   glScalef(1.0, -1.0, 1.0);
   glFrontFace(GL_CW); // Because of reflection front-faces are drawn clockwise. 
   drawFlyingBallAndTorus();
   glFrontFace(GL_CCW);  
   glPopMatrix();
   
   // Blend the disc onto the reflection. Doesn't matter if the stencil test is still
   // active as, by definition, the disc is drawn in the area of its own mask.
   drawReflectiveDisc(xVal, zVal);

   // The non-reflective floor is drawn outside the area corresponding to the disc.
   glStencilFunc(GL_NOTEQUAL, 1, 1); // The stencil test passes only if the corresponding
                                     // stencil buffer pixel is not 1.	
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // The stencil buffer itself is not updated.	

   // Draw a quad as the floor.
   glColor4f(1.0, 0.0, 0.0, 1.0); // High alpha for opacity.
   glNormal3f(0.0, 1.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(-50.0, 0.0, -50.0);
      glVertex3f(-50.0, 0.0, 50.0);
      glVertex3f(50.0, 0.0, 50.0);
      glVertex3f(50.0, 0.0, -50.0);
   glEnd();
  
   // The real ball and torus are drawn without regard to the stencil buffer.
   glStencilFunc(GL_ALWAYS, 1, 1); // The stencil test always passes 
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // The stencil buffer itself is not updated.	
   
   // Draw real ball and torus.
   drawFlyingBallAndTorus();

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
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
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
		 }
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) zVal += 0.5;
   if( key == GLUT_KEY_UP) zVal -= 0.5;
   if (key == GLUT_KEY_RIGHT) xVal += 0.5;
   if( key == GLUT_KEY_LEFT) xVal -= 0.5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the arrow keys to move the reflective disc." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL); // Note stencil buffer initialized
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("ballAndTorusStenciled.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
