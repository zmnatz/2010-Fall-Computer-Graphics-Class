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

#ifndef __PRESENTATIONNODE_H
#define __PRESENTATIONNODE_H

#include <list>
#include <GL/glut.h>

/**
 * Presentation node.
 */
class PresentationNode: public SceneNode
{
public:
   /**
    * Constructor.
    */
	PresentationNode() { }

   /**
    * Destructor
    */
	virtual ~PresentationNode() { }

	/**
	 * Draw this presentation node and its children
	 */
	virtual void Draw()
	{
      SceneNode::Draw();
	}

protected:
	std::list<SceneNode*> m_children;
};

#endif