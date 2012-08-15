//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    TransformNode.h
//	Purpose:	Scene graph transformation node.
//
//============================================================================

#ifndef __BALLTRANSFORM_H
#define __BALLTRANSFORM_H

#include <list>
#include <GL/glut.h>

/**
 * Transform node. Applies a transformation. This class allows OpenGL style 
 * transforms applied to the scene graph.
 */
class BallTransform: public TransformNode
{
public:
	float speed;
	BoundingSphere sphere;
	Vector3 dir;
	float intersectTime;
	Plane intersectPlane;
	Vector3 movementVector;

   /**
    * Constructor.
    */
	BallTransform(Point3 pos,float s,Vector3 direction,float size) {
		dir = direction.NormSquared()== 1. ? direction : direction.Normalize();
		movementVector = dir*s;
		speed = s;
		sphere = BoundingSphere(pos,size);
		float scaledSize = size/speed;

		//Scale the unitsphere
		TransformNode* sizeTranslation = new TransformNode();
		sizeTranslation->Scale(size,size,size);
		sizeTranslation->AddChild(new UnitSphere());
		
		//Set translation to set position
		TransformNode::Translate(pos.x,pos.y,pos.z);
		TransformNode::AddChild(sizeTranslation);

		intersectTime = 0;
		t=0;
	}

   /**
    * Destructor.
    */
	virtual ~BallTransform() { }

	void Translate(const float x, const float y, const float z)
	{
		sphere.m_center = sphere.m_center + Vector3(x,y,z);
		TransformNode::Translate(x,y,z);
	}

	

   /**
	 * Update the position of the ball
	 */
	virtual void Update()
   {
	   t++;
	   //Handle collision
	   if(intersectTime!=0 && t>intersectTime){
		   dir = dir.Reflect(intersectPlane.GetNormal());
		   float timeToReflect = t-intersectTime;
		   float timeAfterReflect = 1-timeToReflect;
		   Vector3 translateVector = movementVector * timeToReflect;
			movementVector = dir * speed;
			translateVector = translateVector - movementVector * timeAfterReflect;
		   Translate(translateVector.x,translateVector.y,translateVector.z);
		   intersectTime = 0;
	   }else{
			Translate(movementVector.x,movementVector.y,movementVector.z);
	   }
   }

protected:
	float t;
	TransformNode* positionTranslation;
};

#endif