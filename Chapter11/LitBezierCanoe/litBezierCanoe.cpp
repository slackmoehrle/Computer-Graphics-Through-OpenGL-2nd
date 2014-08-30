//////////////////////////////////////////////////////////////////////////////////        
// litBezierCanoe.cpp
//
// This program builds on the final shape of bezierCanoe.cpp by coloring the canoe 
// and adding a single directional light source.
//
// Interaction:
// Press the x, X, y, Y, z, Z keys to rotate the viewpoint.
//
// Sumanta Guha.
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
// Control points for Bezier surface.
static float controlPoints[6][4][3] = 
{
	{{0.0, 2.75, -5.0}, {0.0, 0.0, -5.0}, {0.0, 0.0, -5.0}, {0.0, 2.75, -5.0}},
	{{1.0, 2.75, -3.0}, {0.25, 0.0, -3.0}, {-0.25, 0.0, -3.0}, {-1.0, 2.75, -3.0}},
	{{1.5, 2.75, -1.0}, {0.25, 0.0, -1.0}, {-0.25, 0.0, -1.0}, {-1.5, 2.75, -1.0}},
	{{1.5, 2.75, 1.0}, {0.25, 0.0, 1.0}, {-0.25, 0.0, 1.0}, {-1.5, 2.75, 1.0}},
	{{1.0, 2.75, 3.0}, {0.25, 0.0, 3.0}, {-0.25, 0.0, 3.0}, {-1.0, 2.75, 3.0}},
	{{0.0, 2.75, 5.0}, {0.0, 0.0, 5.0}, {0.0, 0.0, 5.0}, {0.0, 2.75, 5.0}}
};

static float Xangle = 30.0, Yangle = 330.0, Zangle = 0.0; // Angles to rotate canoe.
// End globals.

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
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
   float matAmbAndDif1[] = {0.9, 0.0, 0.0, 1.0};
   float matAmbAndDif2[] = {0.0, 0.9, 0.0, 1.0};
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, controlPoints[0][0]); // Specify the Bezier surface.
   glEnable(GL_MAP2_VERTEX_3); // Enable the Bezier surface.

   glEnable(GL_AUTO_NORMAL); // Enable automatic normal calculation.
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate scene.
   glRotatef (Zangle, 0.0, 0.0, 1.0);
   glRotatef (Yangle, 0.0, 1.0, 0.0);
   glRotatef (Xangle, 1.0, 0.0, 0.0);

   // Make a mesh approximation of the Bezier surfacee.
   glMapGrid2f(20, 1.0, 0.0, 20, 0.0, 1.0); // IMPORTANT: The direction of the sample grid is
                                            // reversed along the u-direction to get the right
                                            // normal directions (compare bezierCanoe.cpp).
   // glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

   glEvalMesh2(GL_FILL, 0, 20, 0, 20);

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
   cout <<"Press the x, X, y, Y, z, Z keys to rotate the viewpoint." << endl;
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
   glutCreateWindow("litBezierCanoe.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 
 
   glutMainLoop();  
}
