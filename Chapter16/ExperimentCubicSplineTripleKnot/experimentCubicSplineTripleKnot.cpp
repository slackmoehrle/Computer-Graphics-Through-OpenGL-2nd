/////////////////////////////////////////////////////////////////////////////////////
// experimentCubicSplineTripleKnot.cpp
// (modifying cubicSplineCurve1.cpp)
//
// Interaction: 
// Press 'c' to enter control points mode -
// Press the shift key to cycle through the control points.
// The selected control point (in red) is moved using the arrow keys.
// Press delete to reset control points.
//
// Press 'k' to enter knots mode -
// Press the shift key to cycle through the knots.
// The selected knot (in red) is increased/decreased using the right/left arrow keys.
// Press delete to reset knot values.
// 
//Sumanta Guha
/////////////////////////////////////////////////////////////////////////////////////

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
static int changeControls = 1; // If control points to be changed.
static int selectedControlPoint = 0; // Selected control point number.
static int selectedKnot = 0; // Selected knot number.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Control points.
static float controlPoints[9][3] = 
{
	{-40.0, -20.0, 0.0}, {-30.0, 30.0, 0.0}, {-20.0, -20.0, 0.0},
	{-10.0, 30.0, 0.0}, {0.0, -20.0, 0.0}, {10.0, 30.0, 0.0},
	{20.0, -20.0, 0.0}, {30.0, 30.0, 0.0}, { 40.0, -20.0, 0.0}
};

// Control point values stored unchanged for use on reset.
static float originalControlPoints[9][3] = 
{
	{-40.0, -20.0, 0.0}, {-30.0, 30.0, 0.0}, {-20.0, -20.0, 0.0},
	{-10.0, 30.0, 0.0}, {0.0, -20.0, 0.0}, {10.0, 30.0, 0.0},
	{20.0, -20.0, 0.0}, {30.0, 30.0, 0.0}, { 40.0, -20.0, 0.0}
};

// Knots.
static float knots[13] = 
{0.0, 6.0, 12.0, 18.0, 24.0, 30.0, 36.0, 42.0, 48.0, 54.0, 60.0, 66.0, 72.0};

static GLUnurbsObj *nurbsObject; // Pointer to NURBS object.
// End globals.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Initialization routine.		
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Create NURBS object.
   nurbsObject = gluNewNurbsRenderer();
   gluNurbsProperty(nurbsObject, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
   gluNurbsProperty(nurbsObject, GLU_SAMPLING_TOLERANCE, 10.0);
}

// Function to increase value of a knot.
void increaseKnot(int i)
{
   if ( (i < 12) )
   {
      if (knots[i] < knots[i+1]) knots[i] += 1.0;
	  else 
	  {
	     increaseKnot(i+1);
	     if (knots[i] < knots[i+1]) knots[i] += 1.0;
	  }
   }
   if ( (i == 12 )  && ( knots[i] < 80.0) )    knots[i] += 1.0;
}

// Function to decrease value of a knot.
void decreaseKnot(int i)
{
   if ( (i > 0) )
   {
      if (knots[i] > knots[i-1]) knots[i] -= 1.0;
	  else 
	  {
	     decreaseKnot(i-1);
		 if (knots[i] > knots[i-1]) knots[i] -= 1.0;
	  }
   }
   if ( (i == 0 )  && ( knots[i] > 0.0) )    knots[i] -= 1.0;
}

// Reset knot values.
void resetKnots(void)
{
   int i;
   for (i = 0; i < 13; i++) knots[i] = 6.0*i;
   selectedKnot = 0;
}

// Reset control points.
void resetControlPoints(void)
{
   int i,j;
   for (i = 0; i < 9; i++) for (j = 0; j < 3; j++) controlPoints[i][j] = originalControlPoints[i][j]; 
   selectedControlPoint = 0;
}

