///////////////////////////////////////////////////////////////////////////////////
// experimentBsplinesLinear.cpp
// (modifying bSplines.cpp)
// 
// Interaction:
// Press the up/down arrow keys to cycle between order 1 through 4.
// Press space to select a knot points.
// Press the left/right arrow keys to move the selected knot point.
// Press delete to reset knot values.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////

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
static int selectedKnot = 0; // Selected knot number.
static int splineOrder = 1; // Order of spline.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Knot values scaled by a factor of 10 to avoid floating point error when comparing knot values.
static float knots[9] = {0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0}; 	
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
}

// Function to increase value of a knot.
void increaseKnot(int i)
{
   if ( (i < 8) )
   {
      if (knots[i] < knots[i+1]) knots[i] += 1.0;
	  else 
	  {
	     increaseKnot(i+1);
		 if (knots[i] < knots[i+1]) knots[i] += 1.0;
	  }
   }
   if ( (i == 8 )  && ( knots[i] < 80) ) knots[i] += 1.0;
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
   if ( (i == 0 )  && ( knots[i] > 0) ) knots[i] -= 1.0;
}

// Reset knot values.
void resetKnots(void)
{
   int i;
   for (i = 0; i < 9; i++) knots[i] = 10.0*i;
   selectedKnot = 0;
}

// Recursive computation of B-spline functions.
float Bspline(int index, int order, float u)
{
   float coef1, coef2;
   if ( order == 1 )
   {
	  if ( index == 0 ) if ( ( knots[index] <= u ) && ( u <= knots[index+1] ) ) return 1.0;
      if ( ( knots[index] < u ) && ( u <= knots[index+1] ) ) return 1.0;
	  else return 0.0;
   }
   else
   {
      if ( knots[index + order - 1] == knots[index] ) 
	  {
	     if ( u == knots[index] ) coef1 = 1;
		 else coef1 = 0;
	  }
	  else coef1 = (u - knots[index])/(knots[index + order - 1] - knots[index]);

      if ( knots[index + order] == knots[index+1] )
	  {
		 if ( u == knots[index + order] ) coef2 = 1;
		 else coef2 = 0;
	  }
	  else coef2 = (knots[index + order] - u)/(knots[index + order] - knots[index+1]);
		
      return ( coef1 * Bspline(index, order-1, u) + coef2 * Bspline(index+1,order-1 ,u) );
   }
}

