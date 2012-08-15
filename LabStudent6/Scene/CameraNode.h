//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Camera.h
//	Purpose: Provide support for setting and updating the view and projection.
//
//============================================================================

#ifndef __CAMERA_H
#define __CAMERA_H

enum ProjectionType 	{ PERSPECTIVE, ORTHOGRAPHIC };

/**
 * Camera node class.  
 */
class CameraNode : public SceneNode
{
public:
	/**
	 * Constructor. 
	 */
	CameraNode()
	{
		m_nodeType = SCENE_CAMERA;
		
		// Perspective settings
		m_fov    = 50.0f;
		m_aspect = 1.0f;
		m_near   = 1.0f;
		m_far    = 1000.0f;

		// Initial view settings
		m_lpt    = Point3(0.0f, 0.0f, 0.0f);
		m_vrp    = Point3(0.0f, 0.0f, 1.0f);
		m_v      = Vector3(0.0f, 1.0f, 0.0f);
	}

	/**
	 * Destructor.
	 */
	~CameraNode() { }
	
	/**
	 * Draw the scene node and its children. The base class just draws the
	 * children. Derived classes can use this (SceneNode::Draw()) to draw
	 * all children without having to duplicate this code.
	 */
	void Draw()
	{
		glPushMatrix();
		// Set the viewing matrix. We are already in ModelView mode
		glLoadMatrixf(m_view.Get());		
		
		// Draw children
		SceneNode::Draw();
		glPopMatrix();
	}
	
	/**
	 * Sets the view reference point (camera position)
	 *
 	 *	@param	vp		View reference point.
	 */
	void SetPosition(const Point3& vp)
	{
		m_vrp = vp;
		lookAt();
	}

	/**
	 * Gets the view reference point (camera position)
	 *
	 *	@return		Returns the view reference point.
	 */
	Point3 GetPosition(void) const
	{
		return m_vrp;
	}

	/**
	 * Sets the lookat point.
	 *
	 * @param	lp		Lookat point.
	 */
	void SetLookAtPt(const Point3& lp)
	{
		m_lpt = lp;
		lookAt();
	}

	/**
	 * Gets the lookat point.
	 *
	 * @return	Returns the lookat point.
	 */
	Point3 GetLookAtPt(void) const
	{
		return m_lpt;
	}

	/**
	 * Sets the view up direction.
	 *
	 * @param	vup		View up direction.
	 */
	void SetViewUp(const Vector3& vup)
	{
		m_v = vup;
		lookAt();
	}

	/**
	 *	Gets the view plane normal (vz axis).
	 *
	 * @return	Returns the view plane normal (vz)
	 */
	Vector3 GetViewPlaneNormal() const
	{
		return m_n;
	}

	/**
	 *	Gets the view right axis (vx axis).
	 *
	 * @return	Returns the view right axis (vx)
	 */
	Vector3 GetViewRight() const
	{
		return m_u;
	}

	/**
	 *	Gets the view up axis (vy axis).
	 *
	 * @return	Returns the view up axis (vy)
	 */
	Vector3 GetViewUp() const
	{
		return m_v;
	}

	/**
	 * Roll the camera by the specified degrees about the VPN. 
	 * Rotation about n. Note that the lookat point does not change
	 */
	void Roll(const float degrees)
	{
		// Method below is described in FS Hill and lecture notes. Just 
		// change the two axes and reset the view matrix
		float radians = degreesToRadians(degrees);
		float c = cosf(radians);
		float s = sinf(radians);

		Vector3 u = m_u;
		m_u.Set( c * u.x + s * m_v.x, 
				 c * u.y + s * m_v.y, 
				 c * u.z + s * m_v.z);
		m_v.Set(-s * u.x + c * m_v.x, 
				-s * u.y + c * m_v.y, 
				-s * u.z + c * m_v.z);
		setViewMatrix();
	}

	/**
	 * Change the pitch of the camera by the specified degrees.
	 * This is a rotation about u
	 */
	void Pitch(const float degrees)
	{
		// Method below is described in FS Hill and lecture notes. Just 
		// change the two axes and reset the view matrix
		float radians = (float)degreesToRadians(degrees);
		float c = cosf(radians);
		float s = sinf(radians);
		Vector3 v = m_v;
		m_v.Set( c * v.x + s * m_n.x, 
				 c * v.y + s * m_n.y, 
				 c * v.z + s * m_n.z);
		m_n.Set(-s * v.x + c * m_n.x, 
				-s * v.y + c * m_n.y, 
				-s * v.z + c * m_n.z);

		setViewMatrix();

		// Reset the lookat (keep the same distance)
		Vector3 v1 = m_vrp - m_lpt;
		float d = v1.Norm();
		m_lpt = m_vrp - m_n * d;
	}

	/**
	 * Change the heading of the camera by the specified degrees.
	 * This is a rotation about v
	 */
	void Heading(const float degrees)
	{
		// Method below is described in FS Hill and lecture notes. Just 
		// change the two axes and reset the view matrix
		float radians = (float)degreesToRadians(degrees);
		float c = cosf(radians);
		float s = sinf(radians);
		Vector3 u = m_u;
		m_u.Set(c * u.x - s * m_n.x, 
				c * u.y - s * m_n.y, 
				c * u.z - s * m_n.z);
		m_n.Set(s * u.x + c * m_n.x, 
				s * u.y + c * m_n.y, 
				s * u.z + c * m_n.z);

		setViewMatrix();

		// Reset the lookat (keep the same distance)
		Vector3 v1 = m_vrp - m_lpt;
		float d = v1.Norm();
		m_lpt = m_vrp - m_n * d;
	}

