//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	Zaven Mnatzakanian
//	File:    LightNode.h
//	Purpose: Scene graph light node.
//
//============================================================================

#ifndef __LIGHTNODE_H
#define __LIGHTNODE_H

/**
 * Light Node: Stores and applies lighting properties.
 */
class LightNode : public SceneNode
{
public:
   /**
    * Constructor
    */
   LightNode(GLenum l)
   {
	   light=l;
		m_nodeType = SCENE_LIGHT;
   }

	void SetLightDiffuse(const Color4& c)
	{
		m_diffuse = c;
	}
	
	void SetLightSpecular(const Color4& c)
	{
		m_specular = c;
	}

	void SetLightPosition(const HPoint3& h){
		m_position = h;
	}

	void SetSpotlight(const Vector3& v,const GLfloat& cutoff,const GLfloat& exponent){
		m_spot_direction = v;
		m_spot_cutoff = cutoff;
		m_spot_exponent = exponent;
	}

	void Draw() {
		glEnable(light);
		if(&m_diffuse!=0)
			glLightfv(light,GL_DIFFUSE,&(m_diffuse.r));
		if(&m_specular!=0)
			glLightfv(light,GL_SPECULAR,&(m_specular.r));
		if(&m_position!=0)
			glLightfv(light,GL_POSITION,&(m_position.x));
		if(&m_spot_direction!=0)
			glLightfv(light,GL_SPOT_DIRECTION,&(m_spot_direction.x));
		if(&m_spot_cutoff!=(void*)0)
			glLightfv(light,GL_SPOT_CUTOFF,&m_spot_cutoff);
		if(&m_spot_exponent!=(void*)0)
			glLightfv(light,GL_SPOT_EXPONENT,&m_spot_exponent);
		SceneNode::Draw();
	}
   /**
    * Destructor
    */
   virtual ~LightNode() { }

   protected:
	GLenum light;
	Color4 m_diffuse;
	Color4 m_specular;
	HPoint3 m_position;
	Vector3 m_spot_direction;
	GLfloat m_spot_cutoff;
	GLfloat m_spot_exponent;
};

#endif