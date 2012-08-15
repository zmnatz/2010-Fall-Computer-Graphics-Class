//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Animation3D.cpp
//	Purpose: OpenGL and GLUT program to draw a simple 3D animation with 
//          collision detection. The scene contains moving spheres inside 
//          a fixed, cube enclosure. A simple scene graph is used to define the 
//          scene. Transform nodes, geometry nodes, and presentation nodes
//          are used.
//
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <Scene/Scene.h>

SceneNode scene;
float fps = 72;
Plane walls[6];
BallTransform* balls[9];
int t = 0;
int nextIntersection = 0;

void timer(int value)
{
	for(int i=0;i<9;i++){
		Point3 nextP = balls[i]->sphere.m_center + balls[i]->movementVector;
		float intersectTime = -1;
		Plane* closestPlane = 0;
		//Detect wall collision
		for(int j=0;j<6;j++){
			float sol = walls[j].Solve(nextP);		
			if(sol<=balls[i]->sphere.m_radius){
				float solFromStart = walls[j].Solve(balls[i]->sphere.m_center);
				float temp = solFromStart+balls[i]->sphere.m_radius-sol;
				float tempIntersectTime = solFromStart/temp * balls[i]->speed;
				if(intersectTime == -1 || tempIntersectTime<intersectTime){
					closestPlane = walls+j;
					intersectTime = tempIntersectTime;
				}
			}
		}
		
		//Detect Sphere Collision
		for(int j=0;j<9;j++){
			if(i!=j){
				Ray3 r(balls[i]->sphere.m_center,
					balls[i]->movementVector-balls[j]->movementVector);
				BoundingSphere sphere(balls[j]->sphere.m_center,
					balls[i]->sphere.m_radius + balls[j]->sphere.m_radius);
				float intersectDist = r.Intersect(sphere);
				//The collision must be the first one for both balls, errors occur if 1 ball
				//reacts to the collision but the other does not.
 				if(0<intersectDist && intersectDist<=1 && 
						(intersectTime == -1 || intersectDist<intersectTime) &&
						(balls[j]->intersectTime==0 || t+intersectDist < balls[j]->intersectTime)){
					Vector3 vectorToCol = intersectDist * balls[i]->movementVector;
					Vector3 normal = vectorToCol.Projection(balls[i]->movementVector+balls[j]->movementVector)
								- vectorToCol;
					closestPlane = new Plane(r.o+vectorToCol,normal);	
					intersectTime = intersectDist;
				}
			}
		}
		//If Collision occurs, set collision plane and intersect time
		if(closestPlane){
			balls[i]->intersectPlane = *closestPlane;
			balls[i]->intersectTime = t + intersectTime;
		}
	}
	scene.Update();
	t++;
	glutPostRedisplay();
	glutTimerFunc(1000/fps,timer,0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.Draw();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	//Restrict aspect ratio to 4/3 consistent with 640/480 resolution at start
	float ratio = 4./3.;
	float adjustedW = (float)height*ratio;
	if(width==adjustedW){
		glViewport(0,0,width,height);
	}else if(width>adjustedW){
		int x = width-adjustedW;
		glViewport(x/2,0,width-x,height);
	}else{
		int adjustedH = width/ratio;
		int y = height-adjustedH;
		glViewport(0,y/2,width,height-y);
	}
}

void initializeEnvironment(int argc, char** argv){
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640,480);
   glutInit(&argc, argv);
   glutCreateWindow("3D Animation Lab - Zaven Mnatzakanian");
	//Seed the random number generator
   srand(929233);

   // The stuff below sets the default projection and view as well as a simple
   // lighting environment. (DO NOT CHANGE)
	
   // Set a fixed perspective projection
   glMatrixMode(GL_PROJECTION);
   gluPerspective(70.0f, 1.0f, 1.0f, 200.0f);
   glMatrixMode(GL_MODELVIEW);

   // Set a fixed camera outside the center of the front wall (imagine
   // it being a window) looking parallel to the floor
   gluLookAt(0.0f, -90.0f, 50.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f);

   // Set up a simple lighting environment
   glEnable(GL_LIGHTING);

   // Set white light. Somewhat dim for positioning
   // in the back corner
   GLfloat diffuse0[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse0);
   glEnable(GL_LIGHT0);

   // Load light 0 position - basically a modeling transformation
   // to position the light. Positional light just above the camera
   GLfloat position0[] = { 0.0f, -90.0f, 30.f, 1.0f };
   glLightfv(GL_LIGHT0, GL_POSITION, position0);
   // Enable normalization of vectors (so we can do scaling!)
   glEnable(GL_NORMALIZE);
   
   //Enable depth buffering and back face removal
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

float getRandom(const float minv, const float maxv)
{
    return minv + ((maxv - minv) * (float)rand() / (float)RAND_MAX);
}

int main(int argc, char** argv) {
	initializeEnvironment(argc,argv);
   	scene = SceneNode();

	//Origin moved so that so that each wall can simply rotate the floor to get other walls
	TransformNode* wallScaler = new TransformNode();
	wallScaler->Translate(0,0,50);
	wallScaler->Scale(100,100,100);
	
	//Create Base Wall
	Point3 v[] = {Point3(-.5,.5,0),Point3(-.5,-.5,0),Point3(.5,.5,0),Point3(.5,-.5,0)};
	walls[0] = Plane(Point3(0,0,0),Vector3(0,0,1));
	TransformNode* wall = new TransformNode();
	wall->Translate(0,0,-.5);
	wall->AddChild(new UnitSquareSurface(v));
	
	//Floor
	walls[1] = Plane(Point3(-50,0,0),Vector3(1,0,0));
	MaterialNode* floor = new MaterialNode(.7,.55,0);
	floor->AddChild(wall);
	wallScaler->AddChild(floor);	

	//Ceiling
	walls[2] = Plane(Point3(0,0,100),Vector3(0,0,-1));
	MaterialNode* ceiling = new MaterialNode(.4,.4,1.);
	TransformNode* ceiling_T = new TransformNode();
	ceiling_T->AddChild(wall);
	ceiling_T->Rotate(180,1,0,0);
	ceiling->AddChild(ceiling_T);
	wallScaler->AddChild(ceiling);
	
	//Back
	walls[3] = Plane(Point3(0,50,0),Vector3(0,-1,0));
	MaterialNode* backWall = new MaterialNode(1.,.65,.5);
	TransformNode* backWall_T = new TransformNode();
	backWall_T->AddChild(wall);
	backWall_T->Rotate(90,1,0,0);
	backWall->AddChild(backWall_T);
	wallScaler->AddChild(backWall);

	//Sides
	MaterialNode* whiteWall = new MaterialNode(2,2,2);
		walls[4] = Plane(Point3(0,-50,0),Vector3(0,1,0));
		TransformNode* leftWall = new TransformNode();
		leftWall->AddChild(wall);
		leftWall->Rotate(90,0,1,0);
		whiteWall->AddChild(leftWall);

		walls[5] = Plane(Point3(50,0,0),Vector3(-1,0,0));
		TransformNode* rightWall = new TransformNode();
		rightWall->AddChild(wall);
		rightWall->Rotate(-90,0,1,0);
		whiteWall->AddChild(rightWall);
	wallScaler->AddChild(whiteWall);

	scene.AddChild(wallScaler);
	
	//Balls
	MaterialNode* colors[] = {	new MaterialNode(1,0,0),
								new MaterialNode(0,1,0),
								new MaterialNode(0,0,1)};
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			int ballNo = i*3+j;
			balls[ballNo] = new BallTransform(
				Point3(getRandom(-40,40),getRandom(-40,40),getRandom(25,75)),
				getRandom(5./fps,15./fps),
				Vector3(getRandom(0,1),getRandom(0,1),getRandom(0,1)),
				getRandom(3,7));
			colors[i]->AddChild(balls[ballNo]);
		}	
		scene.AddChild(colors[i]);
	}

	glutTimerFunc(0,timer,0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
   return 0;
}