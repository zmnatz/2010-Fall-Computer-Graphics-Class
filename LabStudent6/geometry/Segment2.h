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
      // Construct vector v (AB) and w (AP)
      Vector2 v(A, B);
      Vector2 w(A, P);

      // Numerator of the component of w onto v. If <= 0 then A 
      // is the closest point. By separating into the numerator
      // and denominator of the component we avoid a division unless
      // it is necessary.
      float n = w.Dot(v);
      if (n <= 0.0f)
      {
         closestPt = A;
         return w.Norm();
      }

      // Get the denominator of the component.  If the component >= 1
      // (d <= n) then point B is the closest point
      float d = v.Dot(v);
      if (d <= n)
      {
         closestPt = B;
         return Vector2(B, P).Norm();
      }

      // Closest point is along the segment. The point is the projection of
      // w onto v.
      closestPt = A + v * (n / d);
      return Vector2(closestPt, P).Norm();
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
      // Construct vectors
      Vector2 b = B - A;
      Vector2 d = segment.B - segment.A;

      // Set 2D perpendicular vector to d
      Vector2 dp(d.y, -d.x);

      // Check if denominator equals 0 (lines are parallel)
      float dtb = dp.Dot(b);
      if (dtb == 0.0f)
         return false;

      // Solve for the parameter t. If outside the range [0,1] then no intersection
      // occurs and we do not have to solve for u or form perp. to b.
      Vector2 c = segment.A - A;
      float t = dp.Dot(c) / dtb;
      if (t < 0.0f || t > 1.0f)
         return false;
      
      // Solve for the parameter u. No intersect occurs if outside the range [0,1]
      Vector2 bp(b.y, -b.x);
      float u = bp.Dot(c) / dtb;
      if (u < 0.0f || u > 1.0f)
         return false;

      // An intersect occurs.  Set the intersect point and return true
      intersectPt = A + b * t;
      return true;
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
