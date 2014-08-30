/////////////////////////////////////////////////////////////////////////////////////////////
// quaternionAnimation.cpp
//
// This program draws a blue L whose orientation can be changed by the user. Animation is 
// shown between the start orientation (fixed red L) and target orientation (current blue L). 
// The animation is done by the spherical linear interpolation of the quaternions 
// corresponding to the start and target orientations.
//
// Interaction:
// Press the x, X, y, Y, z, Z keys to rotate the blue L.
// Press enter to begin animation.
// Press delete to reset.
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
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

#define PI 3.14159265

using namespace std;

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Euler angles.
static float matrixData[16]; // Rotation matrix values.
static float t = 0.0; // Interpolation parameter.
static long font = (long)GLUT_BITMAP_8_BY_13; // Text font.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

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

// Write data.
void writeData(void)
{
   char buffer [33];
   
   intToString(buffer, Xangle);
   glRasterPos3f(-1.0, 0.9, -2.0);
   writeBitmapString((void*)font, "alpha = ");
   writeBitmapString((void*)font, buffer);

   intToString(buffer, Yangle);
   glRasterPos3f(-1.0, 0.8, -2.0);
   writeBitmapString((void*)font, "beta = ");
   writeBitmapString((void*)font, buffer);

   intToString(buffer, Zangle);
   glRasterPos3f(-1.0, 0.7, -2.0);
   writeBitmapString((void*)font, "gamma = ");
   writeBitmapString((void*)font, buffer);
}

// Quaternion class.
class Quaternion
{
public:
   Quaternion() { }
   Quaternion(float wVal, float xVal, float yVal, float zVal)
   {
      w = wVal; x= xVal; y = yVal; z = zVal;
   }
   float getW() { return w; }
   float getX() { return x; }
   float getY() { return y; }
   float getZ() { return z; }

private:
   float w, x, y, z;
};

static Quaternion identityQuaternion(1.0, 0.0, 0.0, 0.0), q; // Global identity quaternion.

// Euler angles class.
class EulerAngles
{
public:
   EulerAngles() { }
   EulerAngles(float alphaVal, float betaVal, float gammaVal)
   {
      alpha = alphaVal; beta = betaVal; gamma = gammaVal;
   }
   float getAlpha() { return alpha; }
   float getBeta()  { return beta; }
   float getGamma() { return gamma; }

private:
   float alpha, beta, gamma;
};

static EulerAngles e; // Global Eular angle value.

// Rotation matrix class.
class RotationMatrix
{
public:
   RotationMatrix() { }
   RotationMatrix( float matrixDataVal[16] )
   {
      for (int i=0; i < 16; i++) matrixData[i] = matrixDataVal[i];
   }
   float getMatrixData(int i) { return matrixData[i]; }

private:
   float matrixData[16];
};

// Routine to multiply two quaternions.
Quaternion multiplyQuaternions(Quaternion q1, Quaternion q2)
{
   float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;

   w1 = q1.getW(); x1 = q1.getX(); y1 = q1.getY(); z1 = q1.getZ(); 
   w2 = q2.getW(); x2 = q2.getX(); y2 = q2.getY(); z2 = q2.getZ(); 

   w3 = w1*w2 - x1*x2 - y1*y2 - z1*z2;
   x3 = w1*x2 + x1*w2 + y1*z2 - z1*y2;
   y3 = w1*y2 + y1*w2 + z1*x2 - x1*z2;
   z3 = w1*z2 + z1*w2 + x1*y2 - y1*x2 ;

   return *new Quaternion(w3, x3, y3, z3);
}

// Routine to convert the Euler angle specifying a rotation to a quaternion. 
Quaternion eulerAnglesToQuaternion(EulerAngles e)
{
   float alpha, beta, gamma;
   Quaternion *q1, *q2, *q3;

   alpha = e.getAlpha(); beta = e.getBeta(); gamma = e.getGamma(); 

   q1 = new Quaternion( cos( (PI/180.0) * (alpha/2.0) ), sin( (PI/180.0) * (alpha/2.0) ), 0.0, 0.0 );
   q2 = new Quaternion( cos( (PI/180.0) * (beta/2.0) ), 0.0, sin( (PI/180.0) * (beta/2.0) ), 0.0 );
   q3 = new Quaternion( cos( (PI/180.0) * (gamma/2.0) ), 0.0, 0.0, sin( (PI/180.0) * (gamma/2.0) ) );

   return multiplyQuaternions( *q1, multiplyQuaternions(*q2, *q3) );
}

// Routine to convert a quaternion specifying a rotation to a 4x4 rotation matrix in column-major order.
RotationMatrix quaternionToRotationMatrix(Quaternion q)
{
   float w, x, y, z;
   float m[16];

   w = q.getW(); x = q.getX(); y = q.getY(); z = q.getZ(); 
   
   m[0] = w*w + x*x - y*y - z*z;
   m[1] = 2.0*x*y + 2.0*w*z;
   m[2] = 2.0*x*z - 2.0*y*w;
   m[3] = 0.0;
   m[4] = 2.0*x*y - 2.0*w*z;
   m[5] = w*w - x*x + y*y - z*z;
   m[6] = 2.0*y*z + 2.0*w*x;
   m[7] = 0.0;
   m[8] = 2.0*x*z + 2.0*w*y;
   m[9] = 2.0*y*z - 2.0*w*x;
   m[10] = w*w - x*x - y*y + z*z;
   m[11] = 0.0;
   m[12] = 0.0;
   m[13] = 0.0;
   m[14] = 0.0;
   m[15] = 1.0;

   return *new RotationMatrix(m);
}

