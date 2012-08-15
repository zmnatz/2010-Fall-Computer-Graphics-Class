//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Plane.h
//	Purpose: Structures to hold definition of a plane in 3 space.
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//============================================================================

#ifndef __PLANE_H__
#define __PLANE_H__

#include <math.h>

struct Plane
{
   float a, b, c, d;     // Components of the plane equation

   /**
    * Default constructor
    */
   Plane()
   {
   }

   /**
    * Constructor given a normal vector and a point on the plane.
    * @param  p  Point on the plane
    * @param  n  Normal to the plane
    */
   Plane(const Point3& p, const Vector3& n)
   {
      Set(p, n);
   }

   /**
    * Constructor given 3 points on the plane. Assume the points are
    * in counter-clockwise order to determine the outward direction.
    * @param  p1   1st point on the plane
    * @param  p2   2nd point on the plane
    * @param  p3   3rd point on the plane
    */
   Plane(const Point3& p1, const Point3& p2,const Point3& p3) 
   {
	   Set(p1,Vector3(p1,p2).Cross(Vector3(p1,p3)));
   }

   /**
    * Set the plane given a normal vector and a point on the plane
    * @param  p  Point on the plane
    * @param  n  Normal to the plane
    */
   void Set(const Point3& p, const Vector3& n)
   {
      //DONE: Student to define
	   a=n.x;
	   b=n.y;
	   c=n.z;
	   d=-1*(p.x+p.y+p.z);
   }

   /**
    * Normalize the plane equation - divides through by the magnitude of the
    * normal vector to make sure the normal is unit length.
    */
   void Normalize()
   {
	   Vector3 n = GetNormal();
	   a=n.x;
	   b=n.y;
	   c=n.z;
      // DONE: Student to define
   }

   /**
    * Solves the plane equation using the supplied point. The sign of the
    * value indicates on which side of the plane the point lies. If the plane
    * is constructed with a unit normal, then the value is the signed distance
    * of the point to the plane.
    * @param  p  Point to solve the plane equation with.
    * @return  Returns the solution of the plane equation given an input point
    */
   float Solve(const Point3& p) const
   {
      // DONE: Student to define
	   return p.x*a+p.y*b+p.z*c-d;
   }

   /**
    * Returns a normal to the plane
    * @return  Returns a normal to the plane.
    */
   Vector3 GetNormal() const
   {
	   // DONE: Student to define
		Vector3 n(a,b,c);
		return n.Normalize();
   }
};

#endif
