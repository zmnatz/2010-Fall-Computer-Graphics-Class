//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSphere.h
//	Purpose:	Simple geometry node that defines a unit sphere and draws
//          it with glutSolidSphere.
//
//============================================================================

#ifndef __UNITSPHERE_H
#define __UNITSPHERE_H

#include <list>
#include <GL/glut.h>
#include "scene/scene.h"

/**
 * Unit sphere geometry node.
 */
class UnitSphere: public GeometryNode
{
public:
   /**
    * Constructor.
    */
	UnitSphere() { }

   /**
    * Destructor
    */
	virtual ~UnitSphere() { }

	/**
	 * Draw the ball / sphere. Note that geometry nodes are at the
    * leaves of the tree, so no children exist.
	 */
	virtual void Draw()
	{
      // Not the most efficeint way to draw a sphere, but good for now
      glutSolidSphere(1.0, 36, 18);
	}

protected:
};

#endif