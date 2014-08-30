//////////////////////////////////////////////////////////////////////////////////////////////
// ballAndTorusShadowMapped.cpp
//
// This program applies shadow mapping to a scene with a single local light source (except for
// the light source the scene is identical to that of ballAndTorusShadowed.cpp).
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
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

using namespace std;

// Begin globals. 
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

static int winWth, winHgt; // OpenGL window sizes.

static unsigned int shadowMapTex; // Shadow map texture.
static int shadowMapTexWth = 512; // Shadow map texture width.
static int shadowMapTexHgt = 512; // Shadow map texture height.

// Light properties.
static float lightFovy  = 120.0; // gluPerspective from light: field of view angle.
static float lightAspect  = 1.0; // gluPerspective from light: aspect ratio.
static float lightNearPlane  = 1.0; // gluPerspective from light: near plane.
static float lightFarPlane  = 35.0; // gluPerspective from light: far plane.
static float lightPos[] = { 0.0, 30.0, 0.0 }; // Light position.
static float lightLookAt[]  = { 0.0, 0.0, 0.0 }; // gluLookAt from light: point looked at (i.e., center).
static float lightUp[]  = { 1.0, 0.0, 0.0 }; // gluLookAt from light: up direction.
static float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 }; // Light ambient values.
static float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 }; // Light diffuse and specular values.
static float globAmb[] = { 0.3, 0.3, 0.3, 1.0 }; // Global ambient values.

// Camera properties.
static float cameraFovy  = 90.0; // gluPerspective from camera: field of view angle.
static float cameraNearPlane  = 5.0; // gluPerspective from camera: near plane.
static float cameraFarPlane  = 100.0; // gluPerspective from camera: far plane.
static float cameraPos[]  = { 0.0, 36.0, 7.0 }; // Camera position.
static float cameraLookAt[]  = { 0.0, 0.0, 0.0 }; // gluLookAt from camera: point looked at (i.e., center).
static float cameraUp[]  = { 0.0, 1.0, 0.0 }; // gluLookAt from camear: up direction.

// Material properties.
float matSpec[] = { 1.0, 1.0, 1.0, 1.0 }; // Material specular values.
float matShine[] = { 50.0 }; // Material shininess.

// Matrices (4x4 matrices each written in a 1-dim array in column-major order).
static float lightProjMat[16]; // Light's projection transformation matrix.
static float lightViewMat[16]; // Light's viewing transformation matrix.  
static float cameraProjMat[16]; // Camera's projection transformation matrix.  
static float cameraViewMat[16]; // Camera's viewing transformation matrix.  
static float texMat[16]; // Texture matrix.
//End globals.

// Draw ball flying around torus.
void drawFlyingBallAndTorus(void)
{
   glShadeModel(GL_SMOOTH); 

   glPushMatrix();

   glTranslatef(0.0, 10.0, 0.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);

   // Fixed torus.
   glColor3f(0.0, 1.0, 0.0);
   glutSolidTorus(2.0, 12.0, 80, 80);

   // Begin revolving ball.
   glRotatef(longAngle, 0.0, 0.0, 1.0);
   
   glTranslatef(12.0, 0.0, 0.0);
   glRotatef(latAngle, 0.0, 1.0, 0.0);
   glTranslatef(-12.0, 0.0, 0.0);
   
   glTranslatef(20.0, 0.0, 0.0);

   glColor3f(0.0, 0.0, 1.0);
   glutSolidSphere(2.0, 20, 20);
   // End revolving ball.

   glPopMatrix();
}
 
// Draw checkered floor.
void drawCheckeredFloor(void)
{  
   glShadeModel(GL_FLAT); // Flat shading to get the checkered pattern.
   int i = 0;

   glPushMatrix();

   for(float z = 50.0; z > -50.0; z -= 5.0)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(float x = -50.0; x < 50.0; x += 5.0)
	  {
         if (i % 2) glColor3f(0.0, 0.5, 0.5);  
         else glColor3f(1.0, 1.0, 1.0); 
         glNormal3f(0.0, 1.0, 0.0);
		 glVertex3f(x, 0.0, z - 5.0);
	     glVertex3f(x, 0.0, z);
		 i++;
	  }
      glEnd();
	  i++;
   }

   glPopMatrix();
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

	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
}

// Initialization routine.
void setup(void)
{    
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Need to set the depth test to <=, rather than the default <, so that the brightly lit
   // regions in the third drawing pass can overwrite their dark versions from the second.
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);

   //Create the shadow map (depth) texture.
   glGenTextures(1, &shadowMapTex);
   glBindTexture(GL_TEXTURE_2D, shadowMapTex);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapTexWth, shadowMapTexHgt, 0,
			    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

   // Local light source properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   
   // Global ambient light.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); 

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

   // Enable texturing.
   glEnable(GL_TEXTURE_2D);
   
   // Enable face culling.
   glEnable(GL_CULL_FACE);

   // Compute and save matrices.
   // All computations, even projection matrix, are done in the modelview matrix stack.
   glPushMatrix();
	
   // Camera's projection transformation matrix.  
   glLoadIdentity();
   gluPerspective(cameraFovy, (float)winWth/winHgt, cameraNearPlane, cameraFarPlane);
   glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjMat);
	
   // Camera's viewing transformation matrix.  
   glLoadIdentity();
   gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], cameraLookAt[0], cameraLookAt[1], cameraLookAt[2],
			 cameraUp[0], cameraUp[1], cameraUp[2]);
   glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMat);
	
   // Light's projection transformation matrix.  
   glLoadIdentity();
   gluPerspective(lightFovy, lightAspect, lightNearPlane, lightFarPlane);
   glGetFloatv(GL_MODELVIEW_MATRIX, lightProjMat);
	
   // Light's viewing transformation matrix.  
   glLoadIdentity();
   gluLookAt(lightPos[0], lightPos[1], lightPos[2], lightLookAt[0], lightLookAt[1], lightLookAt[2],
		     lightUp[0], lightUp[1], lightUp[2]);
   glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMat);
	
   glPopMatrix();
}

