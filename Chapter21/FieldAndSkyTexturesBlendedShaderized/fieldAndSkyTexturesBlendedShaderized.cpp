////////////////////////////////////////////////////////////////////////////          
// fieldAndSkyTexturesBlendedShaderized.cpp
//
// Forward-compatible core GL 4.3 version of fieldAndSkyTexturesBlended.cpp.
//
// Interaction:
// Press the arrow keys to change the direction of the sun.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
//////////////////////////////////////////////////////////////////////////// 

#include <cmath>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
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
#include "light.h"
#include "material.h"
#include "getbmp.h"
#include "vertex.h"

#define PI 3.14159265358979324

using namespace std;
using namespace glm;

static enum object {FIELD, SKY, LINE}; /// VAO ids.
static enum buffer {FIELD_VERTICES, SKY_VERTICES, LINE_VERTICES}; // VBO ids.

// Globals.
// Light properties.
static Light light0 = 
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 0.0, 0.0, 0.0 )
};

// Global ambient.
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

// Front and back material properties.
static const Material matFandB = 
{
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static Vertex fieldVertices[] =  
{
	{vec4(100.0, 0.0, 100.0, 1.0), vec3(0.0, 1.0, 0.0), vec2(8.0, 0.0)},
	{vec4(100.0, 0.0, -100.0, 1.0), vec3(0.0, 1.0, 0.0), vec2(8.0, 8.0)},
	{vec4(-100.0, 0.0, 100.0, 1.0), vec3(0.0, 1.0, 0.0), vec2(0.0, 0.0)},
	{vec4(-100.0, 0.0, -100.0, 1.0), vec3(0.0, 1.0, 0.0), vec2(0.0, 8.0)}
}; 

static Vertex skyVertices[4] =  
{
	{vec4(100.0, 0.0, -70.0, 1.0), vec3(0.0, 0.0, 1.0), vec2(1.0, 0.0)},
	{vec4(100.0, 120.0, -70.0, 1.0), vec3(0.0, 0.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-100.0, 0.0, -70.0, 1.0), vec3(0.0, 0.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-100.0, 120.0, -70.0, 1.0), vec3(0.0, 0.0, 1.0), vec2(0.0, 1.0)}
}; 

static Vertex lineVertices[2] =  
{
	{vec4(0.0, 0.0, -6.0, 1.0), vec3(1.0, 0.0, 0.0), vec2(1.0, 0.0)},
	{vec4(1.0, 0.0, -6.0, 1.0), vec3(1.0, 0.0, 0.0), vec2(1.0, 0.0)}
}; 

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
   grassTexLoc,
   skyTexLoc,
   nightSkyTexLoc,  
   objectLoc,
   light0coordsLoc,
   alphaLoc,
   buffer[3], 
   vao[3],
   texture[3]; 

static BitMapFile *image[3];

static float theta = 0.0; // Angle of the sun with the ground.
static float alpha = 0.0; // Blending parameter.

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

   // Create VAOs and VBOs... 
   glGenVertexArrays(3, vao);
   glGenBuffers(3, buffer); 

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[FIELD]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[FIELD_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)sizeof(fieldVertices[0].coords));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)+sizeof(fieldVertices[0].normal)));
   glEnableVertexAttribArray(2);

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[SKY]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), 0);
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)(sizeof(skyVertices[0].coords)+sizeof(skyVertices[0].normal)));
   glEnableVertexAttribArray(4);

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[LINE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[LINE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_DYNAMIC_DRAW);
   glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(lineVertices[0]), 0);
   glEnableVertexAttribArray(5);

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   
   // Obtain modelview matrix, normal matrix, object and alpha uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   normalMatLoc = glGetUniformLocation(programId,"normalMat"); 
   objectLoc = glGetUniformLocation(programId, "object");
   alphaLoc = glGetUniformLocation(programId, "alpha");

   // Obtain light property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
   light0coordsLoc = glGetUniformLocation(programId, "light0.coords");

   // Obtain global ambient uniform location and set value.
   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   // Obtain front material property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "matFandB.ambRefl"), 1, &matFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "matFandB.difRefl"), 1, &matFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "matFandB.specRefl"), 1, &matFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "matFandB.emitCols"), 1, &matFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "matFandB.shininess"), matFandB.shininess);

   // Load the images.
   image[0] = getbmp("../../Textures/grass.bmp"); 
   image[1] = getbmp("../../Textures/sky.bmp"); 
   image[2] = getbmp("../../Textures/nightSky.bmp"); 

   // Create texture ids.
   glGenTextures(3, texture);

   // Bind grass image.
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   grassTexLoc = glGetUniformLocation(programId, "grassTex");
   glUniform1i(grassTexLoc, 0);

   // Bind sky image.
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   skyTexLoc = glGetUniformLocation(programId, "skyTex");
   glUniform1i(skyTexLoc, 1);

   // Bind night sky image.
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   nightSkyTexLoc = glGetUniformLocation(programId, "nightSkyTex");
   glUniform1i(nightSkyTexLoc, 2);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calculate and update light (sun) position.
   light0.coords.x = cos( (PI/180.0)*theta );
   light0.coords.y = sin( (PI/180.0)*theta );
   glUniform4fv(light0coordsLoc, 1, &light0.coords[0]);
   
   // Calculate and update blending parameter.
   if (theta <= 90.0) alpha = theta/90.0; 
   else alpha = (180.0 - theta)/90.0;    
   glUniform1f(alphaLoc, alpha);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   
   // Calculate and update normal matrix.
   normalMat = transpose(inverse(mat3(modelViewMat)));
   glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));

   // Draw field.
   glUniform1ui(objectLoc, FIELD);
   glBindVertexArray(vao[FIELD]);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   // Draw sky.
   glUniform1ui(objectLoc, SKY);
   glBindVertexArray(vao[SKY]);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   // Draw line.
   glUniform1ui(objectLoc, LINE);
   glBindVertexArray(vao[LINE]);
   modelViewMat = mat4(1.0);
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   lineVertices[1].coords = vec4(cos( (PI/180.0)*theta ), sin( (PI/180.0)*theta ), -6.0, 1.0);
   glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_DYNAMIC_DRAW);
   glDrawArrays(GL_LINES, 0, 2);

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
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT) 
   {
      if (theta > 0.0) theta -= 1.0;
   }
   if (key == GLUT_KEY_LEFT) 
   {
      if (theta < 180.0) theta += 1.0;
   }
   if (key == GLUT_KEY_UP) 
   {
      if (theta < 90.0) theta += 1.0;
      if (theta > 90.0) theta -= 1.0;
   }
   if (key == GLUT_KEY_DOWN) 
   {
      if ( (theta <= 90.0) && (theta > 0.0) ) theta -= 1.0;
      if ( (theta > 90.0) && (theta < 180.0) )theta += 1.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the arrow keys to change the direction of the sun." << endl;
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
   glutCreateWindow("fieldAndSkyTexturesBlendedShaderized.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}

