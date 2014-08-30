/////////////////////////////////////////////////////////////////////////////////////////////        
// shipMovie.cpp
//
// A ship made of Bezier surfaces and GLU quadric objects travels over a blue sea foreground
// with other ships in the background. A torpedo targets the ship. There is simple animation
// of smoke from the ship's chimney.
//
// Interaction:
// Press the x, X, y, Y, z, Z keys to rotate the viewpoint.
// Press space to toggle animation on/off.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
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
#define N 75 // Number of discs in smoke stack.

using namespace std;

// Begin globals.
// Control points for Bezier surface of ship's hull.
static float controlPointsHull[6][6][3] = 
{
	{{0.0, 2.5, 5.0}, {0.0, 1.0, 5.0}, {0.0, 0.0, 5.0}, {0.0, 0.0, 5.0}, {0.0, 1.0, 5.0}, {0.0, 2.5, 5.0}},
	{{-1.25, 2.5, 3.0}, {-0.75, 1.0, 3.0}, {-0.5, 0.0, 3.0}, {0.5, 0.0, 3.0}, {0.75, 1.0, 3.0}, {1.25, 2.5, 3.0}},
	{{-2.0, 2.5, 1.0}, {-1.5, 1.0, 1.0}, {-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.5, 1.0, 1.0}, {2.0, 2.5, 1.0}},
	{{-1.0, 2.5, -1.0}, {-2.0, 1.0, -1.0}, {-1.0, 0.0, -1.0}, {1.0, 0.0, -1.0}, {2.0, 1.0, -1.0}, {1.0, 2.5, -1.0}},
	{{-1.0, 2.5, -3.0}, {-0.75, 1.0, -3.0}, {-0.5, 0.0, -3.0}, {0.5, 0.0, -3.0}, {0.75, 1.0, -3.0}, {1.0, 2.5, -3.0}},
	{{0.0, 2.5, -9.0}, {0.0, 1.0, -9.0}, {0.0, 0.0, -9.0}, {0.0, 0.0, -9.0}, {0.0, 1.0, -9.0}, {0.0, 2.5, -9.0}}
};

// Control points for Bezier surface of ship's deck.
static float controlPointsDeck[6][2][3] = 
{
	{{0.0, 2.5, 5.0}, {0.0, 2.5, 5.0}},
	{{-1.25, 2.5, 3.0}, {1.25, 2.5, 3.0}},
	{{-2.0, 2.5, 1.0}, {2.0, 2.5, 1.0}},
	{{-1.0, 2.5, -1.0}, {1.0, 2.5, -1.0}},
	{{-1.0, 2.5, -3.0}, {1.0, 2.5, -3.0}},
	{{0.0, 2.5, -9.0}, {0.0, 2.5, -9.0}}
};

// Control points for Bezier surface of a propeller blade.
static float controlPointsPropellerBlade[4][4][3] = 
{
	{{-0.2, 0.2, -0.05}, {-0.1, 0.0, -0.02}, {0.1, 0.0, 0.03}, {0.2,  0.2, 0.05}},
	{{-0.2, 0.4, -0.1}, {-0.1, 0.4, -0.05},  {0.1, 0.4, 0.05}, {0.2,  0.4, 0.1}},
	{{-0.2, 0.6, -0.2}, {-0.1, 0.6, -0.1},  {0.1, 0.6, 0.1}, {0.2, 0.6, 0.2}},
	{{-0.2, 0.8, -0.2}, {-0.1, 1.0, -0.1},  {0.1, 1.0, 0.1},  {0.2, 0.8, 0.2}}
};

