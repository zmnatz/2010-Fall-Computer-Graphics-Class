//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    GeometryNode.h
//	Purpose: Scene graph geometry node.
//
//============================================================================

#ifndef __GEOMETRYNODE_H
#define __GEOMETRYNODE_H

/**
 * Geometry node base class. Stores and draws geometry.
 */
class GeometryNode : public SceneNode
{
public:
   /**
    * Constructor
    */
   GeometryNode()
   {
      m_nodeType = SCENE_GEOMETRY;
   }

   /**
    * Destructor
    */
   virtual ~GeometryNode() { }
};

#endif