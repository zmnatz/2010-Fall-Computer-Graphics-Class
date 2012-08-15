//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	 David W. Nesbitt
//	File:     TransformNode.h
//	Purpose:  Scene graph geometry node indicating a triangle based,
//           curved surface.
//
//============================================================================

#ifndef __TRICURVEDSURFACE_H
#define __TRICURVEDSURFACE_H

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/**
 * Triangle mesh surface for use with curved surfaces. Uses 
 * vertex normals.  
 */
class TriCurvedSurface : public GeometryNode
{
public:
	/**
	 * Constructor. 
	 */
	TriCurvedSurface() 
   {
      m_vertexBufferObjects = false;
		m_vertexBuffer = 0;
		m_faceBuffer   = 0;
   }
	
	/**
	 * Destructor.
	 */
	~TriCurvedSurface() 
   {
      if (m_vertexBufferObjects)
      {
         // Delete vertex buffer objects
         glDeleteBuffers(1, &m_vertexBuffer);
		   glDeleteBuffers(1, &m_faceBuffer);
      }
   }
	
   /**
    * Draw this geometry node. Uses either vertex buffer objects (if created) or
    * vertex arrays.
    */
	void Draw()
	{
      if (m_vertexBufferObjects)
			DrawVertexBuffers();
      else
         DrawVertexArrays();
	}
	
	/**
	 * Construct curved surface by passing in vertex list and face list,
	 */
	void Construct(std::vector<VertexAndNormal>& vertexList, 
				   std::vector<unsigned short> faceList)
	{
		m_vertexList = vertexList;
		m_faceList   = faceList;
	}
	
protected:
   // Vertex buffer support
   bool         m_vertexBufferObjects;
   unsigned int m_faceListCount;
	GLuint       m_vertexBuffer;
	GLuint       m_faceBuffer;

	std::vector<VertexAndNormal> m_vertexList;
	
	// Use unsigned short for face list indexes (OpenGL ES compatible)
	std::vector<unsigned short>  m_faceList;

   /**
    * Creates vertex buffers for this object.
    */
	void CreateVertexBuffers()
	{
      // Clear any current GL errors
      GLenum currentError = glGetError();
      if (currentError != GL_NO_ERROR)
         printf("GL Error %d occured at top of CreateVertexBuffers\n", currentError);

      // Generate vertex buffers for the vertex list and the face list
		glGenBuffers(1, &m_vertexBuffer);
		glGenBuffers(1, &m_faceBuffer);
		
      // Bind the vertex list to the vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vertexList.size() * sizeof(VertexAndNormal), 
                     (void*)&m_vertexList[0], GL_STATIC_DRAW);
		
      // Bind the face list to the vertex buffer object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faceList.size() * sizeof(unsigned short),
                     (void*)&m_faceList[0], GL_STATIC_DRAW);

      // Check if any errors occured - if so log it and do not use vertex buffer objects for this node
      currentError = glGetError();
      if (currentError != GL_NO_ERROR)
      {
         printf("GL Error %d occured in CreateVertexBuffers\n", currentError);
         m_vertexBufferObjects = false;
      }
      else
      {
         m_vertexBufferObjects = true;

         // Copy the face list count for use in Draw (then we can clear the vector)
         m_faceListCount = m_faceList.size();

         // We could clear any local memory as it is now in the VBO. However there may be
         // cases where we want to keep it (e.g. collision detection, picking) so I am not
         // going to do that here.
      }
	
      // "unbind" these vertex buffer objects - reset state
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

   // Draw using vertex buffer objects
	void DrawVertexBuffers()
	{
      // Bind the vertex buffer, enable client state for vertices and normal, and set 
      // vertex pointer and normal pointers.
      // Note the use of the offset and the stride.
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexAndNormal), BUFFER_OFFSET(3 * sizeof(float)));
      glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(VertexAndNormal), (void*)0);
		
      // Note the use of 0 offset in glDrawElements
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceBuffer);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_faceListCount, GL_UNSIGNED_SHORT, (void*)0);
		
      // Reset state
		glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

   // Draw using vertex arrays
   void DrawVertexArrays()
   {
		/** Interleaved arrays are easier but are not available in OpenGL ES
		 glInterleavedArrays(GL_N3F_V3F, 0, (void*)&m_vertexList[0]);
		 glDrawElements(GL_TRIANGLES, (GLsizei)m_faceList.size(), 
		                GL_UNSIGNED_SHORT, (void*)&m_faceList[0]);  **/
		
      // Enable vertex and normal arrays
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		
		// Use vertex and normal pointers into structure. Note that the stride allows use of an
      // array of this structure rather than 2 arrays. Make sure the pointer points to the 
      // 1st element of the desired type.
		glNormalPointer(GL_FLOAT,    sizeof(VertexAndNormal), (void*)&m_vertexList[0].m_normal.x);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexAndNormal), (void*)&m_vertexList[0].m_vertex.x);

      // Draw triangles jusing the face list
		glDrawElements(GL_TRIANGLES, (GLsizei)m_faceList.size(), 
					   GL_UNSIGNED_SHORT, (void*)&m_faceList[0]);
		
      // Reset state
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};


#endif