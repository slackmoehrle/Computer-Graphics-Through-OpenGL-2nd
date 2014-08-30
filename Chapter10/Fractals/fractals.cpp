////////////////////////////////////////////////////////////////////////////////////////         
// fractals.cpp
//
// This program draws a fractal Koch snowflake, a variant of the Koch snowflake
// and a fractal tree, all at various levels of recursion. The same data structures 
// are applied to draw each by changing only certain class member functions.
//
// Interaction:
// Press left/right arrows keys to cycle through the fractals.
// Press up/down arrow keys to increase/decrease the recursion level.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <vector>
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
#define ROOT3 1.73205081
#define ONEBYROOT3 0.57735027

#define KOCH 0
#define KOCHVARIANT 1
#define TREE 2

#define RATIO 0.85 // Growth ratio = length of tree sub-branch to length of branch.
#define ANGLE 40 // Angle between the two tree sub-branches.

using namespace std;

// Globals.
static int maxLevel = 0; // Recursion level.
static int shape = KOCH; // Shape index.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

class Source; // Make source class visible.

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

// Write message.
void writeData(void)
{
   char buffer [33];

   glColor3f(0.0, 0.0, 0.0);
   intToString(buffer, maxLevel);
   glRasterPos3f(-20.0, -45.0, 0.0);

   if (shape == KOCH) writeBitmapString((void*)font, "Koch Snowflake    Level: ");
   if (shape == KOCHVARIANT) writeBitmapString((void*)font, "Variant Koch Snowflake    Level: ");
   if (shape == TREE) writeBitmapString((void*)font, "Tree    Level: ");
   writeBitmapString((void*)font, buffer);
}

// Sequel class.
class Sequel
{
public:
   Sequel() { coords.clear(); v.clear(); }
   void drawKochOrVariant(); // Routine to draw Sequel object in case it represents 
                             // a Koch snowflake or variant Koch snowflake.
   void drawTree(); // Routine to draw Sequel object in case it represents a tree.

   friend class Source;

private:
   vector<float> coords; // Vector of x, y co-ordinates of points specifying sequel object.
   vector<Source> v; // Vector of associated source objects to be produced recursively.
};

// Routine to draw Sequel object in case it represents a Koch snowflake or variant Koch snowflake.
void Sequel::drawKochOrVariant()
{
   glBegin(GL_LINE_STRIP);
   for (int i=0; i < 5; i++)
      glVertex2f(coords[2*i], coords[2*i+1]);
   glEnd();
}

// Routine to draw Sequel object in case it represents a tree.
void Sequel::drawTree()
{
   glBegin(GL_LINE_STRIP);
   for (int i=0; i < 3; i++)
      glVertex2f(coords[2*i], coords[2*i+1]);
   glEnd();
}

// Source class.
class Source
{
public:
   Source(){ }
   Source(float coordsVal[4])
   {
      for (int i=0; i < 4; i++) coords[i] = coordsVal[i];
   }

   void draw(); // Routine to draw source line segment.

   Sequel sourceToSequelKoch(); // Routine to generate Sequel object in case of Koch snowflake.
   void produceKoch(int level); // Recursive routine to produce Koch snowflake.
   
   Sequel sourceToSequelKochVariant(); // Routine to generate Sequel object in case of variant Koch snowflake.
   void produceKochVariant(int level); // Recursive routine to produce variant Koch snowflake.

   Sequel sourceToSequelTree(); // Routine to generate Sequel object in case of tree.
   void produceTree(int level); // Recursive routine to produce tree.

   friend class Sequel;

private:
   float coords[4]; // x, y co-ordinates of a line segment endpoints.
};

// Routine to draw source line segment.
void Source::draw()
{
   glBegin(GL_LINES);
   for (int i=0; i < 2; i++)
      glVertex2f(coords[2*i], coords[2*i+1]);
   glEnd();
}

