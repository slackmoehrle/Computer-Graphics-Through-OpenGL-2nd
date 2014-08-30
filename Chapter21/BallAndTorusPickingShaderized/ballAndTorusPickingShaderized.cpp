///////////////////////////////////////////////////////////////////////////          
// ballAndTorusPickingShaderized.cpp
//
// Forward-compatible core GL 4.3 version of ballAndTorusPicking.cpp.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
// Click left mouse button to pick either ball or torus.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "hemisphere.h"
#include "torus.h"

using namespace std;
using namespace glm;

#define HIGHLIGHT_COLORS 1.0, 0.0, 0.0, 1.0 // Colors to highlight picked object.

static enum object {HEMISPHERE, TORUS}; // VAO ids.
static enum buffer {HEM_VERTICES, HEM_INDICES, TOR_VERTICES, TOR_INDICES, SHADER_STORAGE}; // VBO ids.

// Globals.
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

// Hemisphere data.
static Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)]; 
static unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)]; 
static int hemCounts[HEM_LATS];
static void* hemOffsets[HEM_LATS]; 
static vec4 hemColors = vec4(HEM_COLORS); 

// Torus data.
static Vertex torVertices[(TOR_LONGS + 1) * (TOR_LATS + 1)]; 
static unsigned int torIndices[TOR_LATS][2*(TOR_LONGS+1)]; 
static int torCounts[TOR_LATS];
static void* torOffsets[TOR_LATS]; 
static vec4 torColors = vec4(TOR_COLORS);

static vec4 highlightColors = vec4(HIGHLIGHT_COLORS);

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   objectLoc,
   isSelectingLoc,
   highlightFramesLoc,
   hemColorLoc,
   torColorLoc,
   highlightColorLoc,
   buffer[4], 
   vao[2]; 

static int clickX, clickY; // Mouse click coordinates.
static int highlightFrames = 0; // Number of frames to keep highlight.
static int isSelecting = 0; // In selection mode?
static int* storageBufferPtrInt; // int type pointer to buffer data.
static float* storageBufferPtrFloat; // float type pointer to buffer data.

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId);    
   glLinkProgram(programId); 
   glUseProgram(programId); 

   // Initialize hemishpere and torus.
   fillHemisphere(hemVertices, hemIndices, hemCounts, hemOffsets);
   fillTorus(torVertices, torIndices, torCounts, torOffsets);

   // Create VAOs, VBOs and shader storage buffer object... 
   glGenVertexArrays(2, vao);
   glGenBuffers(5, buffer); 

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[HEMISPHERE]);  
   glBindBuffer(GL_ARRAY_BUFFER, buffer[HEM_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(hemVertices), hemVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[HEM_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hemIndices), hemIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), 0);
   glEnableVertexAttribArray(0);

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[TORUS]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[TOR_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(torVertices), torVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[TOR_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(torIndices), torIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(torVertices[0]), 0);
   glEnableVertexAttribArray(1);

   // ...and bind shader storage buffer object to buffer block in fragment shader.
   glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer[SHADER_STORAGE]);
   glBufferData(GL_SHADER_STORAGE_BUFFER, 128, NULL, GL_DYNAMIC_COPY);
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer[SHADER_STORAGE]);

   // Return an int type pointer to the shader storage buffer data...
   storageBufferPtrInt = (int*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
   
   // ...and also cast it as a float type pointer 
   // (to access float data in the fragment shader buffer block).
   storageBufferPtrFloat = (float*)&storageBufferPtrInt[0];

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   // Obtain color uniform locations and set values.
   hemColorLoc = glGetUniformLocation(programId, "hemColor");
   glUniform4fv(hemColorLoc, 1, &hemColors[0]);
   torColorLoc = glGetUniformLocation(programId, "torColor");
   glUniform4fv(torColorLoc, 1, &torColors[0]);
   highlightColorLoc = glGetUniformLocation(programId, "highlightColor");
   glUniform4fv(highlightColorLoc, 1, &highlightColors[0]);

   // Obtain modelview matrix uniform and object uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   objectLoc = glGetUniformLocation(programId, "object");
   
   // Obtain isSelecting uniform location and set value.
   isSelectingLoc = glGetUniformLocation(programId, "isSelecting");
   glUniform1i(isSelectingLoc, isSelecting);

   // Obtain highlightFrames uniform location and set value.
   highlightFramesLoc = glGetUniformLocation(programId, "highlightFrames");
   glUniform1i(highlightFramesLoc, highlightFrames);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   // Don't react unless left button is pressed.
   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
   
   // Set the fields of the fragment shader buffer block:
   // clickedCoords.x and y from the mouse, clickedObj to NONE, clickedDepth to max depth. 
   storageBufferPtrInt[0] = x; 
   storageBufferPtrInt[1] = y;
   storageBufferPtrInt[2] = 2;
   storageBufferPtrFloat[3] = 1.0;
  
   // Enter object selection mode.
   isSelecting = 1;
   glUniform1i(isSelectingLoc, isSelecting);

   // Run the drawing routine in selection mode.
   glutPostRedisplay(); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -25.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); 

   // Draw torus.
   glUniform1ui(objectLoc, TORUS); // Update object name.
   glBindVertexArray(vao[TORUS]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, torCounts, GL_UNSIGNED_INT, (const void **)torOffsets, TOR_LATS);

   // Calculate and update modelview matrix.
   modelViewMat = rotate(modelViewMat, longAngle, vec3(0.0, 0.0, 1.0));
   modelViewMat = translate(modelViewMat, vec3(12.0, 0.0, 0.0));
   modelViewMat = rotate(modelViewMat, latAngle, vec3(0.0, 1.0, 0.0));
   modelViewMat = translate(modelViewMat, vec3(-12.0, 0.0, 0.0));
   modelViewMat = translate(modelViewMat, vec3(20.0, 0.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); 

   // Draw ball as two hemispheres.
   glUniform1ui(objectLoc, HEMISPHERE); // Update object name.
   glBindVertexArray(vao[HEMISPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);
   modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0)); // Scale to make inverted hemisphere.
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); // Update modelview matrix.
   glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);

   // If drawing was done in selection mode...
   if (isSelecting == 1)
   {   
      // ... set number of frames to highlight picked object.
	  highlightFrames = 10;
      glUniform1i(highlightFramesLoc, highlightFrames);  

      // ...restore non-selection mode.
      isSelecting = 0;
	  glUniform1i(isSelectingLoc, isSelecting);
   }

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      latAngle += 5.0;
	  if (latAngle > 360.0) latAngle -= 360.0;
      longAngle += 1.0;
	  if (longAngle > 360.0) longAngle -= 360.0;
   }
	  
   if (highlightFrames > 0) highlightFrames--;
   glUniform1i(highlightFramesLoc, highlightFrames);
   
   glutPostRedisplay();
   glutTimerFunc(animationPeriod, animate, 1);
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
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl << endl
        << "Click left mouse button to pick either ball or torus." << endl;
}


// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("ballAndTorusPickingShaderized.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutTimerFunc(5, animate, 1);
   glutMouseFunc(mouseControl); 

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}

