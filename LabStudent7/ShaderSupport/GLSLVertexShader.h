//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    GLSL vertex shader class
//	Purpose: Support for GLSL vertex shaders. Supports loading, compiling, 
// linking, attaching, etc.
//
//============================================================================

#ifndef __GLSLVERTEXSHADER_H__
#define __GLSLVERTEXSHADER_H__

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
class GLSLVertexShader : public GLSLShader
{
public:
   GLSLVertexShader()  { }
   ~GLSLVertexShader() { }

   bool Create(const char* vertSource)
   {
      bool success = true;
      m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
      char* vs = readShaderSource(vertSource);
      const char* vv = vs;
      glShaderSource(m_vertexShader, 1, &vv, NULL);
      glCompileShader(m_vertexShader);
      if (!CheckCompileStatus(m_vertexShader))
      {
         printf("Vertex shader compile failed\n");
         logCompileError(m_vertexShader);
         success = false;
      }
      delete [] vs;
      return success;
   }

   GLuint Get() const
   {
      return m_vertexShader;
   }

protected:
   GLuint m_vertexShader;
};

#endif
