
#ifndef __UNITSPHERE_H
#define __UNITSPHERE_H

#include <GL/glut.h>

/**
 * Geometry node base class. Stores and draws geometry.
 */
class UnitSphere: public SceneNode
{
public:
   /**
    * Constructor
    */
	UnitSphere() {
	}

   /**
    * Destructor
    */
   virtual ~UnitSphere() { }

   /**
    * Draw this geometry node. Geometry nodes are leaf nodes and have no children.
    */
   virtual void Draw()
   {
	   glPushMatrix();
			glutSolidSphere(1,30,30);
	   glPopMatrix();
   }

protected:
};

#endif