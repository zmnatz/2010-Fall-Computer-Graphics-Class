//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt

//	Author:  David W. Nesbitt
//	File:    Matrix.h
//	Purpose: Matrix class
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#ifndef __MATRIX4x4_H__
#define __MATRIX4x4_H__

#include <math.h>

/**
 * 4x4 matrix
 */
class Matrix4x4
{
public:
	/**
    * Constructor.  Sets the matrix to the identity matrix
    */
	Matrix4x4(void)
	{
      SetIdentity();
	}

   /**
    * Sets the matrix to the identity matrix.
    */
	void SetIdentity(void)
	{
      a[0] = 1.0f; a[4] = 0.0f; a[8]  = 0.0f; a[12] = 0.0f;
      a[1] = 0.0f; a[5] = 1.0f; a[9]  = 0.0f; a[13] = 0.0f;
      a[2] = 0.0f; a[6] = 0.0f; a[10] = 1.0f; a[14] = 0.0f;
      a[3] = 0.0f; a[7] = 0.0f; a[11] = 0.0f; a[15] = 1.0f;
	}

	/**
    * Copy constructor
    * @param  n  Matrix to copy
    */
	Matrix4x4(const Matrix4x4& n)
	{
      m11() = n.m11(); m12() = n.m12(); m13() = n.m13(); m14() = n.m14();
      m21() = n.m21(); m22() = n.m22(); m23() = n.m23(); m24() = n.m24();
      m31() = n.m31(); m32() = n.m32(); m33() = n.m33(); m34() = n.m34();
      m41() = n.m41(); m42() = n.m42(); m43() = n.m43(); m44() = n.m44();
	}

   /**
    * Assignment operator
    * @param   n  Matrix to assign to this matrix
    * @return  Returns the address of this matrix.
    */
	Matrix4x4& operator = (const Matrix4x4& n)
	{
      m11() = n.m11(); m12() = n.m12(); m13() = n.m13(); m14() = n.m14();
      m21() = n.m21(); m22() = n.m22(); m23() = n.m23(); m24() = n.m24();
      m31() = n.m31(); m32() = n.m32(); m33() = n.m33(); m34() = n.m34();
      m41() = n.m41(); m42() = n.m42(); m43() = n.m43(); m44() = n.m44();
		return *this;
	}

   /**
    * Equality operator
    * @param   n  Matrix to test for equality with this matrix.
    * @return  Returns true if hte matrices are equal, false otherwise..
    */
	bool operator == (const Matrix4x4& n) const
	{
      return (m11() == n.m11() && m12() == n.m12() && m13() == n.m13() && m14() == n.m14() &&
              m21() == n.m21() && m22() == n.m22() && m23() == n.m23() && m24() == n.m24() &&
              m31() == n.m31() && m32() == n.m32() && m33() == n.m33() && m34() == n.m34() &&
              m41() == n.m41() && m42() == n.m42() && m43() == n.m43() && m44() == n.m44());
	}

	/**
    * Destructor
    */
	~Matrix4x4(void) { }

   /**
    * Set the matrix to the values specified in the array.  This array
    * can be retrieved from OpenGL.
    * @param  m  Array of float values to fill in this matrix. The
    *            elements are arranged in column order.
    */
   void Set(const float* m)
   {
      for (int i = 0; i < 16; i++)
         a[i] = m[i];
   }

   /**
    * Gets the matrix (can be passed to OpenGL)
    * @return   Returns the elements of this matrix in column order.
    */
   const float* Get(void) const
   {
      // a is stored in column order so we can just return the array
      return a;
   }

   // Read-only access functions
   float m11(void) const { return a[0];  }
	float m12(void) const { return a[4];  }
	float m13(void) const { return a[8];  }
	float m14(void) const { return a[12]; }
	float m21(void) const { return a[1];  }
	float m22(void) const { return a[5];  }
	float m23(void) const { return a[9];  }
	float m24(void) const { return a[13]; }
	float m31(void) const { return a[2];  }
	float m32(void) const { return a[6];  }
	float m33(void) const { return a[10]; }
	float m34(void) const { return a[14]; }
	float m41(void) const { return a[3];  }
	float m42(void) const { return a[7];  }
	float m43(void) const { return a[11]; }
	float m44(void) const { return a[15]; }


