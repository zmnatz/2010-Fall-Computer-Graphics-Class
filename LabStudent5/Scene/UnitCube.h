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

#ifndef __UNITCUBE_H
#define __UNITCUBE_H

#include <GL/glut.h>

/**
 * Unit square geometry node.
 */
class UnitCube: public GeometryNode
{
public:
   /**
    * Constructor. Construct a set of 6 flat faces
    */
	UnitCube() { }

	UnitCube(int numDivisions,bool useVbo)
	{
		UnitSquareFlatSurface* surface = new UnitSquareFlatSurface(numDivisions,numDivisions,useVbo);
		TransformNode* top = new TransformNode;
		top->Translate(0,0,.5);
		top->AddChild(surface);
		SceneNode::AddChild(top);

		TransformNode* side = new TransformNode;
		side->Rotate(90,1,0,0);
		side->AddChild(top);

		for(int i=0;i<4;i++){
		   TransformNode* otherSide = new TransformNode;
		   otherSide->Rotate(90*i,0,0,1);
		   otherSide->AddChild(side);
		   SceneNode::AddChild(otherSide);
		}

		TransformNode* bottom = new TransformNode;
		bottom->Rotate(180,1,0,0);
		bottom->AddChild(top);
		SceneNode::AddChild(bottom);
	}

   /**
    * Destructor
    */
	virtual ~UnitCube() { }

	/**
	 * Draw a unit square. Note that geometry nodes are at the
    * leaves of the tree, so no children exist.
	 */
	virtual void Draw()
	{
		SceneNode::Draw();
	}

protected:

};

#endif