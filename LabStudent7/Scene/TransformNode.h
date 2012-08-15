//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    TransformNode.h
//	Purpose: Scene graph transformation node.
//
//============================================================================

#ifndef __TRANSFORMNODE_H
#define __TRANSFORMNODE_H

/**
 * Transform node. Applies a transformation. This class stores a local
 * modeling transformation matrix and allows OpenGL style transforms
 * to be applied to this matrix.
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
	 * Set the stored matrix to the identity matrix 
	 */
	void LoadIdentity()
	{
		m_matrix.SetIdentity();
	}
	
	/**
	 * Apply a translation to the stored matrix.
	 */
	void Translate(const float x, const float y, const float z)
	{
		m_matrix.Translate(x, y, z);
	}
	
	/**
	 * Apply a rotation to the stored matrix.
	 */
	void Rotate(const float deg, const float x, const float y, const float z)
	{
		m_matrix.Rotate(deg, x, y, z);
	}

	/**
	 * Apply a translation to the stored matrix.
	 */
	void Scale(const float x, const float y, const float z)
	{
		m_matrix.Scale(x, y, z);
	}

	/**
	 * Draw this transformation node and its children
	 */
	virtual void Draw()
	{
  		// Copy the current matrix state
		glPushMatrix();
		
		// Perform this node's transformations
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