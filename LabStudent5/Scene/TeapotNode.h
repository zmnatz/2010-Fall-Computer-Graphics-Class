//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSquareSurface.h
//	Purpose:	Simple geometry node that defines a unit square.
//
//============================================================================

#ifndef __TEAPOTNODE_H
#define __TEAPOTNODE_H

#include <GL/glut.h>

// Teapot class - very simple class to use GLUT to draw a teapot
class TeapotNode: public GeometryNode
{
public:
    TeapotNode() { }
    virtual ~TeapotNode() { }
    virtual void Draw()
    {
      // Teapot is defined with CW faces and is "open" at the top. Disable
      // back face culling, draw, then re-enable culling
      glDisable(GL_CULL_FACE);
      glutSolidTeapot(5.0f);
      glEnable(GL_CULL_FACE);
    }
};

#endif