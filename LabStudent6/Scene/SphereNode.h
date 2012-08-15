//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    ConicSurface.h
//	Purpose: Scene graph geometry node representing a conic surface.
//
//============================================================================

#ifndef __SPHERENODE_H
#define __SPHERENODE_H

/**
 * Sphere surface geometry node.
 */
class SphereNode : public TriCurvedSurface
{
public:
	/**
    * Creates a sphere
    *
    * @param   radius   Radius of sphere
    * @param   hDivs         Number of sides (divisions of the top/bottom)
    * @param   vDivs        Number of height divisions
    * @param  createVertexBufferObjects  If true this object will use vertex buffer objects.
    */
	SphereNode(const float radius,int hDivs,int vDivs,bool createVertexBufferObjects)
	{
		float theta = 0.0f;
		float dTheta = (2*M_PI/vDivs);
		float innerTheta;
		float dInnerTheta = M_PI/hDivs;
		for(int i=0;i<hDivs;i++,theta+=dTheta){
			innerTheta = -M_PI/2;
			for(int j=0;j<=vDivs;j++,innerTheta+=dInnerTheta){
				Vector3 normal = cos(innerTheta)*Vector3(cos(theta),sin(theta),0);
				normal.z=sin(innerTheta);
				Vector3 point = radius*normal;
				VertexAndNormal vn;
				vn.m_vertex = Point3(point.x,point.y,point.z);
				vn.m_normal = normal;
				m_vertexList.push_back(vn);
				if(j!=vDivs){
					int node = i*(vDivs+1)+j;
					int nextCircle = (i==hDivs-1) ? j : node+vDivs+1;
				
					m_faceList.push_back(nextCircle);
					m_faceList.push_back(nextCircle+1);
					m_faceList.push_back(node);

					m_faceList.push_back(nextCircle+1);
					m_faceList.push_back(node+1);	
					m_faceList.push_back(node);
				}
			}
		}
      // Create vertex buffers
      if (createVertexBufferObjects)
         CreateVertexBuffers();
	}
	
private:
   unsigned int m_nRows;
   unsigned int m_nCols;

	// Make default constructor private to force use of the constructor
	// with number of subdivisions.
	SphereNode() { };
};

#endif