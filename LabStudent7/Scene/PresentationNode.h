//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    PresentationNode.h
//	Purpose: Scene graph presentation node.
//
//============================================================================

#ifndef __PRESENTATIONNODE_H
#define __PRESENTATIONNODE_H

/**
 * Presentation node. Applies material and texture
 */
class PresentationNode : public SceneNode
{
public:
	/**
	 * Constructor
	 */
	PresentationNode() 
	{
		m_nodeType = SCENE_PRESENTATION;
		m_materialShininess = 1.0f;
		m_texture = 0;

		// Note: color constructors default rgb to 0 and alpha to 1
	}
	
	/**
	 * Destructor
	 */
	~PresentationNode() { }
	
	/**
	 * Set material ambient reflection coefficient.
	 */
	void SetMaterialAmbient(const Color4& c)
	{
		m_materialAmbient = c;
	}
	
	void SetMaterialDiffuse(const Color4& c)
	{
		m_materialDiffuse = c;
	}
	
	void SetMaterialAmbientAndDiffuse(const Color4& c)
	{
		m_materialAmbient = c;
		m_materialDiffuse = c;
	}
	
	void SetMaterialSpecular(const Color4& c)
	{
		m_materialSpecular = c;
	}
	
	void SetMaterialEmission(const Color4& c)
	{
		m_materialEmission = c;
	}
	
	void SetMaterialShininess(const float s)
	{
		m_materialShininess = s;
	}
	
	void setTexture(GLuint texture){
		m_texture = texture;
	}

	/**
	 * Draw. Simply sets the material properties. Later on when we
	 * add more attribute control probably want to push/pop attributes
	 */
	void Draw()
	{
      // Copy attribute state
      glPushAttrib(GL_LIGHTING_BIT);
	  glPushAttrib(GL_TEXTURE_BIT);

		// Set material. Note that for OpenGL ES you can only use 
		// GL_FRONT_AND_BACK so we use it here instead of GL_FRONT, even
		// though we are not enabling 2-sided lighting
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  &m_materialAmbient.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  &m_materialDiffuse.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &m_materialSpecular.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &m_materialEmission.r);
		glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, m_materialShininess);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		// Draw children of this node
		SceneNode::Draw();

      // Revert to prior attribute state
       glPopAttrib();
	   glPopAttrib();
	}
	
protected:
	Color4       m_materialAmbient;
	Color4       m_materialDiffuse;
	Color4       m_materialSpecular;
	Color4       m_materialEmission;
	GLfloat      m_materialShininess;
	GLuint		 m_texture;
};

#endif