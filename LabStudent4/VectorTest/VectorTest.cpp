//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.461 Principles of Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    VectorTest.cpp
//	Purpose: Test the vector library.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>

#include "geometry/geometry.h"

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("VectorTest.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}


int main(int argc, char* argv[])
{
   logmsg("Vector Test");
   logmsg("\n2D Operations\n");

   // Construct some 2D points
   Point2 a(-3.0f, 4.0f);
   Point2 b(-3.0f,  0.0f);
   Point2 c(-2.0f,  1.0f);
   logmsg("Point a (%.2f, %.2f)", a.x, a.y);
   logmsg("Point b (%.2f, %.2f)", b.x, b.y);
   logmsg("Point c (%.2f, %.2f)", c.x, c.y);

   // Construct a vector from point a to point b
   Vector2 v(a, b);
   logmsg("Vector v (%.2f, %.2f)", v.x, v.y);

   // Get the length of v
   logmsg("Norm of v = %.2f", v.Norm());

   // Create a unit vector from v
   Vector2 u = v.Normalize();
   logmsg("Unit vector of v = (%.2f, %.2f)", u.x, u.y);
   logmsg("Note: v is now a unit vector (result of Normalize())");

   // Create a vector w from point b to point c
   Vector2 w = c - b;
   logmsg("Vector w (%.2f, %.2f)", w.x, w.y);
   w.Normalize();
   logmsg("Unit vector of w = (%.2f, %.2f)", w.x, w.y);

   // Create vectors t0 and t1 that are in the opposite direction of w 
   // and have a length of 5. (Tests both * operators)
   Vector2 t0 = w * -5.0f;
   logmsg("Vector t0 (%.2f, %.2f)", t0.x, t0.y);
   Vector2 t1 = -5.0f * w;
   logmsg("Vector t1 (%.2f, %.2f)", t1.x, t1.y);

   // Add vector v and vector w
   Vector2 wv = v + w;
   logmsg("Vector v + w = (%.2f, %.2f)", wv.x, wv.y);

   // Dot product of v and w
   float dot = v.Dot(w);
   logmsg("Dot Product of v and w is %f", dot);

   // Find the angle in degrees between v and w
   logmsg("Angle between v and w = %.2f degrees", radiansToDegrees(v.AngleBetween(w)));

   // Add vector t0 to point c
   Point2 d = c + t0;
   logmsg("Point d: (%.2f %.2f)", d.x, d.y);

   // Find the projection of vector w onto vector v
   Vector2 wOnv = w.Projection(v);
   logmsg("Projection of w onto v is (%.2f %.2f)", wOnv.x, wOnv.y);

   // Find the reflection of vector t0 off the x axis
   Vector2 n1(0.0f, 1.0f);
   Vector2 r = t0.Reflect(n1);
   logmsg("Reflection of v off the x-axis is %f %f", r.x, r.y);

   // Construct a line segment from point a to point b
   LineSegment2 seg(a, b);

   // Find the distance of point c from line segment ab
   Point2 closestPt;
   float dist = seg.Distance(c, closestPt);
   logmsg("Distance of point c from line segment ab is %.2f", dist);
   logmsg("Closest Point is (%.2f, %.2f)", closestPt.x, closestPt.y);
  
   // New point e
   Point2 e(-4.0f, 10.0f);
   dist = seg.Distance(e, closestPt);
   logmsg("Point e (%.2f, %.2f)", e.x, e.y);
   logmsg("Distance of point e from line segment ab is %.2f", dist);
   logmsg("Closest Point is (%.2f, %.2f)", closestPt.x, closestPt.y);

   // New point f
   Point2 f(6.0f, -1.0f);
   dist = seg.Distance(f, closestPt);
   logmsg("Point f (%.2f, %.2f)", f.x, f.y);
   logmsg("Distance of point f from line segment ab is %.2f", dist);
   logmsg("Closest Point is (%.2f, %.2f)", closestPt.x, closestPt.y);

   /************  3D ***************/

   // Construct three points 
   Point3 a3( 4.0f,  4.0f,  2.0f);
   Point3 b3( 1.0f,  3.0f, -4.0f);
   Point3 c3(10.0f, -2.0f,  4.0f);
   logmsg("\n3D Operations\n");
   logmsg("Point a3 (%.2f, %.2f, %.2f)", a3.x, a3.y, a3.z);
   logmsg("Point b3 (%.2f, %.2f, %.2f)", b3.x, b3.y, b3.z);
   logmsg("Point c3 (%.2f, %.2f, %.2f)", c3.x, c3.y, c3.z);

   // Construct a vector from point a3 to point b3
   Vector3 v3(a3, b3);
   logmsg("Vector v3 (%.2f, %.2f, %.2f)", v3.x, v3.y, v3.z);

   // Get the length of v3
   logmsg("Norm of v = %.2f", v3.Norm());

   // Create a unit vector from v2
   Vector3 u3 = v3.Normalize();
   logmsg("Unit vector of v3 = (%.2f, %.2f, %.2f)", u3.x, u3.y, u3.z);
   logmsg("Note: v3 is now a unit vector (result of Normalize())");

   // Create a vector t that is in the opposite direction of u 
   // and has a length of 10. (Tests both * operators)
   Vector3 t3 = u3 * -10.0f;
   logmsg("Vector t3 (%.2f, %.2f, %.2f)", t3.x, t3.y, t3.z);

   Vector3 t31 = -10.0f * u3;
   logmsg("Vector t31 (%.2f, %.2f, %.2f)", t31.x, t31.y, t31.z);

   // Create a vector from Point a3 to point c3
   Vector3 w3 = c3 - a3;
   logmsg("Vector w3 (%.2f, %.2f, %.2f)", w3.x, w3.y, w3.z);

   // Add vector v and vector w
   Vector3 wv3 = v3 + w3;
   logmsg("Vector v3 + w3 = (%.2f, %.2f, %.2f)", wv3.x, wv3.y, wv3.z);

   // Dot product of v3 and w3
   float dot3 = v3.Dot(w3);
   logmsg("Dot Product of v3 and w3 is %f", dot3);

   // Find the angle in degrees between v and w
   logmsg("Angle between v3 and w3 = %.2f degrees", radiansToDegrees(v3.AngleBetween(w3)));

   // Add vector t to point c
   Point3 d3 = c3 + t3;
   logmsg("Point d3: (%.2f %.2f %.2f)", d3.x, d3.y, d3.z);

   // Find the projection of vector w onto vector v
   Vector3 w3Onv3 = w3.Projection(v3);
   logmsg("Projection of w3 onto v3 is (%.2f %.2f %.2f)", w3Onv3.x, w3Onv3.y, w3Onv3.z);

   // Construct a line segment from point a to point b
   LineSegment3 seg3(a3, b3);
   
   // Find the distance of point c from line segment ab
   Point3 closestPt3;
   float dist3 = seg3.Distance(c3, closestPt3);
   logmsg("Distance of point c3 from line segment ab is %.2f", dist3);
   logmsg("Closest Point is (%.2f, %.2f, %.2f)", closestPt3.x, closestPt3.y, closestPt3.z);

   /************ Planes *************/

   // Construct 3 points (not all in x,y plane!)
   Point3 m( 4.0f, -4.0f,  4.0f);
   Point3 n(-8.0f,  1.0f, -3.0f);
   Point3 o( 2.0f, -3.0f,  2.0f);
   logmsg("\nPlane Test\n");
   logmsg("Point m (%.2f, %.2f, %.2f)", m.x, m.y, m.z);
   logmsg("Point n (%.2f, %.2f, %.2f)", n.x, n.y, n.z);
   logmsg("Point o (%.2f, %.2f, %.2f)", o.x, o.y, o.z);

   // Get the cross product of vector mn and mo
   Vector3 perp = (n-m).Cross(o-m);
   logmsg("Cross Product of mn and mo = %.2f %.2f %.2f", perp.x, perp.y, perp.z);

   // Construct a plane through m,n,o
   Plane plane(m, n, o);
   logmsg("Plane through points m,n,o = %.2f %.2f %.2f %.2f", 
                  plane.a, plane.b, plane.c, plane.d);

   plane.Normalize();
   logmsg("Normalized Plane = %.2f %.2f %.2f %.2f", plane.a, plane.b, plane.c, plane.d);

   // Construct plane with point m and perp
   Plane plane2(m, perp);
   plane2.Normalize();
   logmsg("Normalized Plane with point m and normal perp = %.2f %.2f %.2f %.2f", 
                  plane.a, plane.b, plane.c, plane.d);

   // Check if point n is on the plane
   float s = plane.Solve(n);
   if (s > EPSILON)
      logmsg("Point n is in front of the plane: s = %.2f", s);
   else if (s < -EPSILON)
      logmsg("Point n is behind the plane: s = %.2f", s);
   else  
      logmsg("Point n is on the plane");

   // Check point k against the plane
   Point3 k(5.0, 12.0, 7.0f);
   logmsg("Point k (%.2f, %.2f, %.2f)", k.x, k.y, k.z);
   s = plane.Solve(k);
   if (s > EPSILON)
      logmsg("Point k is in front of the plane: s = %.2f", s);
   else if (s < -EPSILON)
      logmsg("Point k is behind the plane: s = %.2f", s);
   else  
      logmsg("Point k is on the plane");

   // Reflect vector v3 off the plane
   Vector3 r3 = v3.Reflect(plane.GetNormal());
   logmsg("Reflection of v3 off the plane = %.2f, %.2f, %.2f", r3.x, r3.y, r3.z);

   // Create a bounding sphere centered at (0,0,0) with radius 4.0f
   logmsg("\nRay Tests\n");
   Point3 origin(0.0f, 0.0f, 0.0f);
   BoundingSphere sphere(origin, 4.0f);

   // Create a unit length ray starting at (5, 0, 0) in the +x direction and test intersection
   Ray3 ray1(Point3(5.0f, 0.0f, 0.0f), Point3(10.0f, 0.0f, 0.0f), true);
   logmsg("Ray1 Direction: %f %f %f", ray1.d.x, ray1.d.y, ray1.d.z);
   float t = ray1.Intersect(sphere);
   if (t != 0.0f)
   {
      Point3 intersect = ray1.Intersect(t);
      logmsg("   Ray1 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   }
   else
      logmsg("   Ray1 does not intersect sphere");

   // Create a unit length ray starting at (-15, 10, 0) in the +x direction and test intersection
   Ray3 ray2(Point3(-15.0f, 10.0f, 0.0f), Point3(10.0f, 10.0f, 0.0f), true);
   logmsg("Ray2 Direction: %f %f %f", ray2.d.x, ray2.d.y, ray2.d.z);
   t = ray2.Intersect(sphere);
   if (t != 0.0f)
   {
      Point3 intersect = ray2.Intersect(t);
      logmsg("   Ray2 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   }
   else
      logmsg("   Ray2 does not intersect sphere");

   // Create a unit length ray starting at (-15, 2, 0) in the +x direction and test intersection
   Ray3 ray3(Point3(-15.0f, 2.0f, 0.0f), Point3(10.0f, 2.0f, 0.0f), true);
   logmsg("Ray3 Direction: %f %f %f", ray3.d.x, ray3.d.y, ray3.d.z);
   t = ray3.Intersect(sphere);
   if (t != 0.0f)
   {
      Point3 intersect = ray3.Intersect(t);
      logmsg("   Ray3 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   }
   else
      logmsg("   Ray3 does not intersect sphere");

   // Create a unit length ray starting at (2, 2, 0) in the +y direction and test intersection
   Vector3 y(0.0f, 1.0f, 0.0f);
   Ray3 ray4(Point3(2.0f, 2.0f, 0.0f), y);
   logmsg("Ray4 Direction: %f %f %f", ray4.d.x, ray4.d.y, ray4.d.z);
   t = ray4.Intersect(sphere);
   if (t != 0.0f)
   {
      Point3 intersect = ray4.Intersect(t);
      logmsg("   Ray4 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   }
   else
      logmsg("   Ray4 does not intersect sphere");

   return 1;
}