// Routine to generate Sequel object in case of Koch snowflake.
Sequel Source::sourceToSequelKoch()
{
   float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
   int i, j;
   Source s;
   Sequel seq = *new Sequel();

   x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];

   // From the co-ordinates of the two segment endpoints calculate the co-ordinates of 
   // the 5 vertices of the Koch polyline.
   coordsVal[0] = x0;
   coordsVal[1] = y0;
   coordsVal[2] = 0.66666667*x0 + 0.33333333*x1;
   coordsVal[3] = 0.66666667*y0 + 0.33333333*y1;
   coordsVal[4] = 0.5*(x0 + x1) - 0.5*ONEBYROOT3*(y1 - y0);
   coordsVal[5] = 0.5*(y0 + y1) + 0.5*ONEBYROOT3*(x1 - x0);
   coordsVal[6] = 0.33333333*x0 + 0.66666667*x1;
   coordsVal[7] = 0.33333333*y0 + 0.66666667*y1;
   coordsVal[8] = x1;
   coordsVal[9] = y1;

   // Enter Koch polyline vertices into sequel object.
   for (i=0; i < 10; i++) seq.coords.push_back(coordsVal[i]);

   // Specify all 4 segments of the Koch polyline for recursive production.
   for (i=0; i < 4; i++)
   {
      for (j=0; j < 4; j++)
	  {
	     coordsVal1[j] = coordsVal[2*i+j];
	  }
	  s = *new Source(coordsVal1);
	  seq.v.push_back(s);
   }

   return seq;
}

// Recursive routine to produce Koch snowflake.
void Source::produceKoch(int level)
{
   glColor3f(0.0, 0.0, 0.0);

   // Only sequels at given level are drawn.
   if (maxLevel == 0) this->draw();
   else if (maxLevel == 1) this->sourceToSequelKoch().drawKochOrVariant();
        else if (level < maxLevel-1)
	            for (int i=0; i < 4; i++) this->sourceToSequelKoch().v[i].produceKoch(level+1);	
             else this->sourceToSequelKoch().drawKochOrVariant();                                
}

// Routine to generate Sequel object in case of variant Koch snowflake.
Sequel Source::sourceToSequelKochVariant()
{
   float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
   int i;
   Source s;
   Sequel seq = *new Sequel();

   x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];

   // From the co-ordinates of the two segment endpoints calculate the co-ordinates of 
   // the 5 vertices of the Koch polyline.
   coordsVal[0] = x0;
   coordsVal[1] = y0;
   coordsVal[2] = 0.66666667*x0 + 0.33333333*x1;
   coordsVal[3] = 0.66666667*y0 + 0.33333333*y1;
   coordsVal[4] = 0.5*(x0 + x1) - 0.5*ONEBYROOT3*(y1 - y0);
   coordsVal[5] = 0.5*(y0 + y1) + 0.5*ONEBYROOT3*(x1 - x0);
   coordsVal[6] = 0.33333333*x0 + 0.66666667*x1;
   coordsVal[7] = 0.33333333*y0 + 0.66666667*y1;
   coordsVal[8] = x1;
   coordsVal[9] = y1;

   // Enter Koch polyline vertices into sequel object.
   for (i=0; i < 10; i++) seq.coords.push_back(coordsVal[i]);
   
   // Specify the segment joining the first vertex of the Koch polyline to the middle vertex
   // and the segment joining the middle vertex to the last vertex for recursive production.
   for (i=0; i < 2; i++)
   {
	  coordsVal1[0] = coordsVal[4*i];
	  coordsVal1[1] = coordsVal[4*i+1];
	  coordsVal1[2] = coordsVal[4*i+4];
	  coordsVal1[3] = coordsVal[4*i+5];
	  s = *new Source(coordsVal1);
	  seq.v.push_back(s);
   }

   return seq;
}

// Recursive routine to produce variant Koch snowflake.
void Source::produceKochVariant(int level)
{
   glColor3f(0.0, 0.0, 0.0);

   // Only sequels at given level are drawn.
   if (maxLevel == 0) this->draw();
   else if (maxLevel == 1) this->sourceToSequelKochVariant().drawKochOrVariant();
        else if (level < maxLevel-1)
	            for (int i=0; i < 2; i++) this->sourceToSequelKochVariant().v[i].produceKochVariant(level+1);	
             else this->sourceToSequelKochVariant().drawKochOrVariant();                                
}

