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
#include <GL/glut.h>

enum SceneNodeType { SCENE_BASE, SCENE_PRESENTATION, 
                     SCENE_TRANSFORM, SCENE_GEOMETRY };

// Include other scene files
#include "scene/SceneNode.h"
#include "scene/TransformNode.h"
#include "scene/PresentationNode.h"
#include "Scene/MaterialNode.h"
#include "Scene/ColorNode.h"
#include "scene/GeometryNode.h"
#include "scene/UnitSquareFlatSurface.h"
#include "Scene/UnitCube.h"
#include "Scene/TeapotNode.h"
#include "Scene/UnitCylinder.h"

#endif