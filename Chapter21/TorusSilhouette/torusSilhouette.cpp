/////////////////////////////////////////////////////////////////////          
// torusSilhouette.cpp
//
// Forward-compatible core GL 4.3 program drawing a torus with option
// to view only in silhouette with help of a geometry shader.
//
// Interaction:
// Press space to toggle between silhouetted rendering on and off.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////// 

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
#include "torus.h"

using namespace std;
using namespace glm;

static enum object {TORUS}; // VAO id values.
static enum buffer {TOR_VERTICES, TOR_INDICES}; // VAO id values.

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isSilhouette = 0; // Silhouetted?

// Torus data.
static Vertex torVertices[(TOR_LONGS + 1) * (TOR_LATS + 1)]; 
static unsigned int torIndices[TOR_LATS][4*(TOR_LONGS+1)]; 
static int torCounts[TOR_LATS]; 
static void* torOffsets[TOR_LATS]; 
static vec4 torColors = vec4(TOR_COLORS);

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   geometryShaderId,
   modelViewMatLoc,
   isSilhouetteLoc,
   torColorLoc,
   projMatLoc,
   buffer[2], 
   vao[1]; 

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   geometryShaderId = setShader("geometry", "geometryShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, geometryShaderId);
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 

   // Initialize torus.
   fillTorus(torVertices, torIndices, torCounts, torOffsets);

   // Create VAO and VBOs... 
   glGenVertexArrays(1, vao);
   glGenBuffers(2, buffer); 

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[TORUS]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[TOR_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(torVertices), torVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[TOR_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(torIndices), torIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(torVertices[0]), 0);
   glEnableVertexAttribArray(0);

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

  // Obtain color uniform location and set value.
   torColorLoc = glGetUniformLocation(programId, "torColor");
   glUniform4fv(torColorLoc, 1, &torColors[0]);

   // Obtain modelview matrix uniform and isSilhouette uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   isSilhouetteLoc = glGetUniformLocation(programId, "isSilhouette");
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
   glUniform1ui(isSilhouetteLoc, isSilhouette); // Update rendering mode.
   glBindVertexArray(vao[TORUS]);
   glMultiDrawElements(GL_TRIANGLE_STRIP_ADJACENCY, torCounts, GL_UNSIGNED_INT, (const void **)torOffsets, TOR_LATS);

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
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
		 isSilhouette = isSilhouette? 0 : 1;
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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between silhouetted rendering on and off." << endl
	    << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
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
   glutCreateWindow("torusSilhouette.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}

