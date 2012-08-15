//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Segment.h
//	Purpose: 3D Line segment class.
//          Student should  include "geometry.h" to get all class definitions
//          included in proper order.
//
//============================================================================

#ifndef __SEGMENT3_H__
#define __SEGMENT3_H__

#include <math.h>
#include <vector>

/**
 * Line segment in 3D
 */
struct LineSegment3
{
   Point3 A;
   Point3 B;

   /**
    * Default constructor.
    */
   LineSegment3(void)
   {
      A.Set(0.0f, 0.0f, 0.0f);
      B.Set(0.0f, 0.0f, 0.0f);
   }

   /**
    * Constructor given 2 points.
    * @param   p1    First point of the segment.
    * @param   p2    Second point of the segment.
    */
   LineSegment3(const Point3& p1, const Point3& p2)
   {
      A = p1;
      B = p2;
   }

   /**
    * Finds the distance of a specified point from the line segment 
    * and the closest point on the segement to the specified point.
    * @param   P           Test point.
    * @param   closestPt   (Return) Closest point on the segment to c.
    * @return  Returns the distance from P to the closest point on
    *          the segment.
    */
   float Distance(const Point3& P, Point3& closestPt) const
   {
		Vector3 ab(A,B);	
		Vector3 ap(A,P);
		Vector3 proj = ap.Projection(ab);
		float angleBetween = radiansToDegrees(proj.AngleBetween(ab));

		if(radiansToDegrees(proj.AngleBetween(ab))==0)	{	//Projection is on the positive side of vector ab
			if(proj.NormSquared() <= ab.NormSquared())		//Projection is within length of segment
				closestPt = A+proj;							//Projection of ap onto ab is closest point
			else											//Projection extends past length of segment
				closestPt = B;								//B (end of segment) is closest point
			return Vector3(closestPt,P).Norm();				//Return distance between P and closest point
		}else{												//Projection is on the negative side of the vector ab
			closestPt = A;									//A (start of segment) is the closest point
			return ap.Norm();
		}
      // DONE: Student to define
   }
};

#endif
