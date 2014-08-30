////////////////////////////////////////////////////////////////////////////////////////
// hermiteCubic.cpp
// 
// This program draws a Hermite cubic allowing the user control over the two end control 
// points and the tangent vectors there.
//
// Interaction:
// Press space to select a control point or tangent vector.
// Press the arrow keys to move the selected control point or change the tangent vector.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////////////////////////////

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

using namespace std;

// Begin globals.
static int numVal = 0; // Current selection index.
static float lengthArrowLine = 2.0; // Length of arrow line.
static int numVertices = 50; // Number of vertices on cubic.

// Control points.
static float controlPoints[2][3] = 
{
	{ -20.0, 0.0, 0.0}, { 20.0, 0.0, 0.0}
};

// Tangent vectors at control points.
static float tangentVectors[2][3] = 
{
	{ 0.0, 10.0, 0.0}, { 10.0, 0.0, 0.0}
};

// Lengths of tangent vectors.
static float squareLengthTangent[2];

// End points of tangent vectors.
static float endPointTangentVectors[2][3];

// End points of arrow lines.
static float endPointArrowLines[4][3];;
// End globals.

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Routine to compute tangent vector endpoints by adding tangent vector to control point vector.
void computeEndPointTangentVectors(void)
{
   for (int i = 0; i < 2; i++) 
      for (int j = 0; j < 3; j++)
         endPointTangentVectors[i][j] = controlPoints[i][j] + tangentVectors[i][j];
}

// Routine to compute arrow line endpoints.
void computeEndPointArrowLines(void)
{
   for (int i = 0; i < 2; i++)
   {	   
      squareLengthTangent[i] = tangentVectors[i][0]*tangentVectors[i][0] +
	                           tangentVectors[i][1]*tangentVectors[i][1];
	  
	  if (squareLengthTangent[i] != 0.0)
	  {

         endPointArrowLines[2*i][0] = endPointTangentVectors[i][0] -
	                                  lengthArrowLine *
		 					          (tangentVectors[i][0] - tangentVectors[i][1]) /
							          sqrt(2.0 * squareLengthTangent[i]);
         endPointArrowLines[2*i][1] = endPointTangentVectors[i][1] -
	                                  lengthArrowLine *
							          (tangentVectors[i][0] + tangentVectors[i][1]) /
							          sqrt(2.0 * squareLengthTangent[i]);
         endPointArrowLines[2*i][2] = 0.0; 

         endPointArrowLines[2*i+1][0] = endPointTangentVectors[i][0] -
	                                    lengthArrowLine *
							            (tangentVectors[i][0] +tangentVectors[i][1]) /
							            sqrt(2.0 * squareLengthTangent[i]);
         endPointArrowLines[2*i+1][1] = endPointTangentVectors[i][1] -
	                                    lengthArrowLine *
							            (-tangentVectors[i][0] + tangentVectors[i][1]) /
							            sqrt(2.0 * squareLengthTangent[i]);
         endPointArrowLines[2*i+1][2] = 0.0;
	  }
   }
}
   
// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   int i;
   float u, H0, H1, H2, H3;

   computeEndPointTangentVectors(); // Compute tangent vector endpoints.
   computeEndPointArrowLines(); // Compute arrow endpoints.
   

   // Draw the control points as dots.
   glPointSize(5.0);	  
   glColor3f(0.0, 1.0, 0.0); 
   glBegin(GL_POINTS);
   for (i = 0; i < 2; i++) 
	  glVertex3fv(controlPoints[i]);
   glEnd();

   // Draw the tangent vectors as arrows.
   for (i = 0; i < 2; i++) 
   {
      if (squareLengthTangent[i] != 0.0)
	  {
		 glBegin(GL_LINES);
	        glVertex3fv(controlPoints[i]);
	        glVertex3fv(endPointTangentVectors[i]);

	        glVertex3fv(endPointTangentVectors[i]);
            glVertex3fv(endPointArrowLines[2*i]);

	        glVertex3fv(endPointTangentVectors[i]);
            glVertex3fv(endPointArrowLines[2*i+1]);
		 glEnd();
	  }
	  else
	  {
         glBegin(GL_POINTS);
	        glVertex3fv(controlPoints[i]);
         glEnd();
	  }
   }

   // Highlight selected control point or tangent vector,
   glColor3f(1.0, 0.0, 0.0); 
   if (numVal%2 == 0)
   {
      glBegin(GL_POINTS);
	     glVertex3fv(controlPoints[numVal/2]);     
      glEnd();
   }
   else
   {
      i = numVal/2;
      if (squareLengthTangent[i] != 0.0)
	  {
		 glBegin(GL_LINES);
	        glVertex3fv(controlPoints[i]);
	        glVertex3fv(endPointTangentVectors[i]);

	        glVertex3fv(endPointTangentVectors[i]);
            glVertex3fv(endPointArrowLines[2*i]);

	        glVertex3fv(endPointTangentVectors[i]);
            glVertex3fv(endPointArrowLines[2*i+1]);
		 glEnd();
	  }
	  else
	  {
         glBegin(GL_POINTS);
	        glVertex3fv(controlPoints[i]);
         glEnd();
	  }
   }

   // Draw the cubic curve as a line strip.
   glColor3f(0.0, 0.0, 0.0);  
   glBegin(GL_LINE_STRIP);
      for(i = 0; i <= numVertices; ++i)
	  {
         u = (float)i/numVertices;
		 H0 = 2.0*u*u*u - 3*u*u + 1.0;
		 H1 = -2.0*u*u*u + 3*u*u;
		 H2 = u*u*u - 2.0*u*u + u;
		 H3 = u*u*u - u*u;
         glVertex3f(H0*controlPoints[0][0] + H1*controlPoints[1][0] +
			        H2*tangentVectors[0][0] + H3*tangentVectors[1][0], 
                    H0*controlPoints[0][1] + H1*controlPoints[1][1] +
			        H2*tangentVectors[0][1] + H3*tangentVectors[1][1], 					
					0.0);
	  }
   glEnd();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:	
         exit(0);
         break;
      case ' ':
		 if (numVal < 3) numVal++; else numVal = 0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP)
   {
      if (numVal%2 == 0) controlPoints[numVal/2][1] += 0.5;
	  else tangentVectors[numVal/2][1] += 0.5;
   }
   if(key == GLUT_KEY_DOWN) 
   {
      if (numVal%2 == 0) controlPoints[numVal/2][1] -= 0.5;
	  else tangentVectors[numVal/2][1] -= 0.5;
   }
   if(key == GLUT_KEY_LEFT) 
   {
      if (numVal%2 == 0) controlPoints[numVal/2][0] -= 0.5;
	  else tangentVectors[numVal/2][0] -= 0.5;
   }
   if(key == GLUT_KEY_RIGHT)
   {
      if (numVal%2 == 0) controlPoints[numVal/2][0] += 0.5;
	  else tangentVectors[numVal/2][0] += 0.5;
   }

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to select a control point or tangent vector." << endl
        << "Press the arrow keys to move the selected control point or" << endl
		<< "change the tangent vector." << endl; 
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
   glutCreateWindow("hermiteCubic.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}