// Drawing routine.
void drawScene(void)
{
   // FIRST PASS: Draw the scene from the light's viewpoint
   // and capture the z-buffer in the shadow map texture.
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

   // Load the light's projection transformation matrix.
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(lightProjMat);

   // Load the light's viewing transformatin matrix.
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(lightViewMat);

   // Create viewport same size as the shadow map texture.
   glViewport(0, 0, shadowMapTexWth, shadowMapTexHgt);

   // Disable writes to the color buffer so that the scene is not rendered to the window.
   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
   // Cull front faces so that only depths of back faces are written to the z-buffer
   // (so that front faces, which should be visible, win the depth competition in their corresponding pixels).
   glCullFace(GL_FRONT);

   // Draw the scene.
   drawCheckeredFloor();
   drawFlyingBallAndTorus();

   // Capture the depth buffer in the shadow map texture.
   glBindTexture(GL_TEXTURE_2D, shadowMapTex);
   glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, shadowMapTexWth, shadowMapTexHgt, 0 );

   // SECOND PASS: Draw the scene from the camera's viewpoint using only
   // global ambient (dim) light, so the whole scene is drawn shadowed.

   glClear(GL_DEPTH_BUFFER_BIT);

   // Load the camera's projection transformation matrix.
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(cameraProjMat);
	
   // Load the camera's viewing transformation matrix.
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(cameraViewMat);

   // Create viewport same size as the OpenGL window.
   glViewport(0, 0, winWth, winHgt);

   // Cull back faces (not visible to the camera).
   glCullFace(GL_BACK);

   // Allow the scene to be rendered.
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

   // Enable lighting. This enables ONLY global ambient light as LIGHT0 is NOT enabled.
   glEnable(GL_LIGHTING);

   // Draw the scene.
   drawCheckeredFloor();
   drawFlyingBallAndTorus();
   
   // THIRD PASS: Draw the scene from the camera's viewpoint, 
   // but draw only non-shadowed parts and use bright light.
   
   // LIGHT0 throws a bright light.
   glEnable(GL_LIGHT0);
   
   // Compute and save the texture matrix.
   // Computations are done in the modelview matrix stack.
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   // These two transformations generate the bias matrix B.
   glTranslatef(0.5, 0.5, 0.5);
   glScalef(0.5, 0.5, 0.5);
   
   glMultMatrixf(lightProjMat); // B x P_light
   glMultMatrixf(lightViewMat); // B x P_light x V_light
   
   // Save the current modelview matrix.
   glGetFloatv(GL_MODELVIEW_MATRIX, texMat);  
   
   glPopMatrix();

   // List the rows of the texture matrix noting that it is stored in texMat 
   // in column-major order.
   float texMatRow0[] = { texMat[0], texMat[4], texMat[8], texMat[12] };
   float texMatRow1[] = { texMat[1], texMat[5], texMat[9], texMat[13] };
   float texMatRow2[] = { texMat[2], texMat[6], texMat[10], texMat[14] };
   float texMatRow3[] = { texMat[3], texMat[7], texMat[11], texMat[15] };

   // Generate texture coordinates equal to 
   // B x P_light x V_light x (V_camera)^{-1) x vertex
   glEnable(GL_TEXTURE_GEN_S);
   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_S, GL_EYE_PLANE, texMatRow0);

   glEnable(GL_TEXTURE_GEN_T);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_T, GL_EYE_PLANE, texMatRow1);

   glEnable(GL_TEXTURE_GEN_R);
   glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_R, GL_EYE_PLANE, texMatRow2);

   glEnable(GL_TEXTURE_GEN_Q);
   glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_Q, GL_EYE_PLANE, texMatRow3);

   // Activate the shadow map texture.
   glBindTexture(GL_TEXTURE_2D, shadowMapTex);

   // Compare the r-coordinate to the corresponding value stored in the shadow map texture.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

   // Comparison should be true if r <= texture, i.e., if the fragment is not shadowed.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

   // Set shadow comparison to generate an intensity value 
   // (0 for failed comparisons, 1 for successful).
   glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

   // Set alpha test to discard failed comparisons, i.e., shadowed fragments.
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, 0.5);

   // Draw the scene.
   drawCheckeredFloor();
   drawFlyingBallAndTorus();
   
   // Restore states.
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_Q);
   glDisable(GL_LIGHTING);
   glDisable(GL_LIGHT0);
   glDisable(GL_ALPHA_TEST);
	
   // FOURTH PASS: Draw a red sphere at the light's position. 
   glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
   glColor3f(1.0, 0.0, 0.0);
   glutWireSphere(0.2, 10, 10);

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   // Update the new window dimensions.
   winWth = w; 
   winHgt=h;

   // Update the camera's projection matrix by recomputing it (using the modelview matrix stack).
   glPushMatrix();
   glLoadIdentity();
   gluPerspective(cameraFovy, (float)winWth/winHgt, cameraNearPlane, cameraFarPlane);
   glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjMat);
   glPopMatrix();
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
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
		 }
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
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   
   glutInitContextVersion(4, 3); 
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("ballAndTorusShadowMapped.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
 
   glewExperimental = GL_TRUE; 
   glewInit(); 
   
   setup(); 

   glutMainLoop();
}
