//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    GLSL fragment shader class
//	Purpose: Support for GLSL fragment shaders. Supports loading, compiling, 
// linking, attaching, etc.
//============================================================================

#ifndef __GLSLFRAGMENTSHADER_H__
#define __GLSLFRAGMENTSHADER_H__

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <fcntl.h>
#include <io.h>

/**
 * GLSL vertex shader
 */
class GLSLFragmentShader : public GLSLShader
{
public:
   GLSLFragmentShader()  { }
   ~GLSLFragmentShader() { }

   bool Create(const char* fragSource)
   {
      bool success = true;
      m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      char* vs = readShaderSource(fragSource);
      const char* vv = vs;
      glShaderSource(m_fragmentShader, 1, &vv, NULL);
      glCompileShader(m_fragmentShader);
      if (!CheckCompileStatus(m_fragmentShader))
      {
         printf("Fragment shader compile failed\n");
         logCompileError(m_fragmentShader);
         success = false;
      }
      delete [] vs;
      return success;
   }

   GLuint Get() const
   {
      return m_fragmentShader;
   }

protected:
   GLuint m_fragmentShader;
};

#endif
