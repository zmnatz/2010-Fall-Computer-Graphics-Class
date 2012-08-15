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

#ifndef __UNITSQUARESURFACE_H
#define __UNITSQUARESURFACE_H

#include <list>
#include <GL/glut.h>
#include "scene/scene.h"

/**
 * Unit square geometry node.
 */
class UnitSquareSurface: public GeometryNode
{
public:
   /**
    * Constructor. Construct the vertex list for a triangle fan
    * representing a unit square
    */
	UnitSquareSurface() 
   {
      m_vertexList.push_back(Point2(-0.5f,  0.5f));
      m_vertexList.push_back(Point2(-0.5f, -0.5f));
      m_vertexList.push_back(Point2( 0.5f,  0.5f));
      m_vertexList.push_back(Point2( 0.5f, -0.5f));
   }

   /**
    * Destructor
    */
	virtual ~UnitSquareSurface() { }

	/**
	 * Draw a unit square. Note that geometry nodes are at the
    * leaves of the tree, so no children exist.
	 */
	virtual void Draw()
	{
      // Set a single normal
      glNormal3f(0.0f, 0.0f, 1.0f);

      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, (void*)&m_vertexList[0]);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_vertexList.size());
      glDisableClientState(GL_VERTEX_ARRAY);
	}

protected:
   std::vector<Point2> m_vertexList;
};

#endif