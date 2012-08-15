
#ifndef __UNITSQUARESURFACE_H
#define __UNITESQURESURFACE_H

#include <geometry\Point3.h>
#include <GL/glut.h>


/**
 * Geometry node base class. Stores and draws geometry.
 */
class UnitSquareSurface: public SceneNode
{
public:
   /**
    * Constructor
    */
	UnitSquareSurface(Point3* v) {
		vertices = v;
	}

   /**
    * Destructor
    */
   virtual ~UnitSquareSurface() { }

   /**
    * Draw this geometry node. Geometry nodes are leaf nodes and have no children.
    */
   virtual void Draw()
   {
		glPushMatrix();
			glEnableClientState(GL_VERTEX_ARRAY);
				glNormal3f(0,0,1);
				glVertexPointer(3, GL_FLOAT, 0, vertices);
				glDrawArrays(GL_TRIANGLE_STRIP,0,4);
			glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
   }

protected:
	Point3*	  vertices;
};

#endif