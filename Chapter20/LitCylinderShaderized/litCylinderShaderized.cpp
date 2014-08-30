/////////////////////////////////////////////////////////////          
// litCylinderShaderized.cpp
//
// Forward-compatible core GL 4.3 version of litCylinder.cpp.
//
// Interaction:
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha
///////////////////////////////////////////////////////////// 

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
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"
#include "cylinder.h"
#include "light.h"
#include "material.h"

using namespace std;
using namespace glm;

static enum object {CYLINDER}; // VAO ids.
static enum buffer {CYL_VERTICES, CYL_INDICES}; // VBO ids.

// Globals.
static float Xangle = 150.0, Yangle = 60.0, Zangle = 0.0; // Angles to rotate the cylinder.

// Light properties.
static const Light light0 = 
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 1.5, 3.0, 0.0)
};

// Global ambient.
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

// Front material properties.
static const Material cylFront = 
{
	vec4(0.9, 0.0, 0.0, 1.0),
	vec4(0.9, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0
};

// Back material properties.
static const Material cylBack = 
{
	vec4(0.0, 0.9, 0.0, 1.0),
	vec4(0.0, 0.9, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0
};

// Cylinder data.
static Vertex cylVertices[(CYL_LONGS + 1) * (CYL_LATS + 1)]; 
static unsigned int cylIndices[CYL_LATS][2*(CYL_LONGS+1)]; 
static int cylCounts[CYL_LATS]; 
static void* cylOffsets[CYL_LATS]; 

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   normalMatLoc,
   projMatLoc,
   buffer[2], 
   vao[1],
   width, // OpenGL window width.
   height; // OpenGL window height.

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

   // Initialize cylinder.
   fillCylinder(cylVertices, cylIndices, cylCounts, cylOffsets);

   // Create VAOs and VBOs... 
   glGenVertexArrays(1, vao);
   glGenBuffers(2, buffer); 

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[CYLINDER]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[CYL_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cylVertices), cylVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[CYL_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylIndices), cylIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cylVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(cylVertices[0]), (void*)sizeof(cylVertices[0].coords));
   glEnableVertexAttribArray(1);

   // Obtain modelview matrix, projection matrix and normal matrix uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   normalMatLoc = glGetUniformLocation(programId,"normalMat"); 

   // Obtain light property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

   // Obtain global ambient uniform location and set value.
   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   // Obtain front material property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "cylFront.ambRefl"), 1, &cylFront.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "cylFront.difRefl"), 1, &cylFront.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "cylFront.specRefl"), 1, &cylFront.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "cylFront.emitCols"), 1, &cylFront.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "cylFront.shininess"), cylFront.shininess);

   // Obtain back material property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "cylBack.ambRefl"), 1, &cylBack.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "cylBack.difRefl"), 1, &cylBack.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "cylBack.specRefl"), 1, &cylBack.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "cylBack.emitCols"), 1, &cylBack.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "cylBack.shininess"), cylBack.shininess);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calculate and update projection matrix.
   projMat = perspective(60.0f, (float)width/(float)height, 1.0f, 50.0f); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(vec3(0.0, 0.0, 3.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   // Calculate and update normal matrix.
   normalMat = transpose(inverse(mat3(modelViewMat)));
   glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));

   // Draw cylinder.
   glMultiDrawElements(GL_TRIANGLE_STRIP, cylCounts, GL_UNSIGNED_INT, (const void **)cylOffsets, CYL_LATS);

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   width = w; height = h;
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
   cout << "Press x, X, y, Y, z, Z to turn the cylinder." << endl;
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
   glutCreateWindow("litCylinderShaderized.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}