static GLUquadricObj *qobj; // Create a pointer to a new quadric object.
static unsigned int base; // Base index for display lists.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 50; // Time interval between frames.
static int t = 0; // Time parameter.
static float angle = 0; // Angle of torpedo propeller turn.
// End globals.

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
         glNormal3f( -cos( (float)(j+1)/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     -sin( (float)(j+1)/latSlices * PI/2.0 ),
					 -cos( (float)(j+1)/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );
         glVertex3f( radius * cos( (float)(j+1)/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     radius * sin( (float)(j+1)/latSlices * PI/2.0 ),
					 radius * cos( (float)(j+1)/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );
         glNormal3f( -cos( (float)j/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     -sin( (float)j/latSlices * PI/2.0 ),
					 -cos( (float)j/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );
         glVertex3f( radius * cos( (float)j/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     radius * sin( (float)j/latSlices * PI/2.0 ),
					 radius * cos( (float)j/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );         
	  }
      glEnd();
   }
}

// Initialization routine.
void setup(void)
{   
   glEnable(GL_DEPTH_TEST); // Enable depth testing.
   
   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);
   
   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = { 0.0, 1.5, 3.0, 0.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
 
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Material property vectors.
   float matSpec[] = { 0.5, 0.5, 0.5, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   glEnable(GL_AUTO_NORMAL); // Automatic normal calculation for Bezier surfaces.
   glEnable(GL_NORMALIZE); // Automatically resize all normals to unit length.

   // Create the new quadric object.
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL); // Draw qobj filled.
   gluQuadricNormals(qobj, GLU_SMOOTH); // Automatic normal generation per vertex for qobj.

   // Generate display list base.
   base = glGenLists(7); 

   // Draw a ship storey.
   glNewList(base, GL_COMPILE);
      glPushMatrix(); 
      glRotatef(90.0, 1.0, 0.0, 0.0);
	  glColor3f(1.0, 0.0, 0.0);
      gluCylinder(qobj, 1.0, 1.0, 1.0, 15, 5);
	  glColor3f(0.0, 1.0, 1.0);
      gluDisk(qobj, 0.0, 1.0, 10, 5);
	  glPopMatrix();
   glEndList();

   // Draw ship.
   glNewList(base+1, GL_COMPILE);
      glPushMatrix(); 
   
	  // Draw hull.
      glColor3f(0.0, 0.0, 1.0);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 6, 0, 1, 18, 6, controlPointsHull[0][0]);
      glEnable(GL_MAP2_VERTEX_3); 
      glMapGrid2f(20, 1.0, 0.0, 20, 0.0, 1.0); 
      glEvalMesh2(GL_FILL, 0, 20, 0, 20);

      // Draw deck.
      glColor3f(0.0, 1.0, 1.0);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 6, controlPointsDeck[0][0]);
      glEnable(GL_MAP2_VERTEX_3); 
      glMapGrid2f(5, 1.0, 0.0, 20, 0.0, 1.0); 
      glEvalMesh2(GL_FILL, 0, 5, 0, 20);

      // Draw 1st storey.
      glPushMatrix();
      glTranslatef(0.0, 3.0, -2.0);
      glScalef(0.5, 0.5, 4.0);
      glCallList(base);
      glPopMatrix();

      // Draw 2nd storey.
      glPushMatrix();
      glTranslatef(0.0, 3.5, -2.0);
      glScalef(0.4, 0.5, 3.2);
      glCallList(base);
      glPopMatrix();

      // Draw 3rd storey.
      glPushMatrix();
      glTranslatef(0.0, 4.0, -2.0);
      glScalef(0.3, 0.5, 2.4);
      glCallList(base);
      glPopMatrix();

      // Draw chimney.
      glColor3f(0.0, 0.0, 1.0);
      glPushMatrix();
      glTranslatef(0.0, 4.75, -3.5);
      glRotatef(90.0, 1.0, 0.0, 0.0);
      gluCylinder(qobj, 0.2, 0.2, 0.75, 15, 5);
      glPopMatrix();

	  glPopMatrix();
   glEndList();

   // Draw background boat.
   glNewList(base+2, GL_COMPILE);
      glDisable(GL_LIGHTING);
	  glPushMatrix();
      glColor3f(0.15, 0.15, 0.15);
      glBegin(GL_POLYGON);
         glVertex3f(-2.0, -0.5, 0.0);
	     glVertex3f(-3.0, 0.5, 0.0);
         glVertex3f(3.0, 0.5, 0.0);
		 glVertex3f(2.0, -0.5, 0.0);
      glEnd();

      glBegin(GL_POLYGON);
         glVertex3f(-1.0, 0.5, 0.0);
         glVertex3f(-1.0, 1.0, 0.0);
         glVertex3f(1.0, 1.0, 0.0);
         glVertex3f(1.0, 0.5, 0.0);
      glEnd();
	  glPopMatrix();
	  glEnable(GL_LIGHTING);
   glEndList();

   // Draw sea.
   glNewList(base+3, GL_COMPILE);
      glDisable(GL_LIGHTING);
	  glPushMatrix();
	  glColor3f(0.0, 0.2, 0.0);
      glTranslatef(0.0, -5.0, -5.0);
      glScalef(50.0, 10.0, 40.0);
      glutSolidCube(1.0);
	  glPopMatrix();
      glEnable(GL_LIGHTING);
   glEndList();

   // Draw torpedo fin.
   glNewList(base+4, GL_COMPILE);
      glPushMatrix(); 
	  glColor3f(1.0, 0.0, 0.0);
      glTranslatef(0.0, 0.5, 1.0);
	  glRotatef(-90.0, 0.0, 1.0, 0.0);
	  gluPartialDisk(qobj, 0.5, 1.0, 10, 5, 0.0, 90.0);
	  glPopMatrix();
   glEndList();

   // Draw torpedo propeller blade.
   glNewList (base+5, GL_COMPILE);
      glPushMatrix();
	  glColor3f(1.0, 1.0, 0.0);   
	  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, controlPointsPropellerBlade[0][0]);
      glEnable(GL_MAP2_VERTEX_3);
      glMapGrid2f(5, 1.0, 0.0, 5, 0.0, 1.0);
      glTranslatef(0.0, 0.2, 0.0);
      glEvalMesh2(GL_FILL, 0, 5, 0, 5);
	  glPopMatrix();
   glEndList();

   // Draw torpedo.
   glNewList (base+6, GL_COMPILE);
      glPushMatrix();    

      // Draw body.
	  glColor3f(1.0, 0.0, 0.0);
      gluCylinder(qobj, 0.5, 0.5, 3.0, 15, 15);

      // Draw nose.
      glPushMatrix();
	  glColor3f(1.0, 0.0, 0.0);
      glTranslatef(0.0, 0.0, 3.0);
      glRotatef(90.0, 1.0, 0.0, 0.0);
      drawHemisphere(0.5, 10, 10);
      glPopMatrix();

      // Draw three fins.
      glPushMatrix();
      glCallList(base+4);
      glRotatef(120.0, 0.0, 0.0, 1.0);
      glCallList(base+4);
      glRotatef(120.0, 0.0, 0.0, 1.0);
      glCallList(base+4);
      glPopMatrix();

      // Draw backside disk.
	  glColor3f(1.0, 0.0, 0.0);
      gluDisk(qobj, 0.0, 0.5, 10, 10);

      // Propeller stem.
      glPushMatrix();
      glTranslatef(0.0, 0.0, -0.5);
	  glColor3f(1.0, 0.0, 0.0);
      gluCylinder(qobj, 0.2, 0.2, 0.5, 5, 5); 
      glPopMatrix();

	  glPopMatrix();
   glEndList();
}

// Drawing routine.
void drawScene(void)
{
   int i, randBit;
   float height, ratio;
   
   if (t < 450.0) 
   {
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 5.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   glPushMatrix(); // Begin draw ship.
   
   glTranslatef( sin(15.0*PI/180.0)*t/9.0, 0.0, cos(15.0*PI/180.0)*t/9.0 );
   glTranslatef(-4.0, -1.5, -20.0);
   glRotatef(15.0, 0.0, 1.0, 0.0); 

   glPushMatrix(); // Begin rock ship, dont rock smoke.
   randBit = rand() % 2;
   if (!(t%3)) glRotatef(randBit, 1.0, 0.0, 0.0);
   glCallList(base+1); // Draw ship.
   glPopMatrix(); // End rock ship, dont rock smoke.

   // Smoke is drawn in the display routine, and not in a display list in the initialization
   // routine, because the random bits have to be generated during run time.
   glDisable(GL_LIGHTING); // Begin draw smoke.
   gluQuadricDrawStyle(qobj, GLU_POINT); 
   glPushMatrix();
   glTranslatef(0.0, 4.75, -3.5);
   for (i=0; i<3*N/4; i++)
   {
      height = (i + t/3) % N;
	  ratio = (float)height / (float)N;
	  glPushMatrix();
	  randBit = rand()%3;
	  if (randBit) randBit = 1; 
      glColor3f(0.75 - randBit*ratio*0.75, 0.75 - randBit*ratio*0.75, 0.75 - randBit*ratio*0.75);
      glTranslatef(0.0, randBit*ratio*3.0, 0.0);  
	  glRotatef(90.0, 1.0, 0.0, 0.0);
	  gluDisk(qobj, 0.0, 0.2 + randBit*ratio*0.2, 10, 10);
	  glPopMatrix();
   }
   glPopMatrix();
   gluQuadricDrawStyle(qobj, GLU_FILL);
   glEnable(GL_LIGHTING); // End draw smoke.

   glPopMatrix(); // End draw ship.

   glPushMatrix(); // Begin draw 1st background boat.
   glTranslatef(-20.0, 0.0, -25.0); 
   randBit = rand()%2;
   if (!(t%3)) glRotatef(randBit*5.0, 0.0, 0.0, 1.0);
   glCallList(base+2);
   glPopMatrix(); // End draw 1st background boat.

   glPushMatrix(); // Begin draw 2nd background boat.
   glTranslatef(15.0, 0.0, -25.0); 
   glRotatef(90.0, 0.0, 1.0, 0.0);
   randBit = rand()%2;
   if (!(t%3)) glRotatef(randBit*5.0, 0.0, 0.0, 1.0);
   glCallList(base+2);
   glPopMatrix(); // End draw 2nd background boat.

   glPushMatrix(); // Begin draw 3rd background boat.
   glTranslatef(0.0, 0.0, -25.0); 
   glRotatef(120.0, 0.0, 1.0, 0.0);
   randBit = rand()%2;
   if (!(t%3)) glRotatef(randBit*5.0, 0.0, 0.0, 1.0);
   glScalef(1.5, 1.0, 1.0);
   glCallList(base+2);
   glPopMatrix(); // End draw 3rd background boat.

   glPushMatrix(); // Begin draw 4th background boat.
   glTranslatef(5.0, 0.0, -25.0); 
   glRotatef(45.0, 0.0, 1.0, 0.0);
   randBit = rand()%2;
   if (!(t%3)) glRotatef(randBit*5.0, 0.0, 0.0, 1.0);
   glScalef(0.75, 1.0, 1.0);
   glCallList(base+2);
   glPopMatrix(); // End draw 4th background boat.
   
   glCallList(base+3); // Draw sea.

   glPushMatrix(); // Begin draw torpedo.

   glTranslatef( cos(75.0*PI/180.0)*t/12.0, 0.0, -sin(75.0*PI/180.0)*t/12.0 );
   glTranslatef(-6.0, 0.0, 15.5);
   glRotatef(165.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 0.0, 0.0, 1.0);

   glCallList(base+6); // Draw torpedo minus propellers.

   // Torpedo propeller blades are drawn in the display routine, and not in a display list 
   // in the initialization routine, because the angle has to picked up during run time.
   glPushMatrix(); // Begin draw three torpedo propeller blades.
   glTranslatef(0.0, 0.0, -0.5);
   glRotatef(angle, 0.0, 0.0, 1.0);
   glCallList(base+5);
   glRotatef(120.0, 0.0, 0.0, 1.0);
   glCallList(base+5);
   glRotatef(120.0, 0.0, 0.0, 1.0);
   glCallList(base+5);
   glPopMatrix(); // End draw three torpedo propeller blades.

   glPopMatrix(); // End draw torpedo.

   glutSwapBuffers();
   }
   else 
   {
   glClearColor (1.0, 1.0, 1.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glutSwapBuffers();
   if (t > 460) exit(0);
   }
   
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      t++;
      angle += 30;
	  if (angle > 360.0) angle -= 360.0;

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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout <<"Press the x, X, y, Y, z, Z keys to rotate the viewpoint." << endl 
	   << "Press space to toggle animation on/off." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
 
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("shipMovie.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop();  
}
