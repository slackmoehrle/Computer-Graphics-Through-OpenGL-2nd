////////////////////////////////////////////////////////////////////////////////////////          
// antiAliasing+multisampling.cpp
//
// This program shows the effect of antialiasing on a straight line segment and a point,
// as well as the effect of multisampling on the line segment, point and two triangles.
// Antialiasing and multisampling can be turned on and off independently. 
// Point size and line width can be changed independently.
// The scene can be rotated as well as translated in all directions.
//
// Interaction: 
// Press a/A to toggle between antialiasing on and off.
// Press m/M to toggle between multisampling on and off.
// Press p/P to decrease/increase the point size. 
// Press l/L to decrease/increase the line width. 
// Press x, X, y, Y, z, Z to turn the scene.
// Press the arrow and page up/down keys to translate the scene. 
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <stdio.h>
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
static float size = 5.0; // Point size.
static float width = 1.0; // Line width.
static float xDist = 0.0, yDist = 0.0, zDist = -15.0; // Distance scene translated.
static int isAntialiased = 0; // Is antialiasing on?
static int isMultisampled = 0; // Is multisampling on?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static int sampleBuffers[1]; // Number of sample buffers.
static char buffer[10]; // Character string buffer.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert integer to char string.
void intToString(char * destStr, int val) 
{
	sprintf(destStr,"%d",val);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor4f(0.0, 0.0, 0.0, 1.0);
   glLoadIdentity(); 

   glRasterPos3f(-4.5, 4.5, -5.1); 
   // Antialiasing control.
  if (isAntialiased) 
   {
	  glEnable(GL_BLEND); // Enable blending.
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.

      glEnable(GL_LINE_SMOOTH); // Enable line antialiasing.
	  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Ask for best line antialiasing.
      glEnable(GL_POINT_SMOOTH);  // Enable point antialiasing.
	  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Ask for best point antialiasing.
	  writeBitmapString((void*)font, "Antialiasing on!");
   }
   else 
   {
	  glDisable(GL_BLEND); 
      glDisable(GL_LINE_SMOOTH); 
      glDisable(GL_POINT_SMOOTH); 
	  writeBitmapString((void*)font, "Antialiasing off!");
   }

   glRasterPos3f(-4.5, 4.2, -5.1); 
   // Multisampling control.
   if (isMultisampled) 
   {
      glEnable(GL_MULTISAMPLE); // Enable multisampling.
      writeBitmapString((void*)font, "Multisampling on! ");

	  // Get and output the number of sample buffers (should be > 0).
      glGetIntegerv(GL_SAMPLE_BUFFERS, sampleBuffers);
	  intToString(buffer, sampleBuffers[0]);
	  writeBitmapString((void*)font, "Number of sample buffers: ");
	  writeBitmapString((void*)font, buffer);
   }
   else
   {
      glDisable(GL_MULTISAMPLE);
      writeBitmapString((void*)font, "Multisampling off!");
   }

   // Commands to move the scene.
   glTranslatef(xDist, yDist, zDist);
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Draw a wire cube reference frame.
   glLineWidth(1.0);
   glutWireCube(7.0);

   // Draw a point.
   glColor4f(0.0, 1.0, 0.0, 0.8);
   glPointSize(size);
   glBegin(GL_POINTS);
      glVertex3f(0.0, 0.0, 0.0);
   glEnd();

   // Draw a line segment.
   glColor4f(1.0, 0.0, 0.0, 0.8);
   glLineWidth(width);
   glBegin(GL_LINES);
      glVertex3f(-10.0, 0.0, 0.0);
      glVertex3f(10.0, 0.0, 0.0);
   glEnd();
  
   // Draw two adjacent triangles.
   glBegin(GL_TRIANGLES);
      glColor4f(0.0, 0.0, 1.0, 0.8);
      glVertex3f(6.0, 1.0, 0.0);
      glVertex3f(6.0, 8.0, 0.0);
      glVertex3f(12.0, 1.0, 0.0);

      glColor4f(1.0, 1.0, 0.0, 0.8);
      glVertex3f(12.0, 8.0, 0.0);
      glVertex3f(12.0, 1.0, 0.0);
      glVertex3f(6.0, 8.0, 0.0);
   glEnd();

   glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
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
      case 'a':
		 ++isAntialiased %= 2;
         glutPostRedisplay();
		 break;
      case 'A':
		 ++isAntialiased %= 2;
         glutPostRedisplay();
		 break;
      case 'm':
 		 ++isMultisampled %= 2;
         glutPostRedisplay();
		 break;
      case 'M':
 		 ++isMultisampled %= 2;
         glutPostRedisplay();
		 break;
      case 'l':
         width--;
         glutPostRedisplay();
         break;
      case 'L':
		 width++;
         glutPostRedisplay();
         break;
      case 'p':
         size--;
         glutPostRedisplay();
         break;
      case 'P':
         size++;
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
   if (key == GLUT_KEY_UP) yDist++;
   if (key == GLUT_KEY_DOWN) yDist--;

   if (key == GLUT_KEY_RIGHT) xDist++;
   if (key == GLUT_KEY_LEFT) xDist--;

   if (key == GLUT_KEY_PAGE_DOWN) zDist++;
   if (key == GLUT_KEY_PAGE_UP) zDist--;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press a/A to toggle between antialiasing on and off." << endl
	    << "Press m/M to toggle between multisampling on and off." << endl
	    << "Press p/P to decrease/increase the point size." << endl 
        << "Press l/L to decrease/increase the line width." << endl
		<< "Press x, X, y, Y, z, Z to turn the scene." << endl
		<< "Press the arrow and page up/down keys to translate the scene." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
  
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("antiAliasing+multisampling.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop(); 
}

