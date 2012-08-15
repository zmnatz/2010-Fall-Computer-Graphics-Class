//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    TransformNode.h
//	Purpose: Scene graph geometry node indicating a triangle based, flat surface.
//
//============================================================================

#ifndef __TRIFLATSURFACE_H
#define __TRIFLATSURFACE_H

/**
 * Flat mesh surface consisting of a triangles.
 */
class TriFlatSurface : public GeometryNode
{
public:
	/**
	 * Constructor. 
	 */
	TriFlatSurface() 
	{
		m_vertexBufferObjects = false;
		m_vertexBuffer = 0;
		m_faceBuffer   = 0;
		m_texCoordBuffer = 0;
		m_movement = Vector2(0,0);
		move = false;
	}
	
	/**
	 * Destructor.
	 */
	~TriFlatSurface() 
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

	void toggleMovement(){
		move = !move;
	}
	
	void Update(){
		if(move){
			for(int i=0;i<m_textureList.size();i++)
				m_textureList[i]+=m_movement;
		}
	}
protected:
	// Vertex buffer support
   bool         m_vertexBufferObjects;
   unsigned int m_faceListCount;
	GLuint       m_vertexBuffer;
	GLuint       m_faceBuffer;
	Vector2		m_movement;
	bool		move;
	GLuint		m_texCoordBuffer;
	std::vector<Vector2> m_textureList;
	
   // Vertex list - only using the vertex coordinate
	std::vector<Point3>         m_vertexList;
	
	// Only need a single normal to the entire face
	Vector3                     m_faceNormal;
	
	// Use unsigned short for face list indexes (OpenGL ES compatible)
	std::vector<unsigned short> m_faceList;

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

		// Bind the texture list to the texture buffer object if there are textures
		if(m_textureList.size()>0 && m_movement==Vector2(0,0)){
			glGenBuffers(1, &m_texCoordBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
			glBufferData(GL_ARRAY_BUFFER, m_textureList.size() * sizeof(Vector2), 
							(void*)&m_textureList[0], GL_STATIC_DRAW);
		}
      // Bind the vertex list to the vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vertexList.size() * sizeof(Point3), 
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

	   // Set the normal to this face
		glNormal3f(m_faceNormal.x, m_faceNormal.y, m_faceNormal.z);

      // Bind the vertex buffer, enable client state and set vertex pointer.
      // Note the use of 0 offset in glVertexPointer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
		bool drawnTextures = DrawTexture();
		
      // Note the use of 0 offset in glDrawElements
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceBuffer);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_faceListCount, GL_UNSIGNED_SHORT, (void*)0);
		
      // Reset state
		glDisableClientState(GL_VERTEX_ARRAY);
		if(drawnTextures){
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

   // Draw using vertex arrays
	void DrawVertexArrays()
	{
      // Set the normal to this face
		glNormal3f(m_faceNormal.x, m_faceNormal.y, m_faceNormal.z);

      // Enable and set the vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)&m_vertexList[0]);
		bool drawnTextures = DrawTexture();
      // Draw triangles using the face list
		glDrawElements(GL_TRIANGLES, (GLsizei)m_faceList.size(), 
					   GL_UNSIGNED_SHORT, (void*)&m_faceList[0]);
		
      // Reset state
		if(drawnTextures){
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	bool DrawTexture(){
		if(m_texCoordBuffer || m_textureList.size()>0){
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER,m_texCoordBuffer);
			if(m_texCoordBuffer)
				glTexCoordPointer(2,GL_FLOAT,sizeof(Vector2),(void*)0);
            else if(m_textureList.size()>0)
				glTexCoordPointer(2,GL_FLOAT,sizeof(Vector2),(void*)&m_textureList[0].x);
			return true;
		}
		return false;
	}
};

#endif