   /**
    * Gets a matrix element given by row,column. NOTE: this method takes 
    * row, col as 0 based elements (so it works with the Inverse method)
    * while the individual matrix access methods use a 1-based indexing.
    * @param  row   Matrix row (0-based)
    * @param  col   Matrix col (0-based)
    * @return Returns the element at the specified row,col.
    */
   float m(const unsigned int row, const unsigned int col) const
   {
      return (row < 4 && col < 4) ? a[col * 4 + row] : 0.0f;
   }

   // Read-write access functions
   float& m11(void) { return a[0];  }
   float& m12(void) { return a[4];  }
   float& m13(void) { return a[8];  }
   float& m14(void) { return a[12]; }
   float& m21(void) { return a[1];  }
   float& m22(void) { return a[5];  }
   float& m23(void) { return a[9];  }
	float& m24(void) { return a[13]; }
	float& m31(void) { return a[2];  }
	float& m32(void) { return a[6];  }
	float& m33(void) { return a[10]; }
	float& m34(void) { return a[14]; }
	float& m41(void) { return a[3];  }
	float& m42(void) { return a[7];  }
	float& m43(void) { return a[11]; }
	float& m44(void) { return a[15]; } 

   /**
    * Gets a matrix element given by row,column. NOTE: this method takes 
    * row, col as 0 based elements (so it works with the Inverse method)
    * while the individual matrix access methods use a 1-based indexing.
    * @param  row   Matrix row (0-based)
    * @param  col   Matrix col (0-based)
    * @return Returns the address of the element at the specified row,col.
    */
   float& m(const unsigned int row, const unsigned int col)
   {
      return (row < 4 && col < 4) ? a[col * 4 + row] : a[0];
   }

   /**
    * Matrix multiplication.  Multiplies the current matrix by the matrix n
    * ( m' = m n ) and returns the resulting 4x4 matrix.
    * @param   n   Matrix to multiply the current matrix by
    * @return  Returns the product of the current matrix and the supplied matrix.
    */
   Matrix4x4 operator * (const Matrix4x4& n) const
   {
      // Unroll the loop, do 1 row at a time.
      Matrix4x4 t;
      float a0 = m11();
      float a1 = m12();
      float a2 = m13();
      float a3 = m14();
      t.m11() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
      t.m12() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
      t.m13() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
      t.m14() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
      a0 = m21();
      a1 = m22();
      a2 = m23();
      a3 = m24();
      t.m21() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
      t.m22() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
      t.m23() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
      t.m24() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
      a0 = m31();
      a1 = m32();
      a2 = m33();
      a3 = m34();
      t.m31() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
      t.m32() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
      t.m33() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
      t.m34() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
      a0 = m41();
      a1 = m42();
      a2 = m43();
      a3 = m44();
      t.m41() = a0 * n.m11() + a1 * n.m21() + a2 * n.m31() + a3 * n.m41();
      t.m42() = a0 * n.m12() + a1 * n.m22() + a2 * n.m32() + a3 * n.m42();
      t.m43() = a0 * n.m13() + a1 * n.m23() + a2 * n.m33() + a3 * n.m43();
      t.m44() = a0 * n.m14() + a1 * n.m24() + a2 * n.m34() + a3 * n.m44();
	   return t;
   }

   /**
    * Matrix multiplication.  Multiplies the current matrix by the matrix n
    * ( m = m n ) and stores the result in m.
    * @param   n  Matrix to multiply the current matrix by
    * @return  Returns the address of the current matrix.
    */
   Matrix4x4& operator *= (const Matrix4x4& n)
   {
      *this = *this * n;
      return *this;
   }

   /**
    * Matrix multiplication by a scalar.  Multiplies the current matrix 
    * by the scalar s.
    * @param  s   Scalar to multiply the current matrix by
    * @return     Returns the address of the current matrix.
    */
   Matrix4x4 & operator *= (const float s)
   {
      for (unsigned int i = 0; i < 16; i++)
		   a[i] *= s;

	   return *this;
   }

