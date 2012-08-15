//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Point3.h
//	Purpose: Structures to hold 3D coordinates and homogenous coordinates
//
//============================================================================

#ifndef __POINT3_H__
#define __POINT3_H__

#include <math.h>

// Forward references
struct Vector3;

/**
 * 3D Point (cartesian)
 */
struct Point3
{
	float x;
   float y; 
   float z;

	/**
    * Default constructor
    */
	Point3(void) : x(0.0f), y(0.0f), z(0.0f) { }

   /**
    * Constructor with initial values for x,y,z.
    * @param   ix   x coordinate position.
    * @param   iy   y coordinate position.
    * @param   iz   z coordinate position.
    */
   Point3(const float ix, const float iy, const float iz)
               : x(ix), y(iy), z(iz) { }

	/**
    * Copy constructor.
    * @param   p   Point to copy to the new point.
    */
	Point3(const Point3& p) : x(p.x), y(p.y), z(p.z) { }

   /**
    * Convert a homogeneous coordinate into a cartesian coordinate.
    * @param   p   Homogenous point
    */
	Point3(const HPoint3& p) 
   { 
      // Needed later
   }
   
   /**
    * Assignment operator
    * @param   p   Point to assign to this point.
    * @return   Returns the address of this point.
    */
	Point3& operator = (const Point3& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	/**
    * Destructor
    */
	~Point3(void) { }

   /**
    * Set the coordinate components to the specified values.
    * @param   ix   x coordinate position.
    * @param   iy   y coordinate position.
    * @param   iz   z coordinate position.
    */
	void Set(const float ix, const float iy, const float iz)
	{
		x = ix;
		y = iy;
		z = iz;
	}

   /**
    * Equality operator.
    * @param   p  Point to compare to the current point.
    * @return  Returns true if two points are equal, false otherwise.
    */
	bool operator == (const Point3& p) const
	{
		return (x == p.x && y == p.y && z == p.z);
	}

   // The following methods are defined below after Vector3

   /**
    * Add a vector to the current point.
    * @param   v  Vector to add to the current point.
    * @return  Returns a new point: the result of the current point
    *          plus the specified vector.
    */
	Point3 operator + (const Vector3& v) const;

   /**
    * Subtract a vector from the current point.
    * @param   v  Vector to subtract from the current point.
    * @return  Returns a new point: the result of the current point
    *          minus the specified vector.
    */
	Point3 operator - (const Vector3& v) const;
   
   /**
    * Subtraction of a point from the current point.
    * @param   Point to subtract from the current point.
    * @return  Returns a vector.
    */
	Vector3 operator - (const Point3& p) const;
};

#endif
