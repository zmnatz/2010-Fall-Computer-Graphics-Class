//============================================================================
//
//	Author:	 David W. Nesbitt
//	File:    LightNode.h
//	Purpose: Scene graph light node.
//
//============================================================================

#ifndef __LIGHTNODE_H_
#define __LIGHTNODE_H_

#include <list>

// TODO - add support for spotlights and attenuation

/**
 * Presentation node. Applies material and texture
 */
class LightNode : public SceneNode
{
public:
	/**
	 * Constructor
	 */
	LightNode(const unsigned int index) 
	{
		m_nodeType = SCENE_LIGHT;
		m_index = index;

      // Set default attenuation to 1 and spotlight to false
      m_atten0 = 1;
      m_atten1 = 1;
      m_atten2 = 1;
      m_isSpotlight = false;
		
		// Note: color constructors default rgb to 0 and alpha to 1
	}
	
	/**
	 * Destructor
	 */
	~LightNode() { }
	
	/** 
	 * Enable this light source
	 */
	void Enable()
	{
		m_enabled = true;

      // Set light intensity again
      glLightfv(m_index, GL_AMBIENT,  &m_ambient.r);
		glLightfv(m_index, GL_DIFFUSE,  &m_diffuse.r);
		glLightfv(m_index, GL_SPECULAR, &m_specular.r);

      glEnable(m_index);
	}
	
	/** 
	 * Disable this light source
	 */
	void Disable()
	{
		m_enabled = false;

      // Set ambient, diffuse, and specular color to 0 so shader knows the
      // light is off
      float black[4];
      black[0] = 0.0f; black[1] = 0.0f; black[2] = 0.0f; black[4] = 1.0f;
      glLightfv(m_index, GL_AMBIENT,  black);
		glLightfv(m_index, GL_DIFFUSE,  black);
      black[4] = 0.0f;
		glLightfv(m_index, GL_SPECULAR, black);

      glDisable(m_index);
	}
		
	/**
	 * Set ambient light illumination (color).
	 */
	void SetAmbient(const Color4& c)
	{
		m_ambient = c;
      glLightfv(m_index, GL_AMBIENT,  &m_ambient.r);
	}
	
	/**
	 * Set diffuse light illumination (color).
	 */
	void SetDiffuse(const Color4& c)
	{
		m_diffuse = c;
      glLightfv(m_index, GL_DIFFUSE,  &m_diffuse.r);
	}
	
	/**
	 * Set specular light illumination (color).
	 */
	void SetSpecular(const Color4& c)
	{
		m_specular = c;
      glLightfv(m_index, GL_SPECULAR, &m_specular.r);
	}
	
	/**
	 * Set the light position. Uses a homogeneous coordinate. If w = 0 the light is 
	 * directional. Position is set within the Draw method.
	 */
	void SetPosition(const HPoint3& position)
	{
		m_position = position;
	}

   /**
    * Set spotlight parameters
    */
   void SetSpotlight(const Vector3& spotDir, const float spotExponent, const float spotCutoff)
   {
      m_spotDirection = spotDir;
      m_spotExponent  = spotExponent;
      m_spotCutoff    = spotCutoff;
      m_isSpotlight   = true;

      glLightfv(m_index, GL_SPOT_EXPONENT, &m_spotExponent);
      glLightfv(m_index, GL_SPOT_CUTOFF,   &m_spotCutoff);
   }

   /**
    * Set the spotlight direction
    */
   void SetSpotlightDirection(const Vector3& spotDir)
   {
      m_spotDirection = spotDir;
   }
	
   /**
    * Disable spotlight
    */
   void TurnOffSpotlight() 
   {
      m_isSpotlight = false;
//      glLightfv(m_index, GL_SPOT_EXPONENT, &m_spotExponent);
//      glLightfv(m_index, GL_SPOT_CUTOFF,   &m_spotCutoff);
   }

   /**
    * Set attenuation
    */
   void SetAttenuation(const float constant, const float linear, const float quadratic)
   {
      m_atten0 = constant;
      m_atten1 = linear;
      m_atten2 = quadratic;
      if (m_atten0 != 1.0f || m_atten1 != 1.0f || m_atten1 != 1.0f)
      {
         glLightfv(m_index, GL_CONSTANT_ATTENUATION,  &m_atten0);
         glLightfv(m_index, GL_LINEAR_ATTENUATION,    &m_atten1);
         glLightfv(m_index, GL_QUADRATIC_ATTENUATION, &m_atten2);
      }
   }

	/**
	 * Draw. Sets the light properties if enabled. Note that only position
    * is set within the Draw method - since it needs to be transformed by
    * the current matrix. 
	 */
	void Draw()
	{
		if (m_enabled)
		{
         glLightfv(m_index, GL_POSITION, &m_position.x);
         if (m_isSpotlight)
            glLightfv(m_index, GL_SPOT_DIRECTION, &m_spotDirection.x);
      }
		
		// Draw children of this node
		SceneNode::Draw();
	}
	
protected:
	bool         m_enabled;
   bool         m_isSpotlight;
	unsigned int m_index;
	Color4       m_ambient;
	Color4       m_diffuse;
	Color4       m_specular;
   Vector3      m_spotDirection;
   float        m_spotCutoff;
   float        m_spotExponent;
   float        m_atten0;
   float        m_atten1; 
   float        m_atten2;
	
	// Light position as a homogeneous coordinate. If w = 0 the light is directional
	HPoint3      m_position;
};

#endif