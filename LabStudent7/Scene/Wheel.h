#ifndef __WHEELNODE_H
#define __WHEELNODE_H

/**
 * Wheel 
 */
class WheelNode : public TransformNode
{
public:
   /**
    * Constructor
    */
   WheelNode(float iradius, float oradius,SceneNode* side,
				SceneNode* spoke,int spokes,float speed, std::vector<LightNode*> lights) {		
		turn = false;
		TransformNode* lSide = new TransformNode;
		lSide->Translate(0,0,-10);
		lSide->AddChild(side);

		TransformNode* rSide = new TransformNode;
		rSide->Translate(0,0,10);
		rSide->AddChild(side);

		TransformNode* spokeTransform = new TransformNode;
		spokeTransform->Rotate(90,1,0,0);
		spokeTransform->Translate((oradius+iradius)/2,0,0);
		spokeTransform->AddChild(spoke);

		int i;
		float theta=0;
		float dtheta = 360/spokes;
		for(int i=0,theta=0;i<spokes;theta+=dtheta,i++){
			TransformNode* spokeRotate = new TransformNode;
			spokeRotate->AddChild(spokeTransform);
			spokeRotate->Rotate(theta,0,0,1);
			AddChild(spokeRotate);
		}
	
		AddChild(lSide);
		AddChild(rSide);
		m_speed = speed;
		m_lights=lights;
	}

	bool toggleTurn(){
		turn = turn ? false : true;
		for(int i=0;i<m_lights.size();i++)
			turn ? m_lights[i]->Enable() : m_lights[i]->Disable();
		return turn;
	}

	float GetSpeed(){
		return m_speed;
	}


	void Update(){
		if(turn) 
			Rotate(m_speed,0,0,-1);
	}
   /**
    * Destructor
    */
   virtual ~WheelNode() { }

private:
	float m_speed;
	std::vector<LightNode*> m_lights;
	bool turn;
};

#endif