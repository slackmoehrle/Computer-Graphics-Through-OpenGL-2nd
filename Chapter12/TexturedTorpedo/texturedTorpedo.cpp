/////////////////////////////////////////////////////////////////////////////////////////////      
// texturedTorpedo.cpp
//
// This program is an enhancement of torpedo.cpp with a texturing of the body and the blades.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the x, X, y, Y, z, Z keys to rotate the viewpoint.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
/////////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

using namespace std;

#define PI 3.14159265358979324

// Begin globals.
static unsigned int base; // Base index for display lists.
static GLUquadricObj *qobj; // Pointer to GLU quadric object.

// Control points for a propeller blade Bezier surface.
static float controlPointsPropellerBlade[4][3][3] = 
{
	{{-0.2, 0.2, -0.05}, {0.0, 0.0, 0.0}, {0.2,  0.2, 0.05}},
	{{-0.2, 0.4, -0.1}, {0.0, 0.4, 0.0}, {0.2,  0.4, 0.1}},
	{{-0.2, 0.6, -0.2}, {0.0, 0.6, 0.0}, {0.2, 0.6, 0.2}},
	{{-0.2, 0.8, -0.2}, {0.0, 1.0, 0.0}, {0.2, 0.8, 0.2}},
};

// Control points for the texture co-ordinates Bezier surface.
static float texturePoints[2][2][2] = 
{
	{{0.0, 0.0}, {0.0, 1.0}}, 
	{{1.0, 0.0}, {1.0, 1.0}}
};

static float Xangle = 30.0, Yangle = 10.0, Zangle = 0.0; // Angles to rotate torpedo.
static int isAnimate = 0; // In animation mode?
static int animationPeriod = 1; // Speed of animation.
static float angle = 0; // Angle of torpedo propeller turn.
static unsigned char chessboard[64][64][4]; // Storage for chessboard image.
static unsigned char stripes[64][64][4]; // Storage for stripes image.
static unsigned int texture[2]; // Array of texture indices.
// End globals.

// Load program-generated images as textures. 
void loadProceduralTextures()			
{
   // Bind chessboard image to texture index[0].
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessboard);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Bind stripes image to texture index[1].
   glBindTexture(GL_TEXTURE_2D, texture[1]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, stripes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

// Create 64 x 64 RGBA image of a chessboard.
void createChessboard(void)
{
   int i, j;
   for (i = 0; i < 64; i++) 
      for (j = 0; j < 64; j++) 
         if ( ( ((i/8)%2) && ((j/8)%2) ) || ( !((i/8)%2) && !((j/8)%2) ) )
		 {
            chessboard[i][j][0] = 0x00;
            chessboard[i][j][1] = 0x00;
            chessboard[i][j][2] = 0x00;
            chessboard[i][j][3] = 0xFF;
		 }
		 else
		 {
            chessboard[i][j][0] = 0xFF;
            chessboard[i][j][1] = 0x00;
            chessboard[i][j][2] = 0x00;
            chessboard[i][j][3] = 0xFF;
		 }
}

// Create 64 x 64 RGBA image of a striped texture.
void createStripes(void)
{
   int i, j;
   for (i = 0; i < 64; i++) 
      for (j = 0; j < 64; j++) 
		 if ( (j/8)%2 )
		 {
            stripes[i][j][0] = 0xFF;
            stripes[i][j][1] = 0x00;
            stripes[i][j][2] = 0x00;
            stripes[i][j][3] = 0xFF;
		 }
		 else
		 {
            stripes[i][j][0] = 0x00;
            stripes[i][j][1] = 0x00;
            stripes[i][j][2] = 0xFF;
            stripes[i][j][3] = 0xFF;
		 }
}

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
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Create texture ids.
   glGenTextures(2, texture); 
   
   // Create and load procedural textures.
   createChessboard();
   createStripes();
   loadProceduralTextures();

   // Initialize GLU quadric object.
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_LINE);  

   // Generate display list base.
   base = glGenLists(2); 

   // Fin.
   glNewList(base, GL_COMPILE);
      glPushMatrix(); 
      glTranslatef(0.0, 0.5, 1.0);
	  glRotatef(-90.0, 0.0, 1.0, 0.0);
	  gluPartialDisk(qobj, 0.5, 1.0, 10, 5, 0.0, 90.0);
	  glPopMatrix();
   glEndList();

   // Propeller blade.
   glNewList(base+1, GL_COMPILE);
      glPushMatrix();
	  
	  // Enable texturing and filled polygon mode.
	  glEnable(GL_TEXTURE_2D);
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      // Enable Bezier surface with texture co-ordinates generation.
	  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 3, 0, 1, 9, 4, controlPointsPropellerBlade[0][0]);
      glEnable(GL_MAP2_VERTEX_3);
	  glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, texturePoints[0][0]);
	  glEnable(GL_MAP2_TEXTURE_COORD_2);
      glMapGrid2f(5, 0.0, 1.0, 5, 0.0, 1.0);

      glTranslatef(0.0, 0.2, 0.0);

	  // Map the chessboard texture onto the blade Bezier surface.
	  glBindTexture(GL_TEXTURE_2D, texture[0]);  
      glEvalMesh2(GL_FILL, 0, 5, 0, 5);
	  
	  // Disable texturing and return to wireframe polygon mode.
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  glDisable(GL_TEXTURE_2D);
	  
	  glPopMatrix();
   glEndList();

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
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
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(0.0, 0.0, 0.0);
   
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Torpedo body.
   // Enable texturing and filled polygon mode.
   glEnable(GL_TEXTURE_2D);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Initialize quadric.
   gluQuadricDrawStyle(qobj, GLU_FILL);  
   gluQuadricTexture(qobj, GL_TRUE);

   // Map the stripes texture onto the body quadric.
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   gluCylinder(qobj, 0.5, 0.5, 3.0, 15, 15);
   
   // Disable texturing and return to wireframe polygon mode.
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glDisable(GL_TEXTURE_2D);

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
   glMatrixMode(GL_PROJECTION);
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

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("texturedTorpedo.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();  
}
