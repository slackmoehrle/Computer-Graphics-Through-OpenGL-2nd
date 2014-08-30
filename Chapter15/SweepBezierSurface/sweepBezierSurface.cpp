//////////////////////////////////////////////////////////////////////////////////
// sweepBezierSurface.cpp
// 
// This program illustrates how a Bezier surface is swept by a Bezier curve.
//
// Interaction: 
// Press the left/right or up/down keys to move the sweeping curve. 
// Press the space key to toggle between sweeping the surface in either direction.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////

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
// Surface control points.
static float ctrlpointsSurf[4][4][3] = 
{
   { {-3.0, -2.0, 4.0},
     {-1.0, 2.0, 4.0},
     {1.0, -2.0, 4.0},
     {3.0, 2.0, 4.0}},
   { {-3.0, -5.0, 3.0},
     {-1.0, -1.0, 3.0},
     {1.0, -5.0, 3.0},
     {3.0, -1.0, 3.0}},
   { {-3.0, -2.0, 2.0},
     {-1.0, 2.0, 2.0},
     {1.0, -2.0, 2.0},
     {3.0, 2.0, 2.0}},
   { {-3.0, -5.0, 1.0},
     {-1.0, -1.0, 1.0},
     {1.0, -5.0, 1.0},
     {3.0, -1.0, 1.0}}
};

static float t = 0.0; // Parameter of sweeping curve.
static float ctrlpointsSurfTemp[4][4][3]; // Array to store surface control points.
static float ctrlpointsSweepCurv[4][3]; // Control points of the sweeping curve.
static float ctrlpointsCurv0[4][3]; // Control points of first fixed curve.
static float ctrlpointsCurv1[4][3]; // Control points of second fixed curve.
static float ctrlpointsCurv2[4][3]; // Control points of third fixed curve.
static float ctrlpointsCurv3[4][3]; // Control points of fourth fixed curve.
static int leftright = 1; // Sweep direction.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
// End globals.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
}

// Function to compute control points of the fixed curves.
void computeFixedCurvs(void)
{
   int i, j;

   for (i = 0; i < 4; i++)
      for (j = 0; j < 3; j++)
	  {
	     ctrlpointsCurv0[i][j] = ctrlpointsSurf[i][0][j];
		 ctrlpointsCurv1[i][j] = ctrlpointsSurf[i][1][j];
	     ctrlpointsCurv2[i][j] = ctrlpointsSurf[i][2][j];
		 ctrlpointsCurv3[i][j] = ctrlpointsSurf[i][3][j];
	  }
}

// Switch rows and columns of surface control points array.
void flipCtrlpointsSurf(void)
{
   int i, j, k;

   for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++)
	     for (k = 0; k < 3; k++)
		    ctrlpointsSurfTemp[i][j][k] = ctrlpointsSurf[j][i][k];
   for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++)
	     for (k = 0; k < 3; k++)
		    ctrlpointsSurf[i][j][k] = ctrlpointsSurfTemp[i][j][k];
}

// Function to compute the control points of the sweeping curve
// which moves with global parameter t.
void computeSweepCurv(void)
{
   float s = 1.0 - t;
   int i, j;

   for (j = 0; j < 3; j++)
      for (i = 0; i < 4; i++)
	     ctrlpointsSweepCurv[i][j] =     
		      s * s * s * ctrlpointsSurf[0][i][j]
			+ 3 * s * s * t * ctrlpointsSurf[1][i][j]
			+ 3 * s * t * t * ctrlpointsSurf[2][i][j]
			+ t * t * t * ctrlpointsSurf[3][i][j];
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   computeSweepCurv();
   computeFixedCurvs();
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Write u value.
   glColor3f(0.0, 0.0, 0.0);
   floatToString(theStringBuffer, 4, t);
   glRasterPos3f(-8.0, 5.0, 0.0);
   if (leftright)
      writeBitmapString((void*)font, "v = ");
   else
      writeBitmapString((void*)font, "u = ");
   writeBitmapString((void*)font, theStringBuffer);

   glPushMatrix();
   glTranslatef(0.0, 2.0, 0.0);
   glRotatef(75.0, 0.0, 0.0, 1.0);
   
   // Draw surface control points.
   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
	     glVertex3fv(ctrlpointsSurf[i][0]);
   glEnd();
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
	     glVertex3fv(ctrlpointsSurf[i][1]);
   glEnd();
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
			glVertex3fv(ctrlpointsSurf[i][2]);
   glEnd();
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
	     glVertex3fv(ctrlpointsSurf[i][3]);
   glEnd();

   // Draw the partially swept surface.
   glColor3f(0.5, 0.5, 0.5);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpointsSurf[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(50, 0.0, 1.0, 50, 0.0, 1.0);
   glEvalMesh2(GL_LINE, 0, 50, 0, (int)(t*50.0) );

   // Draw first fixed curve.
   glColor3f(1.0, 1.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &ctrlpointsCurv0[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(30, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 30);
   glColor3f(1.0, 1.0, 1.0);

   // Draw second fixed curve.
   glColor3f(1.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &ctrlpointsCurv1[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(30, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 30);
   glColor3f(1.0, 1.0, 1.0);

   // Draw third fixed curve.
   glColor3f(0.0, 1.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &ctrlpointsCurv2[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(30, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 30);
   glColor3f(1.0, 1.0, 1.0);

   // Draw fourth fixed curve.
   glColor3f(0.0, 0.0, 1.0);
   glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &ctrlpointsCurv3[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(30, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 30);
   glColor3f(1.0, 1.0, 1.0);

   // Draw control points of sweeping curve.
   glPointSize(8.0);
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_POINTS);
      glColor3f(1.0, 1.0, 0.0);
	  glVertex3fv(ctrlpointsSweepCurv[0]);
	  glColor3f(1.0, 0.0, 0.0);
	  glVertex3fv(ctrlpointsSweepCurv[1]);
      glColor3f(0.0, 1.0, 0.0);
	  glVertex3fv(ctrlpointsSweepCurv[2]);
      glColor3f(0.0, 0.0, 1.0);
	  glVertex3fv(ctrlpointsSweepCurv[3]);
   glEnd();

   // Draw sweeping curve.
   glColor3f(0.0, 0.0, 0.0);
   glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &ctrlpointsSweepCurv[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
   glMapGrid1f(30, 0.0, 1.0);
   glEvalMesh1(GL_LINE, 0, 30);
   glLineWidth(1.0);

   glPopMatrix();
   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   gluLookAt(4.0, 5.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case ' ':
         flipCtrlpointsSurf();
         computeSweepCurv();
         computeFixedCurvs();
		 leftright = 1 - leftright;
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
   if (leftright == 1)
   {
      if(key == GLUT_KEY_LEFT) 
	     if (t > 0.01) t -= 0.02;
	  if(key == GLUT_KEY_RIGHT)
	     if (t < 0.98) t += 0.02;
   }
   if (leftright == 0)
   {
	  if(key == GLUT_KEY_DOWN) 
	     if (t > 0.01) t -= 0.02;
	  if(key == GLUT_KEY_UP)
	     if (t < 0.98) t += 0.02;
   }
   computeSweepCurv();
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left/right or up/down keys to move the sweeping curve." << endl 
        << "Press the space key to toggle between sweeping the surface in either direction." << endl; 
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
   glutCreateWindow("sweepBezierSurface.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();   
}