// Read global Euler angle values to global EulerAngles object e.
void readEulerAngles(EulerAngles *e)
{
   *e = *new EulerAngles(Xangle, Yangle, Zangle);
}
 
// Write RotationMatrix object r values to global matrixData.
void writeMatrixData(RotationMatrix r)
{
   for (int i=0; i < 16; i++) matrixData[i] = r.getMatrixData(i);
}

// Spherical linear interpolation between unit quaternions q1 and q2 with interpolation parameter t.
Quaternion slerp(Quaternion q1, Quaternion q2, float t)
{
   float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;
   Quaternion q2New;
   float theta, mult1, mult2;

   w1 = q1.getW(); x1 = q1.getX(); y1 = q1.getY(); z1 = q1.getZ(); 
   w2 = q2.getW(); x2 = q2.getX(); y2 = q2.getY(); z2 = q2.getZ();
   
   // Reverse the sign of q2 if q1.q2 < 0.
   if (w1*w2 + x1*x2 + y1*y2 + z1*z2 < 0)  
   {
      w2 = -w2; x2 = -x2; y2 = -y2; z2 = -z2;
   }
	   
   theta = acos(w1*w2 + x1*x2 + y1*y2 + z1*z2);

   if (theta > 0.000001) 
   {
	  mult1 = sin( (1-t)*theta ) / sin( theta );
	  mult2 = sin( t*theta ) / sin( theta );
   }

   // To avoid division by 0 and by very small numbers the approximation of sin(angle)
   // by angle is used when theta is small (0.000001 is chosen arbitrarily).
   else
   {
      mult1 = 1 - t;
	  mult2 = t;
   }

   w3 =  mult1*w1 + mult2*w2;
   x3 =  mult1*x1 + mult2*x2;
   y3 =  mult1*y1 + mult2*y2;
   z3 =  mult1*z1 + mult2*z2;
   
   return *new Quaternion(w3, x3, y3, z3);
}

// Initialization routine.
void setup(void) 
{
   int i;

   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Initialize global matrixData.
   for (i=0; i < 16; i++) matrixData[i] = 0.0;
   matrixData[0] = matrixData[5] = matrixData[10] = matrixData[15] = 1.0;
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   glColor3f(0.0, 0.0, 0.0);
   writeData();

   gluLookAt(0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glRotatef(0.0, 0.0, 0.0, 1.0);
   glRotatef(30.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 1.0, 0.0, 0.0);

   // Draw the co-ordinate axes.
   glLineWidth(2.0);
   glColor3f(0.0, 0.0, 0.0);
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

	   glVertex3f(0.0, 0.0, -6.0);
       glVertex3f(0.0, 0.0, 6.0);
	   glVertex3f(0.25, 0.0, 5.75);
	   glVertex3f(0.0, 0.0, 6.0);
	   glVertex3f(-0.25, 0.0, 5.75);
	   glVertex3f(0.0, 0.0, 6.0);
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
   glTranslatef(0.0, 0.0, 6.1);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "Z");
   glPopMatrix();

   glPushMatrix();

   // Apply rotation matrix from value in global matrixData.
   glMultMatrixf(matrixData);

   // Draw animated red L.
   glColor3f(1.0, 0.0, 0.0);
   glLineWidth(3.0);
   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, 0.0);
	  glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.5, 2.0);
   glEnd();
   glLineWidth(1.0);

   glPopMatrix();

   glPushMatrix();

   // Change Euler angles.
   glRotatef(Xangle, 1.0, 0.0, 0.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Zangle, 0.0, 0.0, 1.0);

   // Draw user-orientable blue L.
   glColor3f(0.0, 0.0, 1.0);
   glLineWidth(3.0);
   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, 0.0);
	  glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.0, 2.0);
      glVertex3f(0.0, 0.5, 2.0);
   glEnd();
   glLineWidth(1.0);

   glPopMatrix();

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   Quaternion qInterpolated;
   RotationMatrix r;

   if (isAnimate) 
   {
      if (t < 1.0) t += 0.04;
      qInterpolated = slerp(identityQuaternion, q, t);
      r = quaternionToRotationMatrix(qInterpolated);
      writeMatrixData(r);

      glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
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
   int i;
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 180.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle <= -180.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 180.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle <= -180.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 180.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle <= -180.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
	  case 13: 
         readEulerAngles(&e);
         q = eulerAnglesToQuaternion(e);
         isAnimate = 1;
		 animate(1);
         glutPostRedisplay();
		 break;
      case 127:
         Zangle = Yangle = Xangle = 0.0;
		 isAnimate = 0;
		 t = 0.0;
         for (i=0; i < 16; i++) matrixData[i] = 0.0;
         matrixData[0] = matrixData[5] = matrixData[10] = matrixData[15] = 1.0;
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
   cout << "Press the x, X, y, Y, z, Z keys to rotate the blue L." << endl
        << "Press enter to begin animation." << endl
        << "Press delete to reset." << endl
        << "Press the up/down arrow keys to speed up/slow down animation." << endl;
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
   glutCreateWindow("quaternionAnimation.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}
