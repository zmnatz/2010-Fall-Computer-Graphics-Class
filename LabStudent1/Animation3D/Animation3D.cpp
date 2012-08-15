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
#include <vector>

#include <GL/glut.h>

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("Animation3D.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   return 0;
}