//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    PresentationNode.h
//	Purpose:	Scene graph presentation node.
//
//============================================================================

#ifndef __MATERIALNODE_H
#define __MATERIALNODE_H

#include <GL/glut.h>

/**
 * Presentation node.
 */
class MaterialNode: public PresentationNode
{
public:
   /**
    * Constructor.
    */
	MaterialNode(float r,float g,float b, float shine){
		ambient[0]=r;
		ambient[1]=g;
		ambient[2]=b;
		diffuse[0]=r;
		diffuse[1]=g;
		diffuse[2]=b;
		specular[0]=r;
		specular[1]=g;
		specular[2]=b;
		shiny = shine;
	}

	MaterialNode(float aR,float aG,float aB,
				float dR,float dG,float dB,
				float spR,float spG,float spB,float sh) {

		ambient[0]=aR;
		ambient[1]=aG;
		ambient[2]=aB;
		diffuse[0]=dR;
		diffuse[1]=dG;
		diffuse[2]=dB;
		specular[0]=spR;
		specular[1]=spG;
		specular[2]=spB;
		shiny = sh;
		
	}

   /**
    * Destructor
    */
	virtual ~MaterialNode() { }

	/**
	 * Draw this presentation node and its children
	 */
	virtual void Draw()
	{
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shiny);
			SceneNode::Draw();
		glPopMatrix();
	}

protected:
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shiny;
};

#endif