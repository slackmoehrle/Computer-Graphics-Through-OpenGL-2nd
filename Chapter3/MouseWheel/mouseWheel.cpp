///////////////////////////////////////////////////////////////////////////////////         
// mouseWheel.cpp
// 
// This program enhances mouseMotion.cpp to with the capability of changing the 
// point size by turning the mouse wheel.
// 
// Interaction:
// Left mouse click to draw a square point, keep left button pressed to drag point, 
// right mouse click to exit, rotate wheel to change the point size.
// 
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <vector>
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


// Use the STL extension of C++.
using namespace std;

// Globals.
static int width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point

// Point class.
class Point
{
public:
   Point(int x, int y)
   {
	  xVal = x; yVal = y; size = pointSize;
   }
   Point(){};
   void setCoords(int x, int y)
   {
	  xVal = x; yVal = y;
   }
   void drawPoint(void); // Function to draw a point.
private:
   int xVal, yVal; // x and y co-ordinates of point.
   static float size; // Size of point.
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint(void)
{  
   // Change from mouseMotion.cpp: the Point member size is not invoked here.
   glBegin(GL_POINTS);
      glVertex3f(xVal, yVal, 0.0);
   glEnd();   
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 

// Currently clicked point.
Point currentPoint;

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0); 

   // Change from mouseMotion.cpp: the drawing size of all points is set by
   // the global pointSize, instead of the Point member variable size.
   glPointSize(pointSize);

   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() ) 
   {	   
	  pointsIterator->drawPoint();
	  pointsIterator++;
   }
 
   currentPoint.drawPoint();

   glFlush();
}

// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   // Store the clicked point in the currentPoint variable when left button is pressed.
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      currentPoint = Point(x, height - y); 

   // Store the currentPoint in the points vector when left button is released.
   if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
      points.push_back(currentPoint);
   
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
   
   glutPostRedisplay();
}

// Mouse motion callback routine.
void mouseMotion(int x, int y)
{
   // Update the location of the current point as the mouse moves with button pressed.
   currentPoint.setCoords(x, height - y);   

   glutPostRedisplay();
}

// Mouse wheel callback routine.
void mouseWheel(int wheel, int direction, int x, int y) 
{
   // Increment/decrement the global pointSize depending on the direction 
   // of rotation of the mouse wheel.
   (direction > 0) ? pointSize++ : pointSize--;

   glutPostRedisplay();
}  

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h; 

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
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Left mouse click to draw a square point, keep left button pressed to drag point," << endl 
        << "right mouse click to exit, rotate wheel to change the point size." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("mouseWheel.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   // Register the mouse callback function.
   glutMouseFunc(mouseControl); 

   // Register the mouse motion callback function.
   glutMotionFunc(mouseMotion);

   // Register the mouse wheel callback function.
   glutMouseWheelFunc(mouseWheel);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}