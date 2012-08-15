//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    geometry.h
//	Purpose: 3D Vector class. 
//          Student should include "geometry.h" to get all class definitions
//          included in proper order.
//
//============================================================================

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <stdarg.h>
#include <math.h>

/**
 * 3D vector class
 */
struct Vector3
{
public:
	float x;
   float y;
   float z;

	/**
    * Default constructor
    */
	Vector3(void) : x(0.0f), y(0.0f), z(0.0f) { }

   /**
    * Constructor given a point.  Essentially a vector from the
    * origin to the point.
    * @param   p  Point.
    */
	Vector3(const Point3& p) : x(p.x), y(p.y), z(p.z) { }

   /**
    * Constructor given 3 components of the vector.
    * @param   ix   x component of the vector.
    * @param   iy   y component of the vector.
    * @param   iz   z component of the vector.
    */
  	Vector3(const float ix, const float iy, const float iz)
               : x(ix), y(iy), z(iz) { }

	/**
    * Constructor from one point to another.
    * @param   from  Point at origin of the vector.
    * @param   to    Point at end of vector
    */
	Vector3(const Point3& from, const Point3& to) : 
		x(to.x-from.x), y(to.y-from.y),z(to.z-from.z) {}
		// DONE: Student to define
      
	/**
    * Copy constructor.
    * @param   w  Vector to copy to the new vector.
    */
	Vector3(const Vector3& w) : x(w.x), y(w.y), z(w.z) { }

   /**
    * Assignment operator
    * @param   w  Vector to copy to the current vector.
    * @return  Returns the address of the current vector.
    */
	Vector3& operator = (const Vector3& w)
	{
		x = w.x;
		y = w.y;
		z = w.z;
		return *this;
	}

	/**
    * Destructor
    */
	~Vector3(void) { }
	
   /**
    * Set the current vector to the specified components.
    * @param   ix   x component of the vector.
    * @param   iy   y component of the vector.
    * @param   iz   z component of the vector.
    */
	void Set(const float ix, const float iy, const float iz)
	{
		x = ix;
		y = iy; 
		z = iz; 
	}

   /**
    * Set the vector components to those of a point.  Essentially a 
    * vector from the origin to the point.
    * @param   p  Point.
    */
	void Set(const Point3& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
	}

   /**
    * Set the current vector to be from one point to another.
    * @param   from  Point at origin of the vector.
    * @param   to    Point at end of vector
    */
	void Set(const Point3& from, const Point3& to)
	{
		*this = Vector3(from,to);
      // DONE: Student to define
	}

	/**
    * Creates a new vector that is the current vector plus the
    * specified vector.
    * @param   w  Vector to add to the current vector.
    * @return   Returns the resulting vector.
    */
	Vector3 operator + (const Vector3& w) const
	{
      // DONE: Student to define
		return Vector3(x+w.x, y+w.y,z+w.z);
	}

	/**
    * Adds vector w to the current vector.
    * @param   w  Vector to add to the current vector.
    * @return  Returns the address of the current vector.
    */
	Vector3& operator += (const Vector3& w)
	{
		*this = *this+w;
		return *this;
	}

	/**
    * Creates a new vector that is the current vector minus the
    * specified vector.
    * @param   w  Vector to subtract from the current vector.
    * @return   Returns the resulting vector.
    */
	Vector3 operator - (const Vector3& w) const
	{
      // DONE: Student to define
		return Vector3(x-w.x,y-w.y,z-w.z);
	}

	/**
    * Subtracts vector w from the current vector.
    * @param   w  Vector to subtract from the current vector.
    * @return  Returns the address of the current vector.
    */
	Vector3& operator -= (const Vector3& w)
	{
		// DONE: Student to define
		*this=*this-w;
		return *this;
	}

	/**
    * Creates a new vector that is the current vector multiplied
    * with the specified scalar.
    * @param   scalar   Scalar to muliply the vector with.
    * @return  Returns the resulting vector
    */
	Vector3 operator * (const float scalar) const
	{
		// DONE: Student to define
		return Vector3(x*scalar,y*scalar,z*scalar);
	}

	/**
    * Multiplies the current vector by a scalar
    * @param   scalar   Scalar to muliply the vector with.
    * @return  Returns the address of the current vector.
    */
	Vector3& operator *= (const float scalar)
	{
		// DONE: Student to define
		*this = *this * scalar;
		return *this;
	}

	/**
    * Equality operator.
    * @param   w  Vector to test if equal to the current vector.
    * @return  Returns true if vector w equals the current vector,
    *          false otherwise.
    */
	bool operator == (const Vector3& w) const
	{
		// DONE: Student to define
		return x==w.x && y==w.y && z==w.z;
	}

	/**
    * Computes the dot product of the current vector with the 
    * specified vector.
    * @param   w  Vector
    * @return  Returns the dot product (a scalar).
    */
	float Dot(const Vector3& w) const
	{
		// DONE: Student to define
		return x*w.x + y*w.y + z*w.z;
	}

	/**
    * Computes the cross product of current vector with w
    * @param   w  Vector to take the cross product with (current X w)
    * @return  Returns the resulting vector.
    */
	Vector3 Cross(const Vector3& w) const
	{
		// DONE: Student to define
		return Vector3(y*w.z-z*w.y,x*w.z-z*w.x,x*w.y-y*w.x);
	}

	/**
    * Computes the norm (length) of the current vector.
    * @return  Returns the length of the vector.
    */
	float Norm(void) const
	{
		// DONE: Student to define
		return sqrt(NormSquared());
	}

	/**
    * Computes the squared norm of a vector
    * (Useful when absolute distance is not required)
    * @return  Returns the length squared of the vector.
    */
	float NormSquared(void) const
	{
		// DONE: Student to define
		return Dot(*this);
	}

	/**
    * Normalizes the vector.
    * @return  Returns the address of the current vector.
    */
	Vector3& Normalize(void)
	{
		// DONE: Student to define
		*this *= 1/Norm();
		return *this;
	}

	/**
    * Calculates the component of the current vector along the 
    * specified vector
    * @param   w  Vector to determine component along.
    * @return  Returns the component of the current vector along w.
    */
	float Component(const Vector3& w) const
	{
		// DONE: Student to define
		return Dot(w)/w.Norm();
	}

	/**
    * Creates a new vector that is the projection of the current 
    * vector along the specified vector.
    * @param   w  Vector to determine projection along.
    * @return  Returns the new vector.
    */
	Vector3 Projection(const Vector3& w) const
	{
      // DONE: Student to define
      return w * (Dot(w) / w.NormSquared());
	}

	/**
    * Calculates the angle (radians) between the current vector and 
    * the specified vector.
    * @param   w  Vector to determine angle from current vector.
    * @return  Returns the angle in radians between the two vectors.
    */
	float AngleBetween(const Vector3& w) const
	{
		// DONE: Student to define
		return acos(Dot(w)/Norm()/w.Norm());
	}

   /**
    * Reflects the current vector given a normal to the reflecting surface.
    * Assumes the normal is unit length.  Note that if done properly the
    * magnitude of the reflected vector will equal the magnitude of the
    * incoming vector.
    * @param   normal   unit length normal to the plane where reflection occurs
    * @return  Returns the reflected vector
    */
   Vector3 Reflect(const Vector3& normal)
   {
	   // DONE: Student to define
		Vector3 reverse = *this*-1;
	   return reverse + (reverse.Projection(normal)-reverse)*2;
   }
};


#endif
