//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    TransformNode.h
//	Purpose:	Scene graph transformation node.
//
//============================================================================

#ifndef __TRANSFORMNODE_H
#define __TRANSFORMNODE_H

#include <list>
#include <GL/glut.h>
#include "geometry/geometry.h"

/**
 * Transform node. Applies a transformation. This class allows OpenGL style 
 * transforms applied to the scene graph.
 */
class TransformNode: public SceneNode
{
public:
   /**
    * Constructor.
    */
	TransformNode()
   { 
      m_nodeType = SCENE_TRANSFORM; 
      LoadIdentity();
   }

   /**
    * Destructor.
    */
	virtual ~TransformNode() { }

   /**
	 * Set the identity matrix 
	 */
	void LoadIdentity()
	{
		t.SetIdentity();
	}
	
	/**
	 * Apply a translation
	 */
	void Translate(const float x, const float y, const float z)
	{
		t.Translate(x,y,z);
	}
	
	/**
	 * Apply a rotation.
	 */
	void Rotate(const float deg, const float x, const float y, const float z)
	{
		t.Rotate(deg,x,y,z);
	}

	/**
	 * Apply a scaling.
	 */
	void Scale(const float x, const float y, const float z)
	{
		t.Scale(x,y,z);
	}

	/**
	 * Draw this transformation node and its children
	 */
	virtual void Draw()
	{
		glPushMatrix();
			glMultMatrixf(t.Get());
			// Loop through the list and draw the children
			SceneNode::Draw();
		glPopMatrix();
	}


   /**
	 * Update the scene node and its children
	 */
	virtual void Update()
   {
	   SceneNode::Update();
   }

protected:
	Matrix4x4 t;
	std::list<SceneNode*> m_children;
};

#endif