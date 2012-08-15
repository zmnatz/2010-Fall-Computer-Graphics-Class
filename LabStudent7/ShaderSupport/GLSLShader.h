//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Base GLSL shader class
//	Purpose: Support for loadng a shader from a file and compiling it
//
//============================================================================

#ifndef __GLSLSHADER_H__
#define __GLSLSHADER_H__

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <fcntl.h>
#include <io.h>

/**
 * Base shader class. Loads from file. Checks compile status.
 */
class GLSLShader
{
public:
   GLSLShader()  { }
   ~GLSLShader() { }

   bool CheckCompileStatus(GLuint shader)
   {
      int param = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
      return (param == GL_TRUE);
   }

protected:
   // Utility to read a shader source file
   char* readShaderSource(const char* filename) 
   {
      char* content = NULL;
      if (filename != NULL)
      {
         // Get file size
         int f = _open(filename, O_RDONLY);
         int count = _lseek(f, 0, SEEK_END);
         _close(f);

         FILE* fp = fopen(filename, "rt");
         if (fp != NULL) 
         {
            if (count > 0) 
            {
               content = (char *)new char[count+1];
               count = (int)fread(content, sizeof(char), count, fp);
               content[count] = '\0';
            }
            fclose(fp);
         }
         else
            printf("Could not read file %s\n", filename);
      }
      else
         printf("Could not read file: %s\n", filename);

      return content;
   } 

   /**
    * Logs a shader compile error
    */
   void logCompileError(GLuint shader)
   {
      GLint    len = 0; 
      GLsizei slen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &len); 
      if (len > 1)
      {
         GLchar* compilerLog = (GLchar*)new GLchar*[len];
         glGetShaderInfoLog(shader, len, &slen, compilerLog);
         printf("compiler log:\n%s", compilerLog);
         delete [] compilerLog;
      }
   }
};

// Include derived classes so application only add this one file
#include "ShaderSupport/GLSLFragmentShader.h"
#include "ShaderSupport/GLSLVertexShader.h"
#include "ShaderSupport/GLSLShaderProgram.h"

#endif
