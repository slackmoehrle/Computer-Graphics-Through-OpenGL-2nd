///////////////////////////////////////////////////////////////////////////
// tessellatedHemisphere.cpp
//
// Forward-compatible core GL 4.3 program drawing a tessellated hemisphere.
//
// Interaction:
// Press `x'-`Z' to rotate the hemisphere.
// Press the up/down arrow keys to increase/decrease inner tess levels.
// Press the right/left arrow keys to increase/decrease outer tess levels.
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
#include "vertex.h"

using namespace std;
using namespace glm;

#define HEM_RADIUS 10.0 // Hemisphere radius.

static enum object {HEMISPHERE};          
static enum buffer {HEMISPHERE_VERTICES};

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.

// Hemisphere data.
static Vertex hemisphereDummyVertex[1] = { { vec4(0.0, 0.0, 0.0, 1.0)} };
static float hemRadius = HEM_RADIUS; 
static float tessLevelOuter[4] = {5.0, 5.0, 5.0, 5.0};
static float tessLevelInner[2] = {5.0, 4.0};

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

static unsigned int
   programId,
   vertexShaderId,
   tessEvaluationShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   hemRadiusLoc,
   buffer[1], 
   vao[1];

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   tessEvaluationShaderId = setShader("tessEvaluation", "tessEvaluationShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, tessEvaluationShaderId);
   glAttachShader(programId, fragmentShaderId);
   glLinkProgram(programId); 
   glUseProgram(programId); 

   // Create VAOs and VBOs... 
   glGenVertexArrays(1, vao);
   glGenBuffers(1, buffer); 

   // ...and fill data.
   glBindVertexArray(vao[HEMISPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[HEMISPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(hemisphereDummyVertex), hemisphereDummyVertex, GL_STATIC_DRAW);

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   // Obtain modelview matrix and hemRadius uniform location.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   hemRadiusLoc = glGetUniformLocation(programId, "hemRadius");
   glUniform1f(hemRadiusLoc, hemRadius); // Update object name.

   // Polygon mode.
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // Input patch size.
   glPatchParameteri(GL_PATCH_VERTICES, 1);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Set tessellation levels.
   glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, tessLevelOuter);
   glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, tessLevelInner);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -25.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   // Draw hemisphere.
   glBindVertexArray(vao[HEMISPHERE]);
   glDrawArrays(GL_PATCHES, 0, 1);
 
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
   if (key == GLUT_KEY_UP) tessLevelInner[1] = ++tessLevelInner[0];
   if (key == GLUT_KEY_DOWN) if (tessLevelInner[0] > 1.0) tessLevelInner[1] = --tessLevelInner[0];
   if (key == GLUT_KEY_RIGHT) tessLevelOuter[3] = tessLevelOuter[2] = tessLevelOuter[1] = ++tessLevelOuter[0];
   if (key == GLUT_KEY_LEFT) if (tessLevelOuter[0] > 1.0) tessLevelOuter[3] = tessLevelOuter[2] = tessLevelOuter[1] = --tessLevelOuter[0];

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press `x'-`Z' to rotate the hemisphere." << endl
	    << "Press the up/down arrow keys to increase/decrease inner tess levels." << endl
        << "Press the right/left arrow keys to increase/decrease outer tess levels." << endl;
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
   glutCreateWindow("tessellatedHemisphere.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}

