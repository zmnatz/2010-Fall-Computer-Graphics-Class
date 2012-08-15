//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    Scene.h
//	Purpose:	Scene graph support.
//
//============================================================================

#ifndef __SCENE_H
#define __SCENE_H

#include <list>

enum SceneNodeType { SCENE_BASE, SCENE_PRESENTATION, 
                     SCENE_TRANSFORM, SCENE_GEOMETRY,
                     SCENE_CAMERA,SCENE_LIGHT};

/**
 * Structure to hold color information, RGBA
 */
struct Color4
{
	float r, g, b, a;
	
	Color4() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
	Color4(const float ir, const float ig, const float ib) :
				r(ir), g(ig), b(ib), a(1.0f) {}
	Color4(const float ir, const float ig, const float ib, const float ia) :
				r(ir), g(ig), b(ib), a(ia) {}
};

/**
 * Structure to hold a normal and vertex
 */
struct VertexAndNormal
{
	Vector3 m_normal;
	Point3  m_vertex;
	
	VertexAndNormal()
	{
		m_normal.Set(0.0f, 0.0f, 0.0f);
		m_vertex.Set(0.0f, 0.0f, 0.0f);
	}
	VertexAndNormal(const Point3& vIn)
	{
		m_normal.Set(0.0f, 0.0f, 0.0f);
		m_vertex = vIn;
	}
};

// Include other scene files
#include "scene/SceneNode.h"
#include "scene/TransformNode.h"
#include "scene/PresentationNode.h"
#include "scene/GeometryNode.h"
#include "scene/TriCurvedSurface.h"
#include "scene/TriFlatSurface.h"
#include "scene/UnitSquare.h"
#include "scene/ConicSurface.h"
#include "Scene/TaurusNode.h"
#include "Scene/SphereNode.h"
#include "scene/CameraNode.h"
#include "Scene/LightNode.h"

#endif