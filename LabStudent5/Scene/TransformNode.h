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
    * Constructor. Sets the node type to TRANSFORM and loads the identity matrix.
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
	 * Set the stored matrix to the identity matrix.
	 */
	void LoadIdentity()
	{
		m_matrix.SetIdentity();
	}
	
	/**
	 * Apply a translation to the stored matrix.
    * @param  x   x translation
    * @param  y   y translation
    * @param  z   z translation
	 */
	void Translate(const float x, const float y, const float z)
	{
		m_matrix.Translate(x, y, z);
	}
	
	/**
	 * Apply a rotation to the stored matrix.
    * @param  deg   Rotation angle in degrees
    * @param  x     x component of the roation axis
    * @param  y     y component of the roation axis
    * @param  z     z component of the roation axis
	 */
	void Rotate(const float deg, const float x, const float y, const float z)
	{
		m_matrix.Rotate(deg, x, y, z);
	}

	/**
	 * Apply a scale to the stored matrix.
    * @param  x   x scaling
    * @param  y   y scaling
    * @param  z   z scaling
	 */
	void Scale(const float x, const float y, const float z)
	{
		m_matrix.Scale(x, y, z);
	}

	/**
	 * Draw this transformation node and its children. Note how this uses push
    * and pop matrix to retain state.
	 */
	virtual void Draw()
	{
      // Copy the current matrix state
		glPushMatrix();

      // Perform this node's transformations - will postmultiply the current OpenGL matrix
		glMultMatrixf(m_matrix.Get());

      // Draw all children
		SceneNode::Draw();

      // Restore the saved matrix state
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
   // Modeling transformation
	Matrix4x4 m_matrix;
};

#endif