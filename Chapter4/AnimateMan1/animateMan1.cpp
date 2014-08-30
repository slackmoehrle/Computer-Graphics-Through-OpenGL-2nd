//////////////////////////////////////////////////////////////////////////////////////////////         
// animateMan1.cpp
// 
// This program draws a man with movable parts who can be animated. The program has two modes:
// (a) develop mode in which key frames are created.
// (b) animate mode in which animation is shown.
//
// At the end of the develop mode configurations data is written to the file animateManDataOut.txt. 
//
// Interaction:
// Press a to toggle between develop and animate modes.
//
// In develop mode:
// Press the space bar to select a part.
// Press the page up/page down keys to rotate the selected part.
// Press the left/right/up/down arrow keys to move the whole configuration.
// Press r/R to rotate the viewpoint.
// Press z/Z to zoom in/out.
// Press n to create a new configuration - other configurations are ghosted
// (the new configuration is a copy of the current one so it must be moved to be visible).
// Press tab to choose a configuration - it is highlighted, others ghosted.
// Press backspace to reset current configuration.
// Press delete to delete current configuration.
//
// In animate mode:
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
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
static float highlightColor[3] = {0.0, 0.0, 0.0}; // Emphasize color.
static float lowlightColor[3] = {0.7, 0.7, 0.7}; // De-emphasize color.
static float partSelectColor[3] = {1.0, 0.0, 0.0}; // Selection indicate color.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int animateMode = 0; // In animation mode?
static int animationPeriod = 1000; // Time interval between frames.
static ofstream outFile; // File to write configurations data.

// Camera class.
class Camera
{
public:
   Camera();
   void incrementViewDirection();
   void decrementViewDirection();
   void incrementZoomDistance() { zoomDistance += 1.0; }
   void decrementZoomDistance() { zoomDistance -= 1.0; }

   float getViewDirection() const { return viewDirection; }
   float getZoomDistance() const { return zoomDistance; }

private:
   float viewDirection;
   float zoomDistance;
};

// Global camera.
Camera camera;

// Camera constructor.
Camera::Camera()
{
   viewDirection = 0.0;
   zoomDistance = 30.0;
}

// Function to increment camera viewing angle.
void Camera::incrementViewDirection()
{
   viewDirection += 5.0;
   if (viewDirection > 360.0) viewDirection -= 360.0;
}

// Function to decrement camera viewing angle.
void Camera::decrementViewDirection()
{
   viewDirection -= 5.0;
   if (viewDirection < 0.0) viewDirection += 360.0;
}

// Man class.
class Man
{
public:
   Man();
   void incrementSelectedPart();

   void incrementPartAngle();
   void decrementPartAngle();
   void setPartAngle(float angle) { partAngles[selectedPart] = angle; }

   void incrementUpMove() { upMove += 0.1; }
   void decrementUpMove() { upMove -= 0.1; }
   void setUpMove(float move) { upMove = move; }

   void incrementForwardMove() { forwardMove += 0.1; }
   void decrementForwardMove() { forwardMove -= 0.1; }
   void setForwardMove(float move) { forwardMove = move; }

   void setHighlight(int inputHighlight) { highlight = inputHighlight; }

   void draw();
   void outputData();
   void writeData();

private:
   // Man configuration values.
   float partAngles[9]; // Angles from 0 to 360 of 9 body parts - torso, left and right
                          // upper arms, left and right lower arms, left and right upper
                          // legs, left and right lower legs. 
                          // All parts move parallel to the same plane.

   float upMove, forwardMove; // Up and forward translation components - both lie
                                // on the plane parallel to which parts rotate -
                                // therefore all translations and part rotations
                                // are along one fixed plane.
        
   int selectedPart; // Selected part number - this part can be interactively rotated
                       // in the develop mode.

   int highlight; // If man is currently selected.
};

// Global vector of man configurations.
vector<Man> manVector;

