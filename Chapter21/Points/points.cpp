///////////////////////////////////////////////////////////////////////          
// points.cpp
//
// Forward-compatible core GL 4.3 program drawing three points
// showing various ways to render a point.
// 
// Interaction:
// Press space to cycle through four different point renderings.
// Press up/down arrow keys to move points in the z-direction.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
///////////////////////////////////////////////////////////////////////

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
#include "getbmp.h"
#include "vertex.h"

using namespace std;
using namespace glm;

static enum buffer {POINTSET_VERTICES}; // VAO ids.
static enum object {POINTSET}; // VBO ids.

// Globals
static Vertex pointSetVertices[] =  
{
   {vec4(-4.0, -4.0, -10.0, 1.0)},
   {vec4(4.0, -4.0, -10.0, 1.0)},
   {vec4(0.0, 4.0, -10.0, 1.0)}
};
static vec4 pointSetColor = vec4(1.0, 0.0, 0.0, 1.0);

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   pointSetColorLoc,
   starTexLoc, 
   renderChoiceLoc,
   zTransLoc,
   buffer[1],
   vao[1],
   texture[1]; 
   
static BitMapFile *image[1]; // Local storage for bmp image data.

static unsigned int renderChoice = 0; // What to render.
static float zTrans = 0.0; // Distance translated in z-direction.

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

   // Create VAOs and VBOs... 
   glGenVertexArrays(1, vao);
   glGenBuffers(1, buffer);

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[POINTSET]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[POINTSET_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(pointSetVertices), pointSetVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pointSetVertices[0]), 0);
   glEnableVertexAttribArray(0);

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   // Obtain point set color uniform location and set value.
   pointSetColorLoc = glGetUniformLocation(programId, "pointSetColor");
   glUniform4fv(pointSetColorLoc, 1, &pointSetColor[0]);

   // Obtain modelview matrix, z-translation and render choice uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   zTransLoc = glGetUniformLocation(programId, "zTrans");
   renderChoiceLoc = glGetUniformLocation(programId, "renderChoice");

   // Load the images.
   image[0] = getbmp("../../Textures/star.bmp"); 

   // Create texture ids.
   glGenTextures(1, texture);

   // Bind star image.
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   starTexLoc = glGetUniformLocation(programId, "starTex");
   glUniform1i(starTexLoc, 0);

   // Enable shader control of point size.
   glEnable(GL_PROGRAM_POINT_SIZE);

   // Set origin of the texture coordinate system on the point sprite.
   glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

   // Enable point sprites.
   glEnable(GL_POINT_SPRITE);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(vec3(0.0, 0.0, zTrans), vec3(0.0, 0.0, zTrans-1.0), vec3(0.0, 1.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); 

   // Render points.
   glUniform1ui(renderChoiceLoc, renderChoice); 
   glUniform1f(zTransLoc, zTrans); 
   glDrawArrays(GL_POINTS, 0, 3);
   
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
	  case ' ': 
	     {
			renderChoice = (renderChoice == 3)? 0 : ++renderChoice; 
			glutPostRedisplay();
		 }
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP)
   {
      zTrans += 0.05;
   }
   if( key == GLUT_KEY_DOWN) 
   {
      zTrans -= 0.05;
	  if (zTrans < -5.0) zTrans = -5.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to cycle through four different point renderings." << endl
	    << "Press up/down arrow keys to move points in the z-direction." << endl;
}

// Main routine.
int main(int argc, char* argv[])
{	
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
   
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("points.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