// Recursive computation of B-spline functions.
float Bspline(int i, int m, float u)
{
   float coef1, coef2;
   if ( m == 1 )
   {
	  if ( i == 0 ) if ( ( knots[i] <= u ) && ( u <= knots[i+1] ) ) return 1.0;
      if ( ( knots[i] < u ) && ( u <= knots[i+1] ) ) return 1.0;
	  else return 0.0;
   }
   else
   {
      if ( knots[i+m-1] == knots[i] ) 
	  {
	     if ( u == knots[i] ) coef1 = 1;
		 else coef1 = 0;
	  }
	  else coef1 = (u - knots[i])/(knots[i+m-1] - knots[i]);

      if ( knots[i+m] == knots[i+1] )
	  {
		 if ( u == knots[i+m] ) coef2 = 1;
		 else coef2 = 0;
	  }
	  else coef2 = (knots[i+m] - u)/(knots[i+m] - knots[i+1]);
		
      return ( coef1 * Bspline(i, m-1, u) + coef2 * Bspline(i+1 ,m-1 ,u) );
   }
}

// Drawing routine.
void drawScene(void)
{
   int i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
   
   // Write mode messsage.
   glColor3f(0.0, 0.0, 0.0);
   if (changeControls == 1) 
   {
      glRasterPos3f(-12.5, 45.0, 0.0);
      writeBitmapString((void*)font, "Control points mode");
   }
   else
   {
      glRasterPos3f(-5.5, 45.0, 0.0);
      writeBitmapString((void*)font, "Knots mode");
   }

   // Draw the control polygon in light gray.
   glColor3f(0.8, 0.8, 0.8);
   glBegin(GL_LINE_STRIP);
   for (i = 0; i < 9; i++) 
      glVertex3fv(controlPoints[i]);
   glEnd();

   // Draw the spline curve.
   glColor3f(0.0, 0.0, 0.0);
   gluBeginCurve(nurbsObject);
   gluNurbsCurve(nurbsObject, 13, knots, 3, controlPoints[0], 4, GL_MAP1_VERTEX_3);
   gluEndCurve(nurbsObject);

   // The following code displays the control points as dots.
   glPointSize(5.0);
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 9; i++) 
         glVertex3fv(controlPoints[i]);
   glEnd();

   // Highlight the selected control point.
   if (changeControls == 1)
   {
      glColor3f(1.0, 0.0, 0.0);
	  glBegin(GL_POINTS);
	     glVertex3fv(controlPoints[selectedControlPoint]);     
	  glEnd();
   }
 
   // Draw horizontal bars corresponding to knot points.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      for (i = 0; i < 13; i++) 
	  {
        glVertex3f(-40.0, -45.0 + (float)i, 0.0);
        glVertex3f( 40.0, -45.0 + (float)i, 0.0);
	  }
   glEnd();

   // Draw the knot points as dots on their respective bars.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 13; i++) 
         glVertex3f( -40.0 + knots[i], -45.0 + (float)i, 0.0);
   glEnd();

      // Label the knot axis.
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(-40.5, -48.0, 0.0);
   writeBitmapString((void*)font, "0");
   glRasterPos3f(-34.5, -48.0, 0.0);
   writeBitmapString((void*)font, "1");
   glRasterPos3f(-28.5, -48.0, 0.0);
   writeBitmapString((void*)font, "2");
   glRasterPos3f(-22.5, -48.0, 0.0);
   writeBitmapString((void*)font, "3");
   glRasterPos3f(-16.5, -48.0, 0.0);
   writeBitmapString((void*)font, "4");
   glRasterPos3f(-10.5, -48.0, 0.0);
   writeBitmapString((void*)font, "5");
   glRasterPos3f(-4.5, -48.0, 0.0);
   writeBitmapString((void*)font, "6");
   glRasterPos3f(2.5, -48.0, 0.0);
   writeBitmapString((void*)font, "7");
   glRasterPos3f(8.5, -48.0, 0.0);
   writeBitmapString((void*)font, "8");
   glRasterPos3f(14.5, -48.0, 0.0);
   writeBitmapString((void*)font, "9");
   glRasterPos3f(20.0, -48.0, 0.0);
   writeBitmapString((void*)font, "10");
   glRasterPos3f(26.0, -48.0, 0.0);
   writeBitmapString((void*)font, "11");
   glRasterPos3f(32.0, -48.0, 0.0);
   writeBitmapString((void*)font, "12");

   // Highlight the selected knot point.
   if (changeControls == 0)
   {
      glColor3f(1.0, 0.0, 0.0);
	  glBegin(GL_POINTS);
	     glVertex3f( -40.0 + knots[selectedKnot], -45.0 + (float)selectedKnot, 0.0);     
	  glEnd();
   }

   // Draw images of knot points (i.e., joints) on the curve.
   glPointSize(5.0);
   glColor3f(0.0, 0.0, 1.0);
   float knotImageX, knotImageY;
   glBegin(GL_POINTS);
      for (i = 3; i < 10; i++) 
	  {
		 knotImageX = knotImageY = 0;
		 for (j = 0; j < 9; j++)
		 {
	        knotImageX += Bspline(j, 4, knots[i]) * controlPoints[j][0];
		    knotImageY += Bspline(j, 4, knots[i]) * controlPoints[j][1];
		 }
         glVertex3f(knotImageX, knotImageY, 0.0);
	  }
   glEnd();

   // Highlight the image of the selected knot point (if its within the parameter space).
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
   knotImageX = knotImageY = 0;
   if ( (3 <= selectedKnot) && (selectedKnot <= 9) )
   {
      for (j = 0; j < 9; j++)
	  {
         knotImageX += Bspline(j, 4, knots[selectedKnot]) * controlPoints[j][0];
         knotImageY += Bspline(j, 4, knots[selectedKnot]) * controlPoints[j][1];
	  }
   glVertex3f(knotImageX, knotImageY, 0.0);
   }   
   glEnd();

   glPopMatrix();
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
      case 'k':
         if (changeControls == 1)  changeControls = 0;
         glutPostRedisplay();
         break;
      case 'c':
         if (changeControls == 0)  changeControls = 1;
         glutPostRedisplay();
         break;
      case ' ':
         if (changeControls == 1)
		 {
		    if (selectedControlPoint < 8) selectedControlPoint++;
		    else selectedControlPoint = 0;
		 }
	     else
		 {
		    if (selectedKnot < 12) selectedKnot++;
		    else selectedKnot = 0;
		 }
         glutPostRedisplay();
         break;
      case 127:	
         if (changeControls == 1) resetControlPoints();
		 else resetKnots();
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
      if (changeControls == 1) controlPoints[selectedControlPoint][1] += 1.0;
   if(key == GLUT_KEY_DOWN)
      if (changeControls == 1) controlPoints[selectedControlPoint][1] -= 1.0;
   if(key == GLUT_KEY_LEFT)
   {
      if (changeControls == 1) controlPoints[selectedControlPoint][0] -= 1.0;
	  else decreaseKnot(selectedKnot);
   }
   if(key == GLUT_KEY_RIGHT)
   {
      if (changeControls == 1) controlPoints[selectedControlPoint][0] += 1.0;
	  else increaseKnot(selectedKnot);
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press 'c' to enter control points mode -" << endl
        << "Press the shift key to cycle through the control points." << endl
        << "The selected control point (in red) is moved using the arrow keys." << endl
        << "Press delete to reset control points." << endl
        << "" << endl
        << "Press 'k' to enter knots mode -" << endl
        << "Press the shift key to cycle through the knots." << endl
        << "The selected knot (in red) is increased/decreased using the right/left arrow keys." << endl
        << "Press delete to reset knot values." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
  
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("experimentCubicSplineTripleKnot.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}

