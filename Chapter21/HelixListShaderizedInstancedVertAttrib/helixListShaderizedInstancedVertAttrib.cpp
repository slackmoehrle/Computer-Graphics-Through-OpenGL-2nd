////////////////////////////////////////////////////////////////          
// helixListShaderizedInstancedVertAttrib.cpp
//
// Forward-compatible core GL 4.3 version of helixList.cpp using
// instanced vertex attributes.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////// 

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
#include "helix.h"

using namespace std;
using namespace glm;

static enum object {HELIX}; // VAO ids.
static enum buffer {HEL_VERTICES, HEL_COLORS, HEL_TRANSFORM_MATS}; // VBO ids.

// Globals.
// Helix data.
static Vertex helVertices[HEL_SEGS+1]; // Helix vertices.
static vec4 helColors[6] = // Colors for each of the 6 instances of the helix (following helixList.cpp).
{ 
   vec4(1.0, 0.0, 0.0, 1.0),
   vec4(0.0, 1.0, 0.0, 1.0),
   vec4(0.0, 0.0, 1.0, 1.0),
   vec4(1.0, 1.0, 0.0, 1.0),
   vec4(1.0, 0.0, 1.0, 1.0),
   vec4(0.0, 1.0, 1.0, 1.0)
};
static mat4 helTransformMats[6]; // Modelview matrices to transform each of the 6 instances of the helix.

static mat4 projMat = mat4(1.0);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   projMatLoc,
   buffer[3], 
   vao[1]; 

// Set the modelview matrices to transform helix instances (following helixList.cpp).
void fillHelTransformMats(void)
{
   helTransformMats[0] = mat4(1.0);
   helTransformMats[0] = translate(helTransformMats[0], vec3(0.0, 0.0, -70.0));

   helTransformMats[1] = mat4(1.0);
   helTransformMats[1] = translate(helTransformMats[1], vec3(30.0, 0.0, -70.0));
   helTransformMats[1] = scale(helTransformMats[1], vec3(0.5, 0.5, 0.5));

   helTransformMats[2] = mat4(1.0);
   helTransformMats[2] = translate(helTransformMats[2], vec3(-25.0, 0.0, -70.0));
   helTransformMats[2] = rotate(helTransformMats[2], 90.0f, vec3(0.0, 1.0, 0.0));

   helTransformMats[3] = mat4(1.0);
   helTransformMats[3] = translate(helTransformMats[3], vec3(0.0, -20.0, -70.0));
   helTransformMats[3] = rotate(helTransformMats[3], 90.0f, vec3(0.0, 0.0, 1.0));

   helTransformMats[4] = mat4(1.0);
   helTransformMats[4] = translate(helTransformMats[4], vec3(-40.0, 40.0, -70.0));
   helTransformMats[4] = scale(helTransformMats[4], vec3(0.5, 0.5, 0.5));

   helTransformMats[5] = mat4(1.0);
   helTransformMats[5] = translate(helTransformMats[5], vec3(30.0, 30.0, -70.0));
   helTransformMats[5] = rotate(helTransformMats[5], 90.0f, vec3(1.0, 0.0, 0.0));
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId);    
   glLinkProgram(programId); 
   glUseProgram(programId);

   // Initialize helix.
   fillHelix(helVertices);

   // Fill the array helTransformMats[6].
   fillHelTransformMats();

   // Create VAOs and VBOs... 
   glGenVertexArrays(1, vao);
   glGenBuffers(3, buffer); 

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[HELIX]);  
   glBindBuffer(GL_ARRAY_BUFFER, buffer[HEL_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(helVertices), helVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(helVertices[0]), 0);
   glEnableVertexAttribArray(0);

   // ...and associate data with vertex shader.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[HEL_COLORS]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(helColors), helColors, GL_STATIC_DRAW);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(helColors[0]), 0);
   glEnableVertexAttribArray(1);
   glVertexAttribDivisor(1, 1); // Set attribute instancing.

   // ...and associate data with vertex shader.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[HEL_TRANSFORM_MATS]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(helTransformMats), helTransformMats, GL_STATIC_DRAW);
   for (int i = 0; i < 4; i++)
   {
      glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(vec4) * i)); 
      glEnableVertexAttribArray(2 + i);
      glVertexAttribDivisor(2 + i, 1); // Set attribute instancing.
   }

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw helix instances.
   glBindVertexArray(vao[HELIX]);
   glDrawArraysInstanced(GL_LINE_STRIP, 0, HEL_SEGS, 6);

   glFlush();
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
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA ); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("helixListShaderizedInstancedVertAttrib.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}