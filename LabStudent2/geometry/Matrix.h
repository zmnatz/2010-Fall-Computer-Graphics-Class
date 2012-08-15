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
		// Student to define
	}

	/**
    * Copy constructor
    * @param  n  Matrix to copy
    */
	Matrix4x4(const Matrix4x4& n)
	{
      // Student to define
	}

   /**
    * Assignment operator
    * @param   n  Matrix to assign to this matrix
    * @return  Returns the address of this matrix.
    */
	Matrix4x4& operator = (const Matrix4x4& n)
	{
      // Student to define
		return *this;
	}

   /**
    * Equality operator
    * @param   n  Matrix to test for equality with this matrix.
    * @return  Returns true if hte matrices are equal, false otherwise..
    */
	bool operator == (const Matrix4x4& n) const
	{
      // Student to define
      return false;
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
      // Student to define
   }

   /**
    * Gets the matrix (can be passed to OpenGL)
    * @return   Returns the elements of this matrix in column order.
    */
   const float* Get(void) const
   {
      // Student to define
      return a;
   }

   // Read-only access functions
   float m11(void) const { return a[0]; } // Student to define
	float m12(void) const { return a[0]; } // Student to define
	float m13(void) const { return a[0]; } // Student to define
	float m14(void) const { return a[0]; } // Student to define
	float m21(void) const { return a[0]; } // Student to define
	float m22(void) const { return a[0]; } // Student to define
	float m23(void) const { return a[0]; } // Student to define
	float m24(void) const { return a[0]; } // Student to define
	float m31(void) const { return a[0]; } // Student to define
	float m32(void) const { return a[0]; } // Student to define
	float m33(void) const { return a[0]; } // Student to define
	float m34(void) const { return a[0]; } // Student to define
	float m41(void) const { return a[0]; } // Student to define
	float m42(void) const { return a[0]; } // Student to define
	float m43(void) const { return a[0]; } // Student to define
	float m44(void) const { return a[0]; } // Student to define

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
      // Student to define
      return 0.0f;
   }

   // Read-write access functions
   float& m11(void) { return a[0]; }// Student to define
   float& m12(void) { return a[0]; }// Student to define
   float& m13(void) { return a[0]; }// Student to define
   float& m14(void) { return a[0]; }// Student to define
   float& m21(void) { return a[0]; }// Student to define
   float& m22(void) { return a[0]; }// Student to define
   float& m23(void) { return a[0]; }// Student to define
	float& m24(void) { return a[0]; }// Student to define
	float& m31(void) { return a[0]; }// Student to define
	float& m32(void) { return a[0]; }// Student to define
	float& m33(void) { return a[0]; }// Student to define
	float& m34(void) { return a[0]; }// Student to define
	float& m41(void) { return a[0]; }// Student to define
	float& m42(void) { return a[0]; }// Student to define
	float& m43(void) { return a[0]; }// Student to define
	float& m44(void) { return a[0]; }// Student to define 

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
      // Student to define
      return a[0];
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
	   // Student to define
	   return *this;
   }

   /**
    * Transforms a coordinate by the matrix.
    * @param   v  Homogeneous point.
    * @return  Returns the transformed homogeneous coordinate position.
    */
   HPoint3 operator *(const HPoint3& v) const
   {
      // Student to define
      return HPoint3(0.0f, 0.0f, 0.0f, 1.0f);
   }

   /**
    * Transforms a coordinate by the matrix.  Returns a homogeneous
    * coordinate position.  Assumes the w coordinate is 1.
    * @param   v  3D point to transform.
    * @return  Returns the transformed point.
    */
   HPoint3 operator *(const Point3& v) const
   {
      // Student to define
      return HPoint3(0.0f, 0.0f, 0.0f, 1.0f);
   }

   /**
    * Transforms a vector (normal or direction) by the matrix. 
    * Only the upper 3x3 portion of the matrix is used (no translation).
    * @param   v  3D vector to transform.
    * @return  Returns the transformed direction.
    */
   Vector3 operator *(const Vector3& v) const
   {
      // Student to define
   }

   /**
    * Transposes the current matrix.
    * @return   Returns the address of the current matrix.
    */
   Matrix4x4& Transpose(void)
   {
	   // Student to define
      return *this;
   }

   /**
    * Calculates the transpose of the current 4x4 matrix and returns it.
    * The current matrix is unchanged.
    * @return   Returns the transpose of the current matrix.
    */
   Matrix4x4 GetTranspose() const
   {
	   // Student to define
      Matrix4x4 t;
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
      // Student to define
   }

   /**
    * Applies a scaling to the current transformation matrix.
	 * @param	x	   x scaling
	 * @param	y	   y scaling
	 *	@param	z	   Z scaling
	 */
	void Scale(const float x, const float y, const float z)
   {
     	// Student to define
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
      // Student to define
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
