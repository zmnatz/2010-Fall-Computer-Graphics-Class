//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    ConicSurface.h
//	Purpose: Scene graph geometry node representing a taurus surface.
//
//============================================================================

#ifndef __TAURUSNODE_H
#define __TAURUSNODE_H

/**
 * Taurus surface geometry node.
 */
class TaurusNode : public TriCurvedSurface
{
public:
	/**
    * Creates a taurus
    *
    * @param   ringRadius   Radius of ring
    * @param   tubeRadius	Radius of tube
    * @param  createVertexBufferObjects  If true this object will use vertex buffer objects.
    */
	TaurusNode(const float ringRadius,const float tubeRadius,int hDivs,int vDivs,bool createVertexBufferObjects)
	{
		float theta = 0.0f;
		float dTheta = (2*M_PI/vDivs);
		float innerTheta = 0.0f;
		float dInnerTheta = 2 * M_PI/hDivs;
		//Loop around outer ring
		for(int i=0;i<hDivs;i++,theta+=dTheta){
			Vector3 direction = Vector3(cos(theta),sin(theta),0);
			Vector3 center = ringRadius * direction;
			innerTheta = -M_PI/2;
			//Write vertexes for tube
			for(int j=0;j<vDivs;j++,innerTheta+=dInnerTheta){
				Vector3 normal = cos(innerTheta)*direction;
				normal.z=sin(innerTheta);
				Vector3 point = center+tubeRadius*normal;
				VertexAndNormal vn;
				vn.m_vertex = Point3(point.x,point.y,point.z);
				vn.m_normal = normal;
				m_vertexList.push_back(vn);
				
				int node = i*vDivs+j;
				int nextCircle = (i==hDivs-1) ? j : node+vDivs;
				int next = (j==vDivs-1) ? -1*j : 1;
				
				m_faceList.push_back(nextCircle);
				m_faceList.push_back(nextCircle+next);
				m_faceList.push_back(node);
				

				m_faceList.push_back(nextCircle+next);
				m_faceList.push_back(node+next);	
				m_faceList.push_back(node);
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
	TaurusNode() { };
};

#endif