// Draw a B-spline function graph as line strip and joints as points.
void drawSpline(int index, int order)
{
   float x;   
   int j;

   // Drawing are scaled by factor of 3 in the y-direction.
   // Special case to handle order 1 to avoid vertical edges.
   if (order == 1)
   {
	  // Spline curve.
      glBegin(GL_LINE_STRIP);
	  for ( x = knots[index]; x < knots[index+1]; x+=0.05 )
         glVertex3f( -40.0 + x, 10.0, 0.0 );	
	  glEnd();
	  glPointSize(3.0);

	  // Joints.
	  glBegin(GL_POINTS);
	  glVertex3f( -40.0 + knots[index], 10.0, 0.0);
	  glVertex3f( -40.0 + knots[index+1], 10.0, 0.0);
	  glEnd();
   }
   else
   {
	  // Spline curve.
	  glBegin(GL_LINE_STRIP);
	  for ( x = knots[index]; x <= knots[index + order]; x += 0.005 )
	     glVertex3f( -40.0 + x, 30*Bspline(index, order, x) - 20.0, 0.0 );	
      glEnd();

	  // Joints.
	  glColor3f(0.0, 0.0, 0.0);
	  glBegin(GL_POINTS);
	  for (j = index; j <= index + order; j++)
	     glVertex3f( -40.0 + knots[j], 30*Bspline(index, order, knots[j]) - 20.0, 0.0 );
	  glEnd();
   }
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();

   glTranslatef(0.0, 0.5, 0.0);

   // Write spline order.
   glColor3f(0.0, 0.0, 0.0);
   switch (splineOrder) 
   {
      case 1:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "First-order B-splines");
	  break;
      case 2:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "Linear B-splines");
	  break;
      case 3:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "Quadratic B-splines");
	  break;
      case 4:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "Cubic B-splines");
	  break;
      default:
      break;
   }

   // Draw successive B-spline functions for the chosen order.
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
   for (i = 0; i < 9 - splineOrder; i++ )
   {
	  switch (i) 
	  {     
         case 0:
	       glColor3f(1.0, 0.0, 0.0);
	     break;
         case 1:
	       glColor3f(0.0, 1.0, 0.0);
	     break;
         case 2:
	       glColor3f(0.0, 0.0, 1.0);
	     break;
         case 3:
	       glColor3f(1.0, 0.0, 1.0);
	     break;
         case 4:
	       glColor3f(0.0, 1.0, 1.0);
	     break;
         case 5:
	       glColor3f(1.0, 1.0, 0.0);
	     break;
         case 6:
	       glColor3f(0.0, 0.0, 0.0);
	     break;
         case 7:
	       glColor3f(1.0, 0.0, 0.0);
	     break;
         default:
         break;
	  }
       drawSpline(i, splineOrder);
   }

   // Draw the x-axis.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f(-40.0, -20.0, 0.0);
      glVertex3f( 40.0, -20.0, 0.0);  
   glEnd();

   // Draw points on the x-axis.
   glPointSize(5.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 9; i++) 
         glVertex3f(-40.0 + (float)i*10.0, -20.0, 0.0);
   glEnd();

   // Label the points on the x-axis.
   glRasterPos3f(-40.5, -23.0, 0.0);
   writeBitmapString((void*)font, "0");
   glRasterPos3f(-30.5, -23.0, 0.0);
   writeBitmapString((void*)font, "1");
   glRasterPos3f(-20.5, -23.0, 0.0);
   writeBitmapString((void*)font, "2");
   glRasterPos3f(-10.5, -23.0, 0.0);
   writeBitmapString((void*)font, "3");
   glRasterPos3f(-0.5, -23.0, 0.0);
   writeBitmapString((void*)font, "4");
   glRasterPos3f(9.5, -23.0, 0.0);
   writeBitmapString((void*)font, "5");
   glRasterPos3f(19.5, -23.0, 0.0);
   writeBitmapString((void*)font, "6");
   glRasterPos3f(29.5, -23.0, 0.0);
   writeBitmapString((void*)font, "7");
   glRasterPos3f(39.5, -23.0, 0.0);
   writeBitmapString((void*)font, "8");

   // Draw the y-axis.
   glBegin(GL_LINES);
      glVertex3f(-40.0, -20.0, 0.0);
      glVertex3f(-40.0, 40.0, 0.0);  
   glEnd();

   // Draw points on the y-axis.
   glBegin(GL_POINTS);
         glVertex3f(-40.0, 10.0, 0.0);
         glVertex3f(-40.0, 40.0, 0.0);
   glEnd();

   // Label the points on the y-axis.
   glRasterPos3f(-42.5, -20.5, 0.0);
   writeBitmapString((void*)font, "0");
   glRasterPos3f(-42.5, 9.5, 0.0);
   writeBitmapString((void*)font, "1");
   glRasterPos3f(-42.5, 39.5, 0.0);
   writeBitmapString((void*)font, "2");

   // Draw horizontal bars corresponding to knot points.
   glBegin(GL_LINES);
      for (i = 0; i < 9; i++) 
	  {
         glVertex3f(-40.0, -35.0 + (float)i, 0.0);
         glVertex3f( 40.0, -35.0 + (float)i, 0.0);
	  }
   glEnd();

   // Draw the knot points as dots on their respective bars.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 9; i++) 
         glVertex3f( -40.0 + knots[i], -35.0 + (float)i, 0.0);
   glEnd();

   // Highlight the selected knot point.
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3f( -40.0 + knots[selectedKnot], 
		          -35.0 + (float)selectedKnot, 0.0); 
   glEnd();

   // Label the knot bars.
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(-7.0, -40.0, 0.0);
   writeBitmapString((void*)font, "Knot Values");
   
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
      case ' ':
         if (selectedKnot < 8) selectedKnot++; 
		 else selectedKnot = 0;
         glutPostRedisplay();
		 break;
      case 127:	
         resetKnots();
		 glutPostRedisplay();
         break;
      default:
         break;
  }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_LEFT) decreaseKnot(selectedKnot);
   if(key == GLUT_KEY_RIGHT) increaseKnot(selectedKnot);
   if(key == GLUT_KEY_UP) 
   {
	   if (splineOrder < 4) splineOrder++; else splineOrder = 1;
   }
   if(key == GLUT_KEY_DOWN) 
   {
	   if (splineOrder > 1) splineOrder--; else splineOrder = 4;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up/down arrow keys to cycle between order 1 through 4." << endl
        << "Press space to select a knot points." << endl
        << "Press the left/right arrow keys to move the selected knot point." << endl
        << "Press delete to reset." << endl;
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
   glutCreateWindow("experimentBsplinesLinear.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}