// Routine to generate Sequel object in case of tree.
Sequel Source::sourceToSequelTree()
{
   float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
   int i, j;
   Source s;
   Sequel seq = *new Sequel();

   x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];

   // From the co-ordinates of the two segment endpoints calculate the co-ordinates of
   // the 3 vertices of the sub-branch V-shape.
   coordsVal[0] = x1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(x1-x0) - RATIO*sin((PI/180.0)*ANGLE/2.0)*(y1-y0);
   coordsVal[1] = y1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(y1-y0) + RATIO*sin((PI/180.0)*ANGLE/2.0)*(x1-x0);
   coordsVal[2] = x1;
   coordsVal[3] = y1;
   coordsVal[4] = x1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(x1-x0) + RATIO*sin((PI/180.0)*ANGLE/2.0)*(y1-y0);
   coordsVal[5] = y1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(y1-y0) - RATIO*sin((PI/180.0)*ANGLE/2.0)*(x1-x0);

   // Enter V-shape vertices into sequel object.
   for (i=0; i < 6; i++) seq.coords.push_back(coordsVal[i]);

   // Specify both sub-branch segments of the V-shape for recursive production.
   for (i=0; i < 2; i++)
   {
	  coordsVal1[0] = coordsVal[2];
	  coordsVal1[1] = coordsVal[3];
      for (j=2; j < 4; j++)
	  {
	     coordsVal1[j] = coordsVal[4*i+j-2];
	  }
	  s = *new Source(coordsVal1);
	  seq.v.push_back(s);
   }

   return seq;
}

// Routine to draw leaf called by the following routine to produce tree.
void drawLeaf(float x, float y)
{
   glColor3f(0.0, 1.0, 0.0);
   glPushMatrix();
   glTranslatef(x, y, 0.0);
   glRotatef(rand()%360, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(1.0, 2.0);
	  glVertex2f(0.0, 4.0);
	  glVertex2f(-1.0, 2.0);
   glEnd();
   glPopMatrix();
}

// Recursive routine to produce tree.
void Source::produceTree(int level)
{
   glColor3f(0.4, 0.5, 0.5);

   // Branches are thinner up the tree.
   glLineWidth(2*(maxLevel - level));

   // Source and sequels at all prior levels are drawn (different from Kock and Koch variant).
   if (maxLevel == 0) this->draw();
   else if (maxLevel == 1) {this->draw(); this->sourceToSequelTree().drawTree();}
        else if (level < maxLevel)
		{
		   if (level == 0) this->draw();
		   this->sourceToSequelTree().drawTree(); 
	       for (int i=0; i < 2; i++) this->sourceToSequelTree().v[i].produceTree(level+1);
		} 

   // Embellish with leaves.
   if (level == maxLevel-1) 
   {
      drawLeaf(this->sourceToSequelTree().coords[0], this->sourceToSequelTree().coords[1]);
	  drawLeaf(this->sourceToSequelTree().coords[4], this->sourceToSequelTree().coords[5]);
   }
   
   // Restore line width.
   glLineWidth(1.0);
}


// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   float coordsVal1[4] = {-30.0, -15.0, 0.0, -15.0 + ROOT3*0.5*60.0}; 
   float coordsVal2[4] = {0.0, -15.0 + ROOT3*0.5*60.0, 30.0, -15.0};
   float coordsVal3[4] = {30.0, -15.0, -30.0, -15.0}; 
   float coordsVal4[4] = {0.0, -30.0, 0.0, -15.0};

   Source src1 = *new Source(coordsVal1); // Edge of an equilateral triangle.
   Source src2 = *new Source(coordsVal2); // Edge of an equilateral triangle.
   Source src3 = *new Source(coordsVal3); // Edge of an equilateral triangle.
   Source src4 = *new Source(coordsVal4); // Vertical line segment.

   writeData();

   if (shape == KOCH)
   // Produce on all three edges of an equilateral triangle.
   {
      src1.produceKoch(0);
      src2.produceKoch(0);
      src3.produceKoch(0);
   }

   if (shape == KOCHVARIANT)
   // Produce on all three edges of an equilateral triangle.
   {
      src1.produceKochVariant(0);
      src2.produceKochVariant(0);
      src3.produceKochVariant(0);
   }

   if (shape == TREE) 
   // Produce on a vertical line segment.	   
   src4.produceTree(0);

   glFlush();
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
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP) maxLevel++;
   if( key == GLUT_KEY_DOWN) if (maxLevel > 0) maxLevel--;
   if (key == GLUT_KEY_RIGHT) if (shape < 2) shape++; else shape = 0;
   if (key == GLUT_KEY_LEFT) if (shape > 0) shape--; else shape = 2;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press left/right arrows keys to cycle through the fractals." << endl
        << "Press up/down arrow keys to increase/decrease the recursion level." << endl; 
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
   glutCreateWindow("fractals.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();  
}