   /**
    * Transforms a coordinate by the matrix.
    * @param   v  Homogeneous point.
    * @return  Returns the transformed homogeneous coordinate position.
    */
   HPoint3 operator *(const HPoint3& v) const
   {
      return HPoint3((a[0] * v.x + a[4] * v.y + a[8]  * v.z + a[12] * v.w),
               (a[1] * v.x + a[5] * v.y + a[9]  * v.z + a[13] * v.w),
		         (a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14] * v.w),
		         (a[3] * v.x + a[7] * v.y + a[11] * v.z + a[15] * v.w));
   }

   /**
    * Transforms a coordinate by the matrix.  Returns a homogeneous
    * coordinate position.  Assumes the w coordinate is 1.
    * @param   v  3D point to transform.
    * @return  Returns the transformed point.
    */
   HPoint3 operator *(const Point3& v) const
   {
      return HPoint3((a[0] * v.x + a[4] * v.y + a[8]  * v.z + a[12]),
                     (a[1] * v.x + a[5] * v.y + a[9]  * v.z + a[13]),
                     (a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14]),
                     1.0f);
   }

   /**
    * Transforms a vector (normal or direction) by the matrix. 
    * Only the upper 3x3 portion of the matrix is used (no translation).
    * @param   v  3D vector to transform.
    * @return  Returns the transformed direction.
    */
   Vector3 operator *(const Vector3& v) const
   {
      return Vector3((a[0] * v.x + a[4] * v.y + a[8]  * v.z),
                     (a[1] * v.x + a[5] * v.y + a[9]  * v.z),
                     (a[2] * v.x + a[6] * v.y + a[10] * v.z));
   }

   /**
    * Transposes the current matrix.
    * @return   Returns the address of the current matrix.
    */
   Matrix4x4& Transpose(void)
   {
      Matrix4x4 t = GetTranspose();
      *this = t;
      return *this;
   }

   /**
    * Calculates the transpose of the current 4x4 matrix and returns it.
    * The current matrix is unchanged.
    * @return   Returns the transpose of the current matrix.
    */
   Matrix4x4 GetTranspose() const
   {
      Matrix4x4 t;
      t.m11() = m11();
      t.m12() = m21();
      t.m13() = m31();
      t.m14() = m41();
      t.m21() = m12();
      t.m22() = m22();
      t.m23() = m32();
      t.m24() = m42();
      t.m31() = m13();
      t.m32() = m23();
      t.m33() = m33();
      t.m34() = m43();
      t.m41() = m14();
      t.m42() = m24();
      t.m43() = m34();
      t.m44() = m44();
      return t;
   }

   // The following convenience methods allow creation of a composite 
   // modeling transfomation. Each method postmultiplies the curent
   // matrix (similar to OpenGL)

   /**
    * Applies a translation to the current transformation matrix.
    * Instead of doing a standard matrix multiplication call, we just reset
    * the elements of the matrix that will change due to the translation.
	 * @param	x	   x translation
	 * @param	y	   y translation
	 *	@param	z	   Z translation
	 */
	void Translate(const float x, const float y, const float z)
   {
      // Set the translation matrix elements.
      m14() += m11() * x + m12() * y + m13() * z;
	   m24() += m21() * x + m22() * y + m23() * z;
	   m34() += m31() * x + m32() * y + m33() * z;
	   m44() += m41() * x + m42() * y + m43() * z;
   }

   /**
    * Applies a scaling to the current transformation matrix.
	 * @param	x	   x scaling
	 * @param	y	   y scaling
	 *	@param	z	   Z scaling
	 */
	void Scale(const float x, const float y, const float z)
   {
      // Set the scale matrix elements.
      m11() *= x;
      m21() *= x;	
	   m31() *= x;	
      m41() *= x;
      m12() *= y;	
      m22() *= y;	
      m13() *= z;
      m32() *= y;	
      m42() *= y;	
      m23() *= z;
      m33() *= z;	
      m43() *= z;
   }

   /**
    * Performs a counterclockwise rotation about the specified axis. 
    * NOTE: rotations along the x, y, or z axis are treated as special cases
    * (i.e. the do not fall through to the general rotation case).
    * @param   angle    Angle (degrees) for the rotation.
    * @param   x        x coordinate of the axis of rotation
    * @param   y        y coordinate of the axis of rotation
    * @param   z        z coordinate of the axis of rotation
    */
   void Rotate(const float angle, const float x, const float y, 
               const float z)
   {
      // The rotation matrix
      Matrix4x4 r;

      // Handle simple cases of rotation about a single axis
      if (x == 1.0f && y == 0.0f && z == 0.0f)
      {
         float radians = degreesToRadians(angle);
	      float cosa = cosf(radians);
	      float sina = sinf(radians);
	      r.m22() = cosa;		r.m23() = -sina;
	      r.m32() = sina;		r.m33() = cosa;
      }
      else if (x == 0.0f && y == 0.0f && z == 1.0f)
      {
         float radians = degreesToRadians(angle);
	      float cosa = cosf(radians);
	      float sina = sinf(radians);
         r.m11() = cosa;		r.m12()  = -sina;
	      r.m21() = sina;		r.m22()  = cosa;
      }
      else if (x == 0.0f && y == 1.0f && z == 0.0f)
      {
         float radians = degreesToRadians(angle);
	      float cosa = cosf(radians);
	      float sina = sinf(radians);
         r.m11() =  cosa;	r.m13() = sina;
	      r.m31() = -sina;	r.m33() = cosa;
      }
      else
      {
         // Set up the standard rotation using quaternions
         // Scalar part
         float s = cosf(degreesToRadians(angle * 0.5f));

         // Vector part: a normalized direction vector
         Vector3 v(x, y, z);
         v.Normalize();
         v *= sinf(degreesToRadians(angle * 0.5f));
         float a = v.x;
         float b = v.y;
         float c = v.z;

         // Set the composite rotation matrix
         r.m11() = 1.0f - 2.0f * b * b - 2.0f * c * c;
         r.m12() = 2.0f * a * b - 2.0f * s * c;
         r.m13() = 2.0f * a * c + 2.0f * s * b;
         r.m21() = 2.0f * a * b + 2.0f * s * c;
         r.m22() = 1.0f - 2.0f * a * a - 2.0f * c * c;
         r.m23() = 2.0f * b * c - 2.0f * s * a;
         r.m31() = 2.0f * a * c - 2.0f * s * b;
         r.m32() = 2.0f * b * c + 2.0f * s * a;
         r.m33() = 1.0f - 2.0f * a * a - 2.0f * b * b;
         r.m44() = 1.0f;
      }

      // Postmultiply the current matrix
      *this *= r;
   }

   /**
    * Calculates the inverse of the current 4x4 matrix and returns it.
    * @return  Returns the inverse of the current matrix.
    */
   Matrix4x4 Inverse(void) const
   {
	   int j, k;
	   int ind;
	   float v1, v2;
	   Matrix4x4 t = *this;
      Matrix4x4 b;
	   for (int i = 0; i < 4; i++)
	   {
         // Find pivot
		   v1 = t.m(i, i);
		   ind = i;
		   for (j = i + 1; j < 4; j++)
		   {
			   if (fabs(t.m(j, i)) > fabs(v1))
			   {
				   ind = j;
				   v1 = t.m(j, i);
			   }
		   }

         // Swap columns
		   if (ind != i)
		   {
			   for (j = 0; j < 4; j++)
			   {
				   v2 = b.m(i, j);
				   b.m(i, j) = b.m(ind, j);
				   b.m(ind, j) = v2;
				   v2 = t.m(i, j);
				   t.m(i, j) = t.m(ind, j);
				   t.m(ind, j) = v2;
			   }
		   }

		   // The matrix is singular (has no inverse), set the inverse
		   //    to the identity matrix.
		   if (v1 == 0.0f)
		   {
            extern void logmsg(const char *message, ...);
			   logmsg("InvertMatrix: Singular matrix");
            b.SetIdentity();
			   return b;
		   }

		   for (j = 0; j < 4; j++)
		   {
			   t.m(i, j) /= v1;
			   b.m(i, j) /= v1;
		   }

         // Eliminate column
		   for (j = 0; j < 4; j++)
		   {
 			   if (j == i)
				   continue;

			   v1 = t.m(j, i);
			   for (k = 0; k < 4; k++)
			   {
				   t.m(j, k) -= t.m(i, k) * v1;
				   b.m(j, k) -= b.m(i, k) * v1;
			   }
		   }
	   }
	   return b;
   }

   /**
    * Logs a message followed by the matrix.
    *
    * @param   str   String to print to log file
    */  
   void Log(const char* str) const
   {
      extern void logmsg(const char *message, ...);
	   logmsg("  %s", str);
	   logmsg("%.3f %.3f %.3f %.3f", m11(), m12(), m13(), m14());
	   logmsg("%.3f %.3f %.3f %.3f", m21(), m22(), m23(), m24());
	   logmsg("%.3f %.3f %.3f %.3f", m31(), m32(), m33(), m34());
	   logmsg("%.3f %.3f %.3f %.3f", m41(), m42(), m43(), m44());
   }

private:
   // Storage for a 4x4 array
   float a[16];
};

#endif
