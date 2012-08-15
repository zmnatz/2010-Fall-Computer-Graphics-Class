//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    LightingViewing.cpp
//	Purpose: OpenGL and GLUT program to draw a simple scene with
//          viewing controls and lighting and shading.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <fcntl.h>
#include <io.h>

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("PhongShading.log", "w");

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