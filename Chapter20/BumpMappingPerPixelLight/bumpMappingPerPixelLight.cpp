/////////////////////////////////////////////////////////////////          
// bumpMappingPerPixelLight.cpp
//
// Forward-compatible core GL 4.3 modification of bumpMapping.cpp
// from per-vertex lighting to per-pixel lighting.
//
// This program and all associated source and header files 
// are exactly the same as in bumpMappingShaderized.cpp
// which implements per-vertex lighting. Only the associated 
// shaders are different.
//
// Interaction:
// Press space to toggle between bump mapping on and off.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////// 

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
#include "plane.h"
#include "light.h"
#include "material.h"

using namespace std;
using namespace glm;

static enum object {PLANE}; // VAO ids.
static enum buffer {PLANE_VERTICES, PLANE_INDICES}; // VBO ids.

// Globals.
// Light properties.
static const Light light0 = 
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 1.0, 0.0, 0.0 )
};

// Global ambient.
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

// Front and back material properties.
static const Material planeFandB = 
{
	vec4(0.0, 0.5, 0.5, 1.0),
	vec4(0.0, 0.5, 0.5, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0
};

// Plane data.
static Vertex planeVertices[(PLANE_LONGS + 1) * (PLANE_LATS + 1)]; 
static unsigned int planeIndices[PLANE_LATS][2*(PLANE_LONGS+1)]; 
static int planeCounts[PLANE_LATS]; 
static void* planeOffsets[PLANE_LATS]; 

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   normalMatLoc,
   isBumpMappedLoc,
   buffer[2], 
   vao[1]; 

static uint isBumpMapped = 0; // Is bump mapping on?

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

   // Initialize plane.
   fillPlane(planeVertices, planeIndices, planeCounts, planeOffsets);

   // Create VAOs and VBOs...
   glGenVertexArrays(1, vao);
   glGenBuffers(2, buffer); 

   // ,,,and associate data with vertex shader.
   glBindVertexArray(vao[PLANE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[PLANE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[PLANE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(planeVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(planeVertices[0]), (void*)sizeof(planeVertices[0].coords));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(planeVertices[0]), (void*)(sizeof(planeVertices[0].coords)+sizeof(planeVertices[0].normal)));
   glEnableVertexAttribArray(2);

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   // Obtain modelview matrix and normal matrix uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   normalMatLoc = glGetUniformLocation(programId,"normalMat"); 
   
   // Obtain isBumpMapped uniform location and set value.
   isBumpMappedLoc = glGetUniformLocation(programId, "isBumpMapped");
   glUniform1uiv(isBumpMappedLoc, 1, &isBumpMapped);

   // Obtain light property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

   // Obtain global ambient uniform location and set value.
   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   // Obtain material property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "planeFandB.ambRefl"), 1, &planeFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "planeFandB.difRefl"), 1, &planeFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "planeFandB.specRefl"), 1, &planeFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "planeFandB.emitCols"), 1, &planeFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "planeFandB.shininess"), planeFandB.shininess);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(vec3(0.0, 5.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   // Calculate and update normal matrix.
   normalMat = transpose(inverse(mat3(modelViewMat)));
   glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));
   
   // Draw plane.
   glMultiDrawElements(GL_TRIANGLE_STRIP, planeCounts, GL_UNSIGNED_INT, (const void **)planeOffsets, PLANE_LATS);
  
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
		 isBumpMapped = isBumpMapped? 0 : 1;
		 glUniform1uiv(isBumpMappedLoc, 1, &isBumpMapped);
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
   cout << "Press space to toggle between bump mapping on and off." << endl;
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
   glutCreateWindow("bumpMappingPerPixelLight.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}