// Global iterators to traverse manVector.
vector<Man>::iterator manVectorIterator;
vector<Man>::iterator manVectorAnimationIterator;

// Man constructor.
Man::Man()
{
   for (int i=0; i<9; i++) partAngles[i] = 0.0;
   upMove = 0.0;
   forwardMove = 0.0;
   selectedPart = 0;
   highlight = 1;
}

// Function to incremented selected part..
void Man::incrementSelectedPart()
{
   if (selectedPart < 8) selectedPart++;
   else selectedPart = 0;
}

// Function to increment angle of selected part.
void Man::incrementPartAngle()
{
   partAngles[selectedPart] += 5.0;
   if (partAngles[selectedPart] > 360.0) partAngles[selectedPart] -= 360.0;
}

// Function to decrement angle of selected part.
void Man::decrementPartAngle()
{
   partAngles[selectedPart] -= 5.0;
   if (partAngles[selectedPart] < 0.0) partAngles[selectedPart] += 360.0; 
}

// Function to draw man.
void Man::draw()
{
   if (highlight||animateMode) glColor3fv(highlightColor);
   else glColor3fv(lowlightColor);

   glPushMatrix(); 

   // Up and forward translations.
   glTranslatef(0.0, upMove, forwardMove);

   // Torso begin.
   if (highlight && !animateMode) if (selectedPart == 0) 
      glColor3fv(partSelectColor);

   glRotatef(partAngles[0], 1.0, 0.0, 0.0);

   glPushMatrix();
   glScalef(4.0, 16.0, 4.0);
   glutWireCube(1.0);
   glPopMatrix();
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Torso end.
   
   // Head begin.
   glPushMatrix();

   glTranslatef(0.0, 11.5, 0.0);
   glPushMatrix();
   glScalef(2.0, 3.0, 2.0);
   glutWireSphere(1.0, 10, 8);
   glPopMatrix();

   glPopMatrix();
   // Head end.

   // Left upper and lower arm begin.
   glPushMatrix();

   // Left upper arm begin.
   if (highlight && !animateMode) if (selectedPart == 1) glColor3fv(partSelectColor);
   glTranslatef(3.0, 8.0, 0.0);
   glRotatef(180.0 + partAngles[1], 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);
   glPushMatrix(); 
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix(); 
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left upper arm end.

   // Left lower arm begin.
   if (highlight && !animateMode) if (selectedPart == 2) glColor3fv(partSelectColor);
   glTranslatef(0.0, 4.0, 0.0);
   glRotatef(partAngles[2], 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);
   glPushMatrix();
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix();
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left lower arm end.

   glPopMatrix();
   // Left upper and lower arm end.

   // Right upper and lower arm begin.
   glPushMatrix();

   // Right upper arm begin.
   if (highlight && !animateMode) if (selectedPart == 3) glColor3fv(partSelectColor);
   glTranslatef(-3.0, 8.0, 0.0);
   glRotatef(180.0 + partAngles[3], 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);
   glPushMatrix(); 
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix();
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right upper arm end.

   // Right lower arm begin.
   if (highlight && !animateMode) if (selectedPart == 4) glColor3fv(partSelectColor);
   glTranslatef(0.0, 4.0, 0.0);
   glRotatef(partAngles[4], 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);
   glPushMatrix();
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix();
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right lower arm end.

   glPopMatrix();
   // Right upper and lower arm end.

   // Left upper and lower leg with foot begin.
   glPushMatrix();

   // Left upper leg begin.
   if (highlight && !animateMode) if (selectedPart == 5) glColor3fv(partSelectColor);
   glTranslatef(1.5, -8.0, 0.0);
   glRotatef(partAngles[5], 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);
   glPushMatrix(); 
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix(); 
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left upper leg end.

   // Left lower leg with foot begin.
   if (highlight && !animateMode) if (selectedPart == 6) glColor3fv(partSelectColor);
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(partAngles[6], 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);
   
   // Lower leg.
   glPushMatrix();
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix();

   // Foot.
   glTranslatef(0.0, -5.0, 0.5);
   glPushMatrix();
   glScalef(2.0, 1.0, 3.0);
   glutWireCube(1.0);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left lower leg with foot end.

   glPopMatrix();
   // Left upper and lower leg with foot end.

   // Right upper and lower leg with foot begin.
   glPushMatrix();

   // Right upper leg begin.
   if (highlight && !animateMode) if (selectedPart == 7) glColor3fv(partSelectColor);
   glTranslatef(-1.5, -8.0, 0.0);
   glRotatef(partAngles[7], 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);
   glPushMatrix(); 
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix(); 
   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right upper leg end.

   // Right lower leg with foot begin.
   if (highlight && !animateMode) if (selectedPart == 8) glColor3fv(partSelectColor);
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(partAngles[8], 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);

   // Lower leg.
   glPushMatrix();
   glScalef(2.0, 8.0, 2.0);
   glutWireCube(1.0);
   glPopMatrix();

   // Foot.
   glTranslatef(0.0, -5.0, 0.5);
   glPushMatrix();
   glScalef(2.0, 1.0, 3.0);
   glutWireCube(1.0);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right lower leg with foot end.

   glPopMatrix();
   // Right upper and lower leg with foot end.

   glPopMatrix(); 
}

