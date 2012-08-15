//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Ray.h
//	Purpose: Ray class. Includes intersection methods with other objects.
//          Student should include "geometry.h" to get all class definitions
//          included in proper order.
//============================================================================

#ifndef __RAY_H__
#define __RAY_H__

#include <math.h>
#include <vector>

/**
 * 3D ray
 */
struct Ray3
{
   Point3  o;     // Origin of the ray
   Vector3 d;     // Ray direction

   /**
    * Default constructor.
    */
   Ray3(void)
   {
      o.Set(0.0f, 0.0f, 0.0f);
      d.Set(1.0f, 0.0f, 0.0f);
   }

   /**
    * Constructor given 2 points.
    * @param   p1    Start point of the ray.
    * @param   p2    End point of the ray (at t = 1).
    * @param   norm  If true normalize the direction vector
    */
   Ray3(const Point3& p1, const Point3& p2, bool normalize)
   {
	   // DONE: Student to define
		Vector3 v(p1,p2);
		if(normalize)
			v.Normalize();
		o=p1;
		d=v;
   }

   /**
    * Constructor given an origin and a direction. 
    * @param   origin   Origin point of the ray.
    * @param   dir      Ray direction.
    */
   Ray3(const Point3& origin, const Vector3& dir)
   {
      // DONE: Student to define
		o=origin;
		d=dir;
   }

   /** 
    * Calculates the reflected ray given the intersect point and
    * the normal to the surface. 
    * @return  Returns the reflected ray.
    */
   Ray3 Reflect(const Point3& intPt, const Vector3& normal) const
   {
      // Required in 605.767
      return Ray3();
   }

   /**
    * Finds the refracted ray.
    * @param   intPt   Intersection point (origin of the refracted ray)
    * @param   n       Normal at the intersection point
    * @param   u1      Index of refraction (leaving)
    * @param   u2      Index of refraction (entering)
    * @param   tir     (OUT) Total internal reflection
    * @return  Returns the refracted ray.
    */
   Ray3 Refract(const Point3& intPt, Vector3& n, const float u1, float u2, 
                bool& tir)
   {
      // Required in 605.767
      return Ray3();
   }

   /**
    * Returns the intersect point given the parameter t
    * @param   t    Parameter along the ray
    * @return  Returns the point along the direction d from the origin a.
    */
   Point3 Intersect(const float t) const
   {
      //DONE: Student to define!
		Vector3 v = Vector3(o)+d*t;
	   return Point3(v.x,v.y,v.z);
   }

   //  ---------------- Intersection methods --------------------//

   /**
    * Intersection of the ray with a plane.
    * @param   plane  Plane to test intersection with
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const Plane& p) const
   {
     // Required in 605.767
      return 0.0f; 
   }
      
   /**
    * Intersection of a ray and a sphere using vector method.
    * @param   sphere  Sphere to test intersection with
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const BoundingSphere& sphere) const
   {
		Vector3 oToSphereCenter(o,sphere.m_center);							
		Vector3 projection = oToSphereCenter.Projection(d);					//Project o to sphere center onto d
		float distToSphereCenter = oToSphereCenter.Norm();					//Distance from o to sphere center
		float angleBetween = radiansToDegrees(d.AngleBetween(projection));	
		float dist = (projection-oToSphereCenter).Norm();					//Distance from projection to sphere center

		if(sphere.m_radius<dist || (angleBetween!=0.0f && distToSphereCenter>sphere.m_radius))
			return 0.0f;		// Sphere's radius is less than distance to projection or 
								// o is outside sphere and d points away, so no intersect
		else if(sphere.m_radius==dist)				//d falls exactly on the edge of the sphere
			return projection.Norm()/d.Norm();		
		else if(distToSphereCenter<sphere.m_radius)		//origin is inside the sphere
			if(angleBetween!=0)	//distance determined by projection->center (leg) and sphere radius (hypotenuse)
				return sqrt(pow(sphere.m_radius,2)-pow(dist,2))/d.Norm();
			else	//the segment between the origin and the sphere edge is the hypotenuse
				return sqrt(pow(sphere.m_radius,2)+oToSphereCenter.NormSquared())/d.Norm();
		else
			return (projection.Norm()-sqrt(pow(sphere.m_radius,2)-pow(dist,2)))/d.Norm();
   }

   /**
    * Intersection of a ray and an axis aligned bounding box.
    * @param   box  AABB to test intersection with
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const AABB& box) const
   {
      // Required in 605.767
      return 0.0f; 
   }

   /**
    * Intersection of a ray and a polygon
    * @param   polygon  Polygon to test intersection with
    * @param   normal   Normal to the polygon
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(std::vector<Point3>& polygon, Vector3& normal) const
   {
      // Required in 605.767
      return 0.0f;
   }

   /**
    * Intersection of a ray with a triangle.
    * @param   v0  Vertex of the triangle
    * @param   v1  Vertex of the triangle
    * @param   v2  Vertex of the triangle
    * @param   u   (OUT) barycentric coordinate of intersection
    * @param   v   (OUT) barycentric coordinate of intersection
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const Point3& v0, const Point3& v1, const Point3& v2,
                   float& u, float& v) const
   {
      // Required in 605.767
      return 0.0f;
   }
};

#endif
