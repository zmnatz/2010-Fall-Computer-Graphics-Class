//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSquare.h
//	Purpose: Scene graph geometry node representing a unit square.
//
//============================================================================

#ifndef __UNITSQUARE_H
#define __UNITSQUARE_H

class UnitSquareFlatSurface : public TriFlatSurface
{
public:
	/**
	 * Creates a unit length and width "flat surface".  The surface is composed of
	 * triangles such that the unit length/width surface is divided into n
	 * equal paritions in both x and y. Constructs a vertex list and face list
	 * for the surface.
    * @param  n   Number of subdivisions in x and y
    * @param  createVertexBufferObjects  If true this object will use vertex buffer objects.
	 */
	UnitSquareFlatSurface(float width,float height, bool createVertexBufferObjects,Vector2 movement, float scale)
	{
		int n=1;
		// Having issues with roundoff when n = 40,50 - so compare with
		// some tolerance
		Point3 p(-0.5f, -0.5f, 0.0f);
		width *= .5f;
		height *= .5f;
		for (int i=-1; i <= 1;i+=2){
			for (int j=-1;j<=1;j+=2){
				float ws = width*j;
				float hs = height*i;
				m_vertexList.push_back(Point3(ws,hs,0)); 
				m_textureList.push_back(scale*Vector2(ws+width,hs+height));
			}
		}
		

		// Form triangle face indexes. Note: there are n+1 rows 
		// and n+1 columns.
		m_nCols = n + 1;
		for (unsigned int row = 0; row < n; row++)
		{
			for (unsigned int col = 0; col < n; col++)
			{
				// Divide each square into 2 triangles - make sure they are ccw.
				// GL_TRIANGLES draws independent triangles for each set of 3 vertices
				m_faceList.push_back(getIndex(1, 0));
				m_faceList.push_back(getIndex(0, 0));
				m_faceList.push_back(getIndex(0, 1));
				
				m_faceList.push_back(getIndex(1, 0));
				m_faceList.push_back(getIndex(0, 1));
				m_faceList.push_back(getIndex(1, 1));
			}
		}
		
		// Face normal is (0, 0, 1)
		m_faceNormal.Set(0.0f, 0.0f, 1.0f);
		m_movement = movement;
		move = false;
      // Create vertex buffer objects if specified
      if (createVertexBufferObjects)
         CreateVertexBuffers();
	}
private:
	unsigned int m_nCols;
	
	// Convenience method to get the index into the vertex list given the
	// "row" and "column" of the subdivision/grid
	unsigned int getIndex(unsigned int row, unsigned int col) const
	{
		return (row*m_nCols) + col;
	}	
};

#endif