// Function to output configurations data to file.
void Man::outputData()
{
	int i;
	for (i = 0; i < 9; i++) outFile << partAngles[i] << " ";
	outFile << upMove << " " << forwardMove << endl;
}

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

// Routine to write configurations data.
void Man::writeData()
{
   char buffer [33];
   
   floatToString(buffer, 4, partAngles[0]);	
   glRasterPos3f(-28.0, 10.0, 0.0);
   writeBitmapString((void*)font, "torso = "); 
   glRasterPos3f(-11.0, 10.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[1]);	
   glRasterPos3f(-28.0, 8.0, 0.0);
   writeBitmapString((void*)font, "left upper arm = "); 
   glRasterPos3f(-11.0, 8.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[2]);	
   glRasterPos3f(-28.0, 6.0, 0.0);
   writeBitmapString((void*)font, "left lower arm = "); 
   glRasterPos3f(-11.0, 6.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[3]);	
   glRasterPos3f(-28.0, 4.0, 0.0);
   writeBitmapString((void*)font, "right upper arm = "); 
   glRasterPos3f(-11.0, 4.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[4]);	
   glRasterPos3f(-28.0, 2.0, 0.0);
   writeBitmapString((void*)font, "right lower arm = "); 
   glRasterPos3f(-11.0, 2.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[5]);	
   glRasterPos3f(-28.0, 0.0, 0.0);
   writeBitmapString((void*)font, "left uppper leg = "); 
   glRasterPos3f(-11.0, 0.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[6]);	
   glRasterPos3f(-28.0, -2.0, 0.0);
   writeBitmapString((void*)font, "left lower leg = "); 
   glRasterPos3f(-11.0, -2.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[7]);	
   glRasterPos3f(-28.0, -4.0, 0.0);
   writeBitmapString((void*)font, "right upper leg = "); 
   glRasterPos3f(-11.0, -4.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, partAngles[8]);	
   glRasterPos3f(-28.0, -6.0, 0.0);
   writeBitmapString((void*)font, "right lower leg = "); 
   glRasterPos3f(-11.0, -6.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, upMove);	
   glRasterPos3f(-28.0, -8.0, 0.0);
   writeBitmapString((void*)font, "upMove = "); 
   glRasterPos3f(-11.0, -8.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, forwardMove);	
   glRasterPos3f(-28.0, -10.0, 0.0);
   writeBitmapString((void*)font, "forwardMove = "); 
   glRasterPos3f(-11.0, -10.0, 0.0);
   writeBitmapString((void*)font, buffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   // Write text in isolated (i.e., before gluLookAt) translate block.
   glPushMatrix();
   glTranslatef(0.0, 0.0, -30.0);
   glColor3fv(highlightColor);
   glRasterPos3f(-28.0, 25.0, 0.0);
   if (!animateMode)
   {
	  writeBitmapString((void*)font, "DEVELOP MODE"); 
      manVectorIterator->writeData();
   }
   else writeBitmapString((void*)font, "ANIMATE MODE");
   glPopMatrix();

   // Place camera.
   gluLookAt(camera.getZoomDistance() * sin(camera.getViewDirection()*PI/180.0), 0.0, 
	         camera.getZoomDistance() * cos(camera.getViewDirection()*PI/180.0), 0.0, 
			 0.0, 0.0, 0.0, 1.0, 0.0); 
 
   // Local iterator to traverse manVector.
   vector<Man>::iterator localManVectorIterator;

   // Move man right 10 units because of data text on left of screen.
   glTranslatef(10.0, 0.0, 0.0);

   if (!animateMode) // Develop mode.
   {
      localManVectorIterator = manVector.begin();

      // Draw all the configurations in manVector.
      while(localManVectorIterator != manVector.end() )
	  {
         localManVectorIterator->draw();
	     localManVectorIterator++;
	  }
   }
   else // Animated mode - 
	    // use separate iterator so as to leave develop mode iterator unchanged.
   {
      manVectorAnimationIterator->draw();
   }

   // Other (fixed) objects in scene are drawn below starting here.

   // Black floor.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   for(float z=-25.0; z<100.0; z+=5.0)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(float x=-50.0; x<50.0; x+=5.0)
	  {
	     glVertex3f(x, -25.0, z);
	     glVertex3f(x, -25.0, z+5.0);
	  }
      glEnd();
   }

   // Green sphere.
   glColor3f(0.0, 1.0, 0.0);
   glTranslatef(0.0, -20.0, 10.0);
   glPushMatrix();
   glScalef(5.0, 5.0, 5.0);
   glutWireSphere(1.0, 10, 8);
   glPopMatrix();

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   if (animateMode)
   {
      manVectorAnimationIterator++;
	  if (manVectorAnimationIterator == manVector.end() ) 
		  manVectorAnimationIterator = manVector.begin();
      
	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
}

// Function to write configurations to file.
void outputConfigurations(void)
{
   // Local iterator to traverse manVector.
   vector<Man>::iterator localManVectorIterator;

   outFile.open("animateManDataOut.txt");
   localManVectorIterator = manVector.begin();
   while(localManVectorIterator != manVector.end() )
   {
      localManVectorIterator->outputData();
	  localManVectorIterator++;
   }
   outFile.close();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);  

   // Initialize global manVector with single configuration.
   manVector.push_back(Man() );
   
   // Initialize global iterators for manVector.
   manVectorIterator = manVector.begin();
   manVectorAnimationIterator = manVector.begin();

   // Initialize camera.
   camera = Camera();
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
   vector<Man>::iterator localManVectorIterator;
   switch(key) 
   {
      case 27:
         exit(0);
         break;
	  case 'a': // Toggle between develop and animate modes.
         if (animateMode == 0) 
		 {
	        manVectorAnimationIterator = manVector.begin();
			outputConfigurations(); // Write configurations data to file at end of develop mode.
			animateMode = 1;
			animate(1);
		 }	
		 else animateMode = 0;
         glutPostRedisplay();
		 break;
	  case 'r': // Rotate camera.
         camera.incrementViewDirection();
         glutPostRedisplay();
		 break;
	  case 'R': // Rotate camera.
         camera.decrementViewDirection();
         glutPostRedisplay();
		 break;
	  case 'z': // Zoom in.
	     camera.decrementZoomDistance();
         glutPostRedisplay();
		 break;
	  case 'Z': // Zoom out.
	     camera.incrementZoomDistance();
         glutPostRedisplay();
		 break;
	  case 'n': // Create new man configuration.
		 // Turn highlight off current configuration.
         manVectorIterator->setHighlight(0); 

		 localManVectorIterator = manVectorIterator;
		 manVectorIterator++;

		 // Insert copy of current configuration and highlight.
         manVectorIterator = 
		   manVector.insert(manVectorIterator, Man(*localManVectorIterator) );
         manVectorIterator->setHighlight(1);

		 glutPostRedisplay();
		 break;
      case ' ': // Select next body part.
		 manVectorIterator->incrementSelectedPart();

         glutPostRedisplay();
		 break;

	  // Tab - select next man configuration.
	  case 9:
		 // Turn highlight off current configuration.
		 manVectorIterator->setHighlight(0);

		 // Increment iterator - go to start if at end already.
		 manVectorIterator++;
         if (manVectorIterator == manVector.end() ) 
			 manVectorIterator = manVector.begin();

		 // Highlight current configuration.
		 manVectorIterator->setHighlight(1);

		 glutPostRedisplay();
		 break;

	  // Backspace - reset current man configuration,
	  case 8: 
		 if (manVectorIterator != manVector.begin() ) // Not first configuration. 
		 {
		    // Make copy of the previous configuration and highlight.
		    localManVectorIterator = manVectorIterator;
		    localManVectorIterator--;
            manVectorIterator = 
		    manVector.insert(manVectorIterator, Man(*localManVectorIterator) );
		    manVectorIterator->setHighlight(1);

    		// Delete current configuration.
		    manVectorIterator++;
		    manVectorIterator = manVector.erase(manVectorIterator);
		
		    // Return iterator.
		    manVectorIterator--;
		 }
		 else // First configuration
		 {
			// Delete the current configuration.
	        manVectorIterator = manVector.erase(manVectorIterator);
			
			// Create new configuration.
			manVector.insert(manVectorIterator, Man() );
		 }

		 glutPostRedisplay();
		 break;

	  // Delete - delete current man configuration.
	  case 127:
		 if (manVector.size() > 1) 
		 {
		    manVectorIterator = manVector.erase(manVectorIterator);
		    if (manVectorIterator != manVector.begin() ) 
				manVectorIterator--;

         // Highlight current configuration.
		 manVectorIterator->setHighlight(1);
		 }

		 glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_PAGE_DOWN) manVectorIterator->decrementPartAngle();
   if (key == GLUT_KEY_PAGE_UP) manVectorIterator->incrementPartAngle();
   if (key == GLUT_KEY_LEFT) manVectorIterator->decrementForwardMove();
   if (key == GLUT_KEY_RIGHT) manVectorIterator->incrementForwardMove();
   if (key == GLUT_KEY_DOWN)
   {
      if (!animateMode) manVectorIterator->decrementUpMove();
	  else animationPeriod +=10;
   }
   if( key == GLUT_KEY_UP)
   {
      if (!animateMode) manVectorIterator->incrementUpMove();
	  else if (animationPeriod > 10) animationPeriod -=10;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press a to toggle between develop and animate modes." << endl
	    << endl
        << "In develop mode:" << endl
	    << "Press the space bar to select a part." << endl
        << "Press the page up/page down keys to rotate the selected part." << endl
        << "Press the left/right/up/down arrow keys to move the whole configuration." << endl
		<< "Press r/R to rotate the viewpoint." << endl
		<< "Press z/Z to zoom in/out." << endl
		<< "Press n to create a new configuration - other configurations are ghosted" << endl
		<< "(the new configuration is a copy of the current one so it must be moved to be visible)." << endl
		<< "Press tab to choose a configuration - it is highlighted, others ghosted." << endl
		<< "Press backspace to reset current configuration." << endl
		<< "Press delete to delete current configuration." << endl
		<<  endl
		<< "In animate mode:" << endl
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
   glutCreateWindow ("animateMan1.cpp"); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop(); 
}

