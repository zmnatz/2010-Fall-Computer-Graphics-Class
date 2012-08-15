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

#ifndef __UNITCYLINDER_H
#define __UNITCYLINDER_H
#include <math.h>
#include <Scene\Scene.h>

/**
 * Unit square geometry node.
 */
class UnitCylinder: public GeometryNode
{
public:
   /**
    * Constructor. Construct the vertex list for a triangle fan
    * representing a unit square
    */
	UnitCylinder(float topRadius,float bottomRadius,int numDivs,int vDivs,bool useVbo)
	{
		float TWO_PI = 2*M_PI;
		float angleStep = TWO_PI/numDivs;
		float vStep = 1.0f/vDivs;
		float cylSlope = topRadius-bottomRadius;
		for(int i=0;i<=vDivs;i++){
			float z=vStep*i-.5;
			float radius = bottomRadius+((float)i/vDivs)*cylSlope;
			for(int a=0;a<numDivs;a++){
				float angle = a*angleStep;
				Point3 p = Point3(radius*cos(angle),radius*sin(angle),z);
				m_vertexList.push_back(p);
				Vector3 normal(p);
				normal.z=cylSlope;
				normal.Normalize();
				m_normalList.push_back(normal);
				if(i!=vDivs){
					int node = i*numDivs+a;
					int nextNode = (a+1==numDivs) ? i*numDivs : node+1;
					m_faceList.push_back(node+numDivs);
					m_faceList.push_back(node);
					m_faceList.push_back(nextNode+numDivs);

					m_faceList.push_back(nextNode+numDivs);
					m_faceList.push_back(node);
					m_faceList.push_back(nextNode);	
				}
			}
		}
		//Generate Top 
		int lastNode = m_vertexList.size();
		m_vertexList.push_back(Point3(0,0,.5));
		m_normalList.push_back(Vector3(0,0,1));
		for(int j=lastNode-numDivs;j<lastNode;j++){
			int nextNode = j+1==lastNode ? lastNode-numDivs : j+1;
			m_faceList.push_back(m_vertexList.size()-1);
			m_faceList.push_back(j);
			m_faceList.push_back(nextNode);
		}
		//Generate Bottom
		m_vertexList.push_back(Point3(0,0,-.5));
		m_normalList.push_back(Vector3(0,0,-1));
		for(int j=0;j<numDivs;j++){
			int nextNode = j+1==numDivs ? 0 : j+1;
			m_faceList.push_back(m_vertexList.size()-1);
			m_faceList.push_back(nextNode);
			m_faceList.push_back(j);
		}

		//Create vertex buffer
		useVertexBuffer = useVbo;
		if(useVertexBuffer){
			glGenBuffers(3,bufferId);
			glBindBuffer(GL_ARRAY_BUFFER,bufferId[0]);
			glBufferData(GL_ARRAY_BUFFER,m_vertexList.size()*sizeof(Point3),
				(void*)&m_vertexList.front(),GL_STATIC_DRAW);
			
			glBindBuffer(GL_ARRAY_BUFFER,bufferId[1]);
			glBufferData(GL_ARRAY_BUFFER,m_normalList.size()*sizeof(Vector3),
				(void*)&m_normalList.front(),GL_STATIC_DRAW);
		}
	}

   /**
    * Destructor
    */
	virtual ~UnitCylinder() { }

	/**
	 * Draw a unit square. Note that geometry nodes are at the
    * leaves of the tree, so no children exist.
	 */
	virtual void Draw()
	{
      glEnableClientState(GL_VERTEX_ARRAY);
		if(useVertexBuffer){
			glBindBuffer(GL_ARRAY_BUFFER,bufferId[0]);
			glVertexPointer(3, GL_FLOAT, 0,0);
			glBindBuffer(GL_ARRAY_BUFFER,bufferId[1]);
			glNormalPointer(GL_FLOAT,0,0);
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}else{
			glVertexPointer(3, GL_FLOAT, 0, (void*)&m_vertexList[0]);
			glNormalPointer(GL_FLOAT,0,(void*)&m_normalList[0]);
		}
		glDrawElements(GL_TRIANGLES,(GLsizei)m_faceList.size(),GL_UNSIGNED_INT,(void*)&m_faceList[0]);
      glDisableClientState(GL_VERTEX_ARRAY);
	}

protected:
	std::vector<Point3> m_vertexList;
	std::vector<Vector3>m_normalList;
	std::vector<unsigned int> m_faceList;
	bool useVertexBuffer;
	GLuint bufferId[3];
};

#endif