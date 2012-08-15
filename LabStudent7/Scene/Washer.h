//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    TorusSurface.h
//	Purpose: Scene graph geometry node representing a torus.
//
//============================================================================

#ifndef __WASHERSURFACE_H
#define __WASHERSURFACE_H

#include <list>
#include "geometry/geometry.h"

class WasherSurface : public TriCurvedSurface
{
public:
	/**
    * Creates a washer with an inner and outer radius. 
    * @param   iradius  Inner radius of the washer
    * @param   oradius  Outer radius of the washer
    * @param   parts    Number of divisions around the ring
    */
	WasherSurface(const float iradius, const float oradius,const int parts)
	{
       int i;
	   float theta;  
	   float dtheta = (2.0f * M_PI) / (float)parts;
       VertexAndNormal vtx;

	   for (theta = 0.0, i = 0; i <= parts; i++, theta += dtheta)
	   {
			Vector2 dir(cos(theta),sin(theta));
			Vector2 inner = iradius * dir;
			Vector2 outer = oradius * dir;
			vtx.m_normal.Set(0.0,0.0,1.0);
			vtx.m_vertex.Set(inner.x,inner.y,0.5);
			m_vertexList.push_back(vtx);
			vtx.m_vertex.Set(outer.x,outer.y,0.5);
			m_vertexList.push_back(vtx);
			vtx.m_normal.Set(dir.x,dir.y,0);
			m_vertexList.push_back(vtx);
			vtx.m_vertex.Set(outer.x,outer.y,-.5);
			m_vertexList.push_back(vtx);
			vtx.m_normal.Set(0,0,-1);
			m_vertexList.push_back(vtx);
			vtx.m_vertex.Set(inner.x,inner.y,-.5);
			m_vertexList.push_back(vtx);
			vtx.m_normal.Set(-dir.x,-dir.y,0);
			m_vertexList.push_back(vtx);
			vtx.m_vertex.Set(inner.x,inner.y,0.5);
			m_vertexList.push_back(vtx);

			for(int k=0;k<4;k++){
				m_textureList.push_back(Vector2(i,0));
				m_textureList.push_back(Vector2(i,1));
			}

			if(i!=parts)
				for(int j=0;j<8;j+=2)
				{
					int pt = 8 * i + j;
					int nextPart = pt + 8;
					m_faceList.push_back(nextPart);
					m_faceList.push_back(pt);
					m_faceList.push_back(pt+1);
				
					m_faceList.push_back(nextPart+1);
					m_faceList.push_back(nextPart);
					m_faceList.push_back(pt+1);
				}
	   }
	}
	
private:
	// Make default constructor private to force use of the constructor
	// with arguments.
	WasherSurface() { };
};

#endif