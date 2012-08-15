//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Animation3D.cpp
//	Purpose: OpenGL and GLUT program to draw a simple 3D animation with 
//          collision detection. The scene contains moving spheres inside 
//          a fixed, cube enclosure. A simple scene graph is used to define the 
//          scene. Transform nodes, geometry nodes, and presentation nodes
//          are used.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>
#include <GL/glut.h>

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("AnimatedBalls.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

 // glPushMatrix();
    
 // glPopMatrix();

  glutSwapBuffers();
}

/**
 * Main 
 */
/*int main(int argc, char** argv)
{
   //Seed the random number generator
   srand(929233);

   // The stuff below sets the default projection and view as well as a simple
   // lighting environment. (DO NOT CHANGE)
 
   // Set a fixed perspective projection
   glMatrixMode(GL_PROJECTION);
   gluPerspective(70.0f, 1.0f, 1.0f, 200.0f);
   glMatrixMode(GL_MODELVIEW);

   // Set a fixed camera outside the center of the front wall (imagine
   // it being a window) looking parallel to the floor
   gluLookAt(0.0f, -90.0f, 50.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f);

   // Set up a simple lighting environment
   glEnable(GL_LIGHTING);

   // Set white light. Somewhat dim for positioning
   // in the back corner
   GLfloat diffuse0[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse0);
   glEnable(GL_LIGHT0);

   // Load light 0 position - basically a modeling transformation
   // to position the light. Positional light just above the camera
   GLfloat position0[] = { 0.0f, -90.0f, 30.f, 1.0f };
   glLightfv(GL_LIGHT0, GL_POSITION, position0);

   // Enable normalization of vectors (so we can do scaling!)
   glEnable(GL_NORMALIZE);

   glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 480);
   glutInit(&argc, argv);

   //glutMainLoop();

   return 0;
}*/