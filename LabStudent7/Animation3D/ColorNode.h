//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    ColorNode.h
//	Purpose:	Simple presentation node that defines a color.
//
//============================================================================

#ifndef __COLORNODE_H
#define __COLORNODE_H

#include <list>
#include <GL/glut.h>
#include "scene/scene.h"

/**
 * Color presentation node.
 */
class ColorNode: public PresentationNode
{
public:
   /**
    * Constructor.
    */
	ColorNode() { }

   /**
    * Constructor given a red, green, and blue value.
    */
   ColorNode(const float r, const float g, const float b)
   {
      rgb[0] = r;
      rgb[1] = g;
      rgb[2] = b;
   }

   /**
    * Destructor
    */
	virtual ~ColorNode() { }

	/**
	 * Draw this presentation node and its children
	 */
	virtual void Draw()
	{
      // Set the current color and draw all children. Very simple lighting support
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rgb);
      SceneNode::Draw();
	}

protected:
   float rgb[3];
};

#endif