//////////////////////////////////////////////////////////////////////////////////////////////      
// lightAndMaterial2.cpp
//
// This program draws a blue sphere lit by two positional lights, one white and one green,
// the location of each shown by a smaller wire sphere. The lights can be individually turned 
// off and on. The properties of the white light can be controlled and it can also be
// rotated. The global ambient light can be controlled. The viewpoint can be set to be local
// or infinite.
//
// Interaction:
// Press 'w' (or 'W') to toggle the white light off/on. 
// Press 'g' (or 'G') to toggle the green light off/on. 
// Press 'd/D' to decrease/increase the white light's diffuse and specular intensity.
// Press 'm/M' to decrease/increase global ambient white light intensity.
// Press 'l' (or 'L') to toggle between infinite and local viewpoint.
// Press 'p' (or 'P') to toggle between positional and directional white light.
// Press arrow keys to rotate the white light.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////////////// 

#include <iostream>
#include <fstream>

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
static int light0On = 1; // White light on?
static int light1On = 1; // Green light on?
static float d = 1.0; // Diffuse and specular white light intensity.
static float m = 0.2; // Global ambient white light intensity.
static int localViewer = 1; // Local viewpoint?
static float p = 1.0; // Positional light?
static float xAngle = 0.0, yAngle = 0.0; // Rotation angles of white light.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static char theStringBuffer[10]; // String buffer.

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

// Write data.
void writeData(void)
{ 
   glDisable(GL_LIGHTING); // Disable lighting.
   glColor3f(1.0, 1.0, 1.0); 
   
   floatToString(theStringBuffer, 4, d);
   glRasterPos3f(-1.0, 1.05, -2.0);
   writeBitmapString((void*)font, "Diffuse and specular white light intensity: ");
   writeBitmapString((void*)font, theStringBuffer);
   
   floatToString(theStringBuffer, 4, m);
   glRasterPos3f(-1.0, 1.0, -2.0);
   writeBitmapString((void*)font, "Global ambient whitle light intensity: ");  
   writeBitmapString((void*)font, theStringBuffer);

   glRasterPos3f(-1.0, 0.95, -2.0);
   if (localViewer) writeBitmapString((void*)font, "Local viewpoint.");  
   else writeBitmapString((void*)font, "Infinite viewpoint.");

   glEnable(GL_LIGHTING); // Re-enable lighting.
}

// Initialization routine.
void setup(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

   // Material property vectors.
   float matAmbAndDif[] = {0.0, 0.0, 1.0, 1.0};
   float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties of sphere.
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// Drawing routine.
void drawScene()
{  
   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec0[] = { d, d, d, 1.0 };
   float lightPos0[] = { 0.0, 0.0, 3.0, p };
   float lightDifAndSpec1[] = { 0.0, 1.0, 0.0, 1.0 };
   float lightPos1[] = { 1.0, 2.0, 0.0, 1.0 };
   float globAmb[] = { m, m, m, 1.0 };
   
   // Light0 properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

   // Light1 properties.
   glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
   glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);
  
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer); // Enable local viewpoint

   // Turn lights off/on.
   if (light0On) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
   if (light1On) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   writeData();

   gluLookAt(0.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Draw light source spheres (or arrow) after disabling lighting.
   glDisable(GL_LIGHTING);

   // Light0 positioned and sphere positioned in case of positional light
   // and arrow in case of directional light.
   glPushMatrix();
   glRotatef(xAngle, 1.0, 0.0, 0.0); // Rotation about x-axis.
   glRotatef(yAngle, 0.0, 1.0, 0.0); // Rotation about z-axis.
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
   glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
   glColor3f(d, d, d); 
   if (light0On) 
   {
	   if (p) glutWireSphere(0.05, 8, 8); // Sphere at positional light source.
	   else // Arrow pointing along incoming directional light.
	   {
          glLineWidth(3.0);
		  glBegin(GL_LINES);
		     glVertex3f(0.0, 0.0, 0.25);
             glVertex3f(0.0, 0.0, -0.25);
			 glVertex3f(0.0, 0.0, -0.25);
			 glVertex3f(0.05, 0.0, -0.2);
			 glVertex3f(0.0, 0.0, -0.25);
			 glVertex3f(-0.05, 0.0, -0.2);
          glEnd();
		  glLineWidth(1.0);
	   }
   }
   glPopMatrix();

   // Light1 and its sphere positioned.
   glPushMatrix();
   glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
   glTranslatef(lightPos1[0], lightPos1[1], lightPos1[2]);
   glColor3f(0.0, 1.0, 0.0); 
   if (light1On) glutWireSphere(0.05, 8, 8);
   glPopMatrix();

   glEnable(GL_LIGHTING);

   // Sphere.
   glutSolidSphere(1.5, 200, 200);

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
	  case 'w':
		 if (light0On) light0On = 0; else light0On = 1;
		 glutPostRedisplay();
		 break;
	  case 'W':
		 if (light0On) light0On = 0; else light0On = 1;
		 glutPostRedisplay();
		 break;
	  case 'g':
		 if (light1On) light1On = 0; else light1On = 1;
		 glutPostRedisplay();
		 break;
	  case 'G':
		 if (light1On) light1On = 0; else light1On = 1;
		 glutPostRedisplay();
		 break;
	  case 'l':
		 if (localViewer) localViewer = 0; else localViewer = 1;
		 glutPostRedisplay();
		 break;
	  case 'L':
		 if (localViewer) localViewer = 0; else localViewer = 1;
		 glutPostRedisplay();
		 break;
	  case 'p':
		 if (p) p = 0.0; else p = 1.0;
		 glutPostRedisplay();
		 break;
	  case 'P':
		 if (p) p = 0.0; else p = 1.0;
		 glutPostRedisplay();
		 break;
	  case 'd':
 		 if (d > 0.0) d -= 0.05;
         glutPostRedisplay();
		 break;
	  case 'D':
 		 if (d < 1.0) d += 0.05;
         glutPostRedisplay();
		 break;
	  case 'm':
 		 if (m > 0.0) m -= 0.05;
         glutPostRedisplay();
		 break;
	  case 'M':
 		 if (m < 1.0) m += 0.05;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_DOWN)
   {
      xAngle++;
      if (xAngle > 360.0) xAngle -= 360.0;
   }
   if(key == GLUT_KEY_UP)
   {
      xAngle--;
      if (xAngle < 0.0) xAngle += 360.0;
   }
   if(key == GLUT_KEY_RIGHT)
   {
      yAngle++;
      if (yAngle > 360.0) yAngle -= 360.0;
   }
   if(key == GLUT_KEY_LEFT)
   {
      yAngle--;
      if (yAngle < 0.0) yAngle += 360.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press 'w' (or 'W') to toggle the white light off/on." << endl
        << "Press 'g' (or 'G') to toggle the green light off/on." << endl
        << "Press 'd/D' to decrease/increase the white light's diffuse and specular intensity." << endl
        << "Press 'm/M' to decrease/increase global ambient white light intensity." << endl
        << "Press 'l' (or 'L') to toggle between infinite and local viewpoint." << endl
        << "Press 'p' (or 'P') to toggle between positional and directional white light." << endl
        << "Press arrow keys to rotate the white light." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("lightAndMaterial2.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
    
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
   
   glutMainLoop();
}
