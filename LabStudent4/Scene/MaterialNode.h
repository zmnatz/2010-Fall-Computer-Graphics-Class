//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    PresentationNode.h
//	Purpose:	Scene graph presentation node.
//
//============================================================================

#ifndef __MATERIALNODE_H
#define __MATERIALNODE_H

#include <GL/glut.h>

/**
 * Presentation node.
 */
class MaterialNode: public PresentationNode
{
public:
   /**
    * Constructor.
    */
	MaterialNode(float r,float g,float b) {
		color[0]=r;
		color[1]=g;
		color[2]=b;
	}

   /**
    * Destructor
    */
	virtual ~MaterialNode() { }

	/**
	 * Draw this presentation node and its children
	 */
	virtual void Draw()
	{
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);
			PresentationNode::Draw();
		glPopMatrix();
	}

protected:
	float color[3];
};

#endif