	/**
	 * Move and Turn.  Move the LookAt point by the specified vector.
	 * The VRP is then moved so that it is the same distance from the 
	 * LookAt point as previous.  This effectively moves and turns.
	 *
	 * NOTE: a different method would involve a rotation usinfg pitch and 
	 * heading and then move forward or backwards along the new VPN. I figure
	 * that method is more common and wanted to present the method below
	 * as an alternate way to approach the problem
	 */
	void MoveAndTurn(const float right, const float up, const float forward)
	{
		// Construct a vector from current lookat to VRP
		Vector3 v1 = m_vrp - m_lpt;
		float d = v1.Norm();

		// Move the lookAt point by specified distances along the 3 view axes.
		Vector3 dir = m_u * right + m_v * up + m_n * -forward;
		m_lpt = m_lpt + dir;

		// Construct vector from new LookAt to current VRP
		Vector3 v2 = m_vrp - m_lpt;

		// Find the projection of v1 onto v2. Keep same distance as previous
		Vector3 v3 = v1.Projection(v2);
		v3.Normalize();
		v3 *=d;

		// New VRP is the new lookat point plus v3
		m_vrp = m_lpt + v3;
		lookAt();
	}

   /**
    * Gets the current matrix (used to store modeling transforms).
    *
    *	@return	Returns the current modeling/viewing composite matrix.
    */
	Matrix4x4 GetMatrix() const
	{
      return m_view;
   }

   /**
    * Sets a perspective projection
    */
   void SetPerspective(const float fov, const float aspect, 
                       const float n, const float f)
   {
      m_fov    = fov;
      m_aspect = aspect;
      m_near   = n;
      m_far    = f;
      setPerspective();
   }

   /**
    * Change the field of view.
    */
   void ChangeFieldOfView(const float fov)
   {
      m_fov = fov;
      setPerspective();
   }

   /**
    * Change the aspect ratio.
    */
   void ChangeAspectRatio(const float aspect)
   {
      m_aspect = aspect;
      setPerspective();
   }

   /**
    * Change the near and far clipping planes.
    */
   void ChangeClippingPlanes(const float n, const float f)
   {
      m_near = n;
      m_far  = f;
      setPerspective();
   }

private:
	// Perspective projection parameters
	float   m_fov;      // Field of view in degrees
	float   m_aspect;   // Aspect ratio (width / height)
	float   m_near;     // Near clipping plane distance
	float   m_far;      // Far clipping plane distance

	Point3  m_vrp;		// View point (eye)
	Point3  m_lpt;		// Lookat point

	// View axes
	Vector3  m_n;		// View plane normal
	Vector3  m_u;		// View right axis
	Vector3  m_v;		// View up axis

	// Matrices
	Matrix4x4 m_view;	         // Viewing matrix
	Matrix4x4 m_projection;	   // Projection matrix

   // Sets the view axes
   void lookAt()
   {
  	   // Set the VPN, which is the vector vp - vc
	   m_n = m_vrp - m_lpt;
	   m_n.Normalize();

	   // Get the view right axis from UP cross VPN
	   m_u = m_v.Cross(m_n);
	   m_u.Normalize();

	   // Calculate the view up axis from VPN cross VRT. No need to normalize
      //    sinfce VPN and VRT are orthogonal and unit length 
	   m_v = m_n.Cross(m_u);

      // Set the view matrix
      setViewMatrix();
	}

   void setPerspective()
   {
      // Get the dimensions at the near clipping plane
      float h = m_near * tanf(degreesToRadians(m_fov * 0.5f));
      float w = m_aspect * h;

      // Set the elements of the perspective projection matrix
      m_projection.m11() =  m_near / w;
      m_projection.m12() =  0.0f;
      m_projection.m13() =  0.0f;
      m_projection.m14() =  0.0f;
      m_projection.m21() =  0.0f;
      m_projection.m22() =  m_near / h;
      m_projection.m23() =  0.0f;
      m_projection.m24() =  0.0f;
      m_projection.m31() =  0.0f;
      m_projection.m32() =  0.0f;
      m_projection.m33() = -(m_far + m_near) / (m_far - m_near);
      m_projection.m34() = -(2.0f * m_far * m_near) / (m_far - m_near);
      m_projection.m41() =  0.0f;
      m_projection.m42() =  0.0f;
      m_projection.m43() = -1.0f;
      m_projection.m44() =  0.0f;

      // Set matrix mode to projection and load the matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m_projection.Get());		

		// Set the matrix mode back to ModelView so we are always in ModelView
		// mode except when in this method
		glMatrixMode(GL_MODELVIEW);
   }

	// Create viewing transformation matrix by composinfg the translation 
	// matrix with the rotation matrix given by the view coordinate axes
	void setViewMatrix()
	{
		// Set the view matrix usinfg the view axes and the 
		//    translation (simplified)
		float x = -m_vrp.x;
		float y = -m_vrp.y;
		float z = -m_vrp.z;
		m_view.m11() = m_u.x;
		m_view.m12() = m_u.y;
		m_view.m13() = m_u.z;
		m_view.m14() = m_u.x * x + m_u.y * y + m_u.z * z;
		m_view.m21() = m_v.x;
		m_view.m22() = m_v.y;
		m_view.m23() = m_v.z;
		m_view.m24() = m_v.x * x + m_v.y * y + m_v.z * z;
		m_view.m31() = m_n.x;
		m_view.m32() = m_n.y;
		m_view.m33() = m_n.z;
		m_view.m34() = m_n.x * x + m_n.y * y + m_n.z * z;
		m_view.m41() = 0.0f;
		m_view.m42() = 0.0f;
		m_view.m43() = 0.0f;
		m_view.m44() = 1.0f;
	}
};

#endif
