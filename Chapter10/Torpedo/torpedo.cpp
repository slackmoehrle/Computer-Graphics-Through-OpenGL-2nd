/////////////////////////////////////////////////////////////         
// torpedo.cpp
//
// This program draws a torpedo composed of different pieces: 
// (i) Body: GLU cylinder 
// (ii) Nose: hemisphere
// (iii) Three fins: GLU partial disks
// (iv) Backside disk: GLU disk 
// (v) Propeller stem: GLU cylinder
// (vi) Three propeller blades: bicubic Bezier patches
// 
// Interaction:
// Press space to toggle between animation on and off.
// Press the x, X, y, Y, z, Z keys to rotate the viewpoint.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////

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
 
#define PI 3.14159265358979324

using namespace std;

// Begin globals. 
static unsigned int base; // Display lists base index.
static GLUquadricObj *qobj; // Pointer to GLU quadric object.

// Control points for a propeller blade bicubic Bezier patch.
static float controlPointsPropellerBlade[4][4][3] = 
{
	{{-0.2, 0.2, -0.05}, {-0.1, 0.0, -0.02}, {0.1, 0.0, 0.03}, {0.2,  0.2, 0.05}},
	{{-0.2, 0.4, -0.1}, {-0.1, 0.4, -0.05},  {0.1, 0.4, 0.05}, {0.2,  0.4, 0.1}},
	{{-0.2, 0.6, -0.2}, {-0.1, 0.6, -0.1},  {0.1, 0.6, 0.1}, {0.2, 0.6, 0.2}},
	{{-0.2, 0.8, -0.2}, {-0.1, 1.0, -0.1},  {0.1, 1.0, 0.1},  {0.2, 0.8, 0.2}}
};

static float Xangle = 30.0, Yangle = 10.0, Zangle = 0.0; // Angles to rotate torpedo.
static int isAnimate = 0; // In animation mode?
static int animationPeriod = 1; // Speed of animation.
static float angle = 0; // Angle of torpedo propeller turn.
// End globals.

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      angle += 10;
	  if (angle > 360.0) angle -= 360.0;

	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
}

// Initialization routine.
void setup(void) 
{
   
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Initialize GLU quadric object.
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_LINE);  

   // Generate display list base.
   base = glGenLists(2); 

   // Fin.
   glNewList (base, GL_COMPILE);
      glPushMatrix(); 
      glTranslatef(0.0, 0.5, 1.0);
	  glRotatef(-90.0, 0.0, 1.0, 0.0);
	  gluPartialDisk(qobj, 0.5, 1.0, 10, 5, 0.0, 90.0);
	  glPopMatrix();
   glEndList();

   // Propeller blade.
   glNewList (base+1, GL_COMPILE);
      glPushMatrix();
	  
	  // Enable Bezier surface: a bicubic Bezier patch.
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, controlPointsPropellerBlade[0][0]);
      glEnable(GL_MAP2_VERTEX_3);
      glMapGrid2f(5, 0.0, 1.0, 5, 0.0, 1.0);

      glTranslatef(0.0, 0.2, 0.0);

	  // Create Bezier surface.
      glEvalMesh2(GL_LINE, 0, 5, 0, 5);

	  glPopMatrix();
   glEndList();
}

// Routine to draw hemisphere.
void drawHemisphere(float radius, int longSlices, int latSlices)
{
   int  i, j;
   for(j = 0; j < latSlices; j++)
   {
      // One latitudinal triangle strip.
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i <= longSlices; i++)
      {
         glVertex3f( radius * cos( (float)(j+1)/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     radius * sin( (float)(j+1)/latSlices * PI/2.0 ),
					 radius * cos( (float)(j+1)/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );
         glVertex3f( radius * cos( (float)j/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     radius * sin( (float)j/latSlices * PI/2.0 ),
					 radius * cos( (float)j/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );         
	  }
      glEnd();
   }
}

// Drawing routine.
void drawScene(void)
{
   glListBase(base); 
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(0.0, 0.0, 0.0);
   
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);
 
   // Torpedo body.
   gluCylinder(qobj, 0.5, 0.5, 3.0, 15, 15);

   // Torpedo nose.
   glPushMatrix();
   glTranslatef(0.0, 0.0, 3.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   drawHemisphere(0.5, 10, 10);
   glPopMatrix();

   // Three fins.
   glPushMatrix();
   glCallList(base);
   glRotatef(120.0, 0.0, 0.0, 1.0);
   glCallList(base);
   glRotatef(120.0, 0.0, 0.0, 1.0);
   glCallList(base);
   glPopMatrix();

   // Backside disk.
   gluDisk(qobj, 0.0, 0.5, 10, 10);

   // Propeller stem.
   glPushMatrix();
   glTranslatef(0.0, 0.0, -0.5);
   gluCylinder(qobj, 0.2, 0.2, 0.5, 5, 5); 
   glPopMatrix();

   // Three propeller blades.
   glPushMatrix();
   glTranslatef(0.0, 0.0, -0.5);
   glRotatef(angle, 0.0, 0.0, 1.0);
   glCallList(base+1);
   glRotatef(120.0, 0.0, 0.0, 1.0);
   glCallList(base+1);
   glRotatef(120.0, 0.0, 0.0, 1.0);
   glCallList(base+1);
   glPopMatrix();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
		 }
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
   cout << "Press space to toggle between animation on and off." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the viewpoint." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("torpedo.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
