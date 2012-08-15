//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSquareSurface.h
//	Purpose:	Simple geometry node that defines a unit square.
//
//============================================================================

#ifndef __UNITSQUAREFLATSURFACE_H
#define __UNITSQUAREFLATSURFACE_H

#include <Scene\Scene.h>



/**
 * Unit square geometry node.
 */
class UnitSquareFlatSurface: public GeometryNode
{
public:
   /**
    * Constructor. Construct the vertex list for a triangle fan
    * representing a unit square
    */
	UnitSquareFlatSurface(int hDivs,int vDivs,bool useVbo)
	{
		useVertexBuffer = useVbo;
		float hStep = 1.0f/hDivs;
		float vStep = 1.0f/vDivs;
		for(int i=0;i<=hDivs;i++){
			float x = -.5+(float)i*hStep;
			for(int j=0;j<=vDivs;j++){
				float y = -.5+(float)j*vStep;
				m_vertexList.push_back(Point2(x,y));
				if(i!=hDivs && j!=vDivs){
					int node = i*(vDivs+1)+j;
					int nextNode = node + (vDivs+1);
					m_faceList.push_back(node+1);
					m_faceList.push_back(node);
					m_faceList.push_back(nextNode);

					m_faceList.push_back(node+1);
					m_faceList.push_back(nextNode);
					m_faceList.push_back(nextNode+1);
				}
			}
		}
		if(useVertexBuffer){
			useVertexBuffer = useVbo;
			glGenBuffers(1,&bufferId);
			glBindBuffer(GL_ARRAY_BUFFER,bufferId);
			glBufferData(GL_ARRAY_BUFFER,m_vertexList.size()*sizeof(Point2),
				(void*)&m_vertexList.front(),GL_STATIC_DRAW);
		}
	}

   /**
    * Destructor
    */
	virtual ~UnitSquareFlatSurface() { }

	/**
	 * Draw a unit square. Note that geometry nodes are at the
    * leaves of the tree, so no children exist.
	 */
	virtual void Draw()
	{
		// Set a single normal
		glNormal3f(0.0f, 0.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
			if(useVertexBuffer){
				glBindBuffer(GL_ARRAY_BUFFER,bufferId);
				glVertexPointer(2,GL_FLOAT,0,0);
				glBindBuffer(GL_ARRAY_BUFFER,0);
			}else{
				glVertexPointer(2, GL_FLOAT, 0, (void*)&m_vertexList[0]);
			}
			glDrawElements(GL_TRIANGLES,(GLsizei)m_faceList.size(),GL_UNSIGNED_INT,(void*)&m_faceList[0]);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

protected:
   std::vector<Point2> m_vertexList;
   std::vector<unsigned int> m_faceList;
   bool useVertexBuffer;
   GLuint bufferId;
};

#endif