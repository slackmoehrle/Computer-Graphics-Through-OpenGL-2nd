////////////////////////////////////////////////////////////////////////////////         
// bicubicSplineSurface.cpp
// 
// This program draws the bicubic B-spline approximation of a 15x10 array of 
// movable control points over a fixed standard knot vector in either direction.
//
// Interaction:
// Press space, backspace, tab and enter keys to select a control point.
// Press the right/left arrow keys to move the control point up/down the x-axis.
// Press the up/down arrow keys to move the control point up/down the y-axis.
// Press the page up/down keys to move the control point up/down the z-axis.
// Press the x, X, y, Y, z, Z keys to rotate the viewpoint.
// Press delete to reset control points.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////// 

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
static float controlPoints[15][10][3]; // Control points.
static float Xangle = 30.0, Yangle = 10.0, Zangle = 40.0; // Angles to rotate surface.
static int rowCount = 0, columnCount = 0; // Indexes of selected control point.
static GLUnurbsObj *nurbsObject; // Pointer to NURBS object.

// Standard knot vector along the u-parameter.
static float uknots[19] = 
{0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 
7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 12.0, 12.0, 12.0};

// Standard knot vector along the v-parameter.
static float vknots[14] = 
{0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 
7.0, 7.0, 7.0, 7.0};
// End globals.

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Reset control points.
void resetControlPoints(void)
{
   int i, j;

   for (i = 0; i < 15; i++)  
      for (j = 0; j < 10; j++)  
	  {
         controlPoints[i][j][0] = -2.5 + j*0.5;
         controlPoints[i][j][1] = 0.0;
	     controlPoints[i][j][2] = 6.0 - i;
	  }
   rowCount = columnCount = 0;
}

// Initialization routine.	
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Create NURBS object.
   nurbsObject = gluNewNurbsRenderer();
   gluNurbsProperty(nurbsObject, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
   gluNurbsProperty(nurbsObject, GLU_SAMPLING_TOLERANCE, 100.0);
   gluNurbsProperty(nurbsObject, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);

   resetControlPoints();
}

// Drawing routine.
void drawScene(void)
{
   int i, j;
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   gluLookAt(0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate scene.
   glRotatef (Zangle, 0.0, 0.0, 1.0);
   glRotatef (Yangle, 0.0, 1.0, 0.0);
   glRotatef (Xangle, 1.0, 0.0, 0.0);

   // Draw the spline surface.
   glColor3f(0.0, 0.0, 0.0);
   gluBeginSurface(nurbsObject);
   gluNurbsSurface(nurbsObject, 19, uknots, 14, vknots, 
	               30, 3, controlPoints[0][0], 4, 4, GL_MAP2_VERTEX_3);
   gluEndSurface(nurbsObject);

   glPointSize(5.0);

   // Draw green control points.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 15; i++)
         for (j = 0; j < 10; j++)
            glVertex3fv(controlPoints[i][j]);
   glEnd();

   // Draw red selected control point.
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3fv(controlPoints[rowCount][columnCount]);
   glEnd();

   // Draw the co-ordinate axes.
   glColor3f(0.0, 0.0, 1.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
	   glVertex3f(-6.0, 0.0, 0.0);
       glVertex3f(6.0, 0.0, 0.0);
       glVertex3f(5.75, 0.0, 0.25);
       glVertex3f(6.0, 0.0, 0.0);
       glVertex3f(5.75, 0.0, -0.25);
       glVertex3f(6.0, 0.0, 0.0);

	   glVertex3f(0.0, -6.0, 0.0);
       glVertex3f(0.0, 6.0, 0.0);
       glVertex3f(0.25, 5.75, 0.0);
       glVertex3f(0.0, 6.0, 0.0);
       glVertex3f(-0.25, 5.75, 0.0);
       glVertex3f(0.0, 6.0, 0.0);

	   glVertex3f(0.0, 0.0, -9.0);
       glVertex3f(0.0, 0.0, 6.5);
	   glVertex3f(0.25, 0.0, 6.25);
	   glVertex3f(0.0, 0.0, 6.5);
	   glVertex3f(-0.25, 0.0, 6.25);
	   glVertex3f(0.0, 0.0, 6.5);
   glEnd();
   glLineWidth(1.0);

   // Label the co-ordinate axes.
   glPushMatrix();
   glTranslatef(6.1, 0.0, 0.0);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "X");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 6.1, 0.0);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "Y");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 0.0, 6.6);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "Z");
   glPopMatrix();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
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
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         break;
      case 9:
		 {
		    if (rowCount < 14) rowCount++;
		    else rowCount = 0;
		 }
		 break;
      case 13:
		 if (rowCount > 0) rowCount--;
		 else rowCount = 14;
         break;
      case ' ':
		 {
		    if (columnCount < 9) columnCount++;
		    else columnCount = 0;
		 }
         break;
      case 8:
		 if (columnCount > 0) columnCount--;
		 else columnCount = 9;
         break;
      case 127:	
         resetControlPoints();
         break;
      default:
         break;
   }
   glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT) controlPoints[rowCount][columnCount][0] -= 0.1;
   if (key == GLUT_KEY_RIGHT) controlPoints[rowCount][columnCount][0] += 0.1;
   if (key == GLUT_KEY_DOWN) controlPoints[rowCount][columnCount][1] -= 0.1;
   if (key == GLUT_KEY_UP) controlPoints[rowCount][columnCount][1] += 0.1;
   if (key == GLUT_KEY_PAGE_DOWN) controlPoints[rowCount][columnCount][2] += 0.1;
   if (key == GLUT_KEY_PAGE_UP) controlPoints[rowCount][columnCount][2] -= 0.1;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space, backspace, tab and enter keys to select a control point." << endl
        << "Press the right/left arrow keys to move the control point up/down the x-axis." << endl
        << "Press the up/down arrow keys to move the control point up/down the y-axis." << endl
        << "Press the page up/down keys to move the control point up/down the z-axis." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the viewpoint." << endl
        << "Press delete to reset control points." << endl;
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
   glutCreateWindow("bicubicSplineSurface.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
  
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
  
   glutMainLoop(); 
}
