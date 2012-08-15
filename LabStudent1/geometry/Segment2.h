//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Segment.h
//	Purpose: 2D Line segment class. Includes clipping code. 
//          Student should  include "geometry.h" to get all class definitions
//          included in proper order.
//
//============================================================================

#ifndef __SEGMENT2_H__
#define __SEGMENT2_H__

#include <math.h>
#include <vector>

/**
 * Rectangle
 */
struct CRectangle
{
   float left;
   float right;
   float bottom;
   float top;
};


/**
 * Line segment in 2D
 */
struct LineSegment2
{
   Point2 A;
   Point2 B;

   /**
    * Default constructor.
    */
   LineSegment2(void)
   {
      A.Set(0.0f, 0.0f);
      B.Set(0.0f, 0.0f);
   }

   /**
    * Constructor given 2 points.
    * @param   p1    First point of the segment.
    * @param   p2    Second point of the segment.
    */
   LineSegment2(const Point2& p1, const Point2& p2)
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
   float Distance(const Point2& P, Point2& closestPt) const
   {
		Vector2 ab(A,B);	
		Vector2 ap(A,P);
		float comp = ap.Component(ab);

		if(comp<0){
			closestPt=A;
			return ap.Norm();
		}else if(comp<1){
			closestPt = A+ab.Normalize()*comp;
			return comp;
		}else{
			closestPt = B;
			return Vector2(closestPt,P).Norm();
		}
	   // DONE: Student to define
   }

   /**
    * Determines if the current segment intersects the specified segment.
    * If an intersect occurs the intersectPt is determined.  Note: the
    * case where the lines overlap is not considered.
    * @param   segment        Segment to determine intersection with.
    * @param   intersectPt    (OUT) Intersection point.
    * @return   Returns true if an intersection exists, false if not.
    */
   bool Intersect(const LineSegment2& segment, Point2& intersectPt)
   {
		Vector2 v(A,B);
		Vector2 vSeg(segment.A,segment.B);
		float segNorm = vSeg.Norm();
		float distToSegA = Distance(segment.A,Point2());
		float distToSegB = Distance(segment.B,Point2());
		//Go over simple cases first to avoid complex calculation
		if(distToSegB>segNorm || distToSegB>segNorm)	//Segment's A or B is further from *this than its length
			return false;								//Thus it can't intersect
		else if(distToSegA==0){
			intersectPt=segment.A;
			return true;
		}else if(distToSegB==0){
			intersectPt=segment.B;
			return true;
		}else{
			
		}
   }

   /**
    * Clips the line segment to a specified convex polygon.
    * @param  poly  A counter-clockwise oriented polygon.
    * @returns   Returns the clipped segment.
    */
   bool ClipToPolygon(std::vector<Point2>& poly, LineSegment2& clipSegment)
   {
      // Later...
      return true;
   }

   /**
    * Clips the line segment to a specified rectangle.
    * @param  poly  A counter-clockwise oriented polygon.
    * @returns   Returns the clipped segment.
    */
   bool ClipToRectangle(const CRectangle& r, LineSegment2& clip)
   {
      // Later...
      return true;
   }
};

#endif
