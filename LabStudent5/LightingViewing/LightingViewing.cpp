//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    LightingViewing.cpp
//	Purpose: OpenGL and GLUT program to draw a simple scene with
//          viewing controls and fixed function lighting and shading.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "scene/scene.h"

int FrameID = 0;
const float WORLD_ASPECT = 4.0f/3.0f;
const float FRAMES_PER_SEC = 72.0f;

// Root of the scene graph
SceneNode* SceneRoot;
//Position 
Point3 position;
Vector3 lookAt;
Vector3 normal;
float velocity;
bool leftMouseDown = false;
bool rightMouseDown = false;
Point3 mousePos;


// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("LightingViewing.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

void setupLighting(){
	//Set light 1 on the ceiling perpendicular to the ceiling
	glEnable(GL_LIGHT0);
	GLfloat position0[] = {0,0,-1,0};
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	// Set white light in the back corner
	glEnable(GL_LIGHT1);
	GLfloat position1[] = {90,90,30,1};
	GLfloat diffuse1[]  = {1,1,1,1};
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
}

/**
 * Use a timer method to try to do a consistent update rate.
 * Without using a timer, the speed of movement will depend on how fast
 * the program runs (fast movement on a fast PC and slow movement on a
 * slower PC)
 */
void timerFunction(int value)
{
	//Check mouse down for movement, forward or back
	if(leftMouseDown || rightMouseDown){
		//Vector3 v = Vector3(0,1,0).Cross(moveTo);
		Matrix4x4 m = Matrix4x4();
		m.Rotate(-80.0f*mousePos.x/FRAMES_PER_SEC,normal.x,normal.y,normal.z);
		Vector3 cross = normal.Cross(lookAt);
		m.Rotate(-80.0f*mousePos.y/FRAMES_PER_SEC,cross.x,cross.y,cross.z);
		
		lookAt = m * lookAt;
		normal = m * normal;
		position = position + ((leftMouseDown ? 1 : 0) + (rightMouseDown ? -1 : 0))
			* (velocity / FRAMES_PER_SEC * lookAt);
	}

	glLoadIdentity();
	Point3 look = position+lookAt;
	gluLookAt(position.x,position.y,position.z,look.x,look.y,look.z,normal.x,normal.y,normal.z);
	setupLighting();
	glutPostRedisplay();
	glutTimerFunc(1000 / FRAMES_PER_SEC, timerFunction, 0);
	
}
void initCamera(){
	//Construct camera points
	position = Point3(0,-100,20);
	lookAt = Vector3(0,1,0);
	normal = Vector3(0,0,1);
	velocity = 20;
}
/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Draw the scene graph
   SceneRoot->Draw();
  
   // Swap buffers
   glutSwapBuffers();
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void mouseMotion(int x, int y)
{
	mousePos.x = ((float)x/glutGet(GLUT_WINDOW_WIDTH))-.5f;
	mousePos.y = (1.0f-(float)y/glutGet(GLUT_WINDOW_HEIGHT))-.5f;
}

/**
 * Mouse callback (called when a mouse button state changes)
 */
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN){
		if(button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON){
			if (button == GLUT_LEFT_BUTTON) {
				leftMouseDown = true;
			}else if (button == GLUT_RIGHT_BUTTON) {
				rightMouseDown = true;
			}
			mouseMotion(x,y);
		}
	}else if(state == GLUT_UP){
		if(button == GLUT_LEFT_BUTTON){
			leftMouseDown = false;
		}else if(button == GLUT_RIGHT_BUTTON){
			rightMouseDown = false;
		}	
	}

}



/**
 * Keyboard callback.
 */
void keyboard(unsigned char key, int x, int y)
{
	Matrix4x4 m = Matrix4x4();
	Vector3 v;
   switch (key) 
   {
   case 'R':
   case 'r':
	   m.Rotate(key=='r' ? -5 : 5,lookAt.x,lookAt.y,lookAt.z);
	   normal = m * normal;
	   break;
   case 'h':
   case 'H':
	   m.Rotate(key=='h' ? 5 : -5,normal.x,normal.y,normal.z);
	   lookAt = m * lookAt;
	   break;
   case 'p':
   case 'P':
	   v = normal.Cross(lookAt);
	   m.Rotate(key=='p' ? 5 : -5,v.x,v.y,v.z);
	   lookAt = m * lookAt;
	   normal = m * normal;
	   break;
   case 'f':
	   glShadeModel(GL_FLAT);
	   break;
   case 'g':
	   glShadeModel(GL_SMOOTH);
	   break;
   case 'v':
	   velocity--;
		break;
   case 'V':
	   velocity++;
	   break;
    // Escape key
   case 'i':
	   initCamera();
	   break;
    case 27:            
      exit(0);
	   break;

    default:
       break;
   }
}

/**
 * Reshape callback. Alter the viewport to maintain a fixed, square viewport
 * in the middle of the screen window.
 */
void reshape(int width, int height)
{
   // Center the world window in the viewport so it does not
   // stretch world window
   float windowAspect = (float)width / (float)height;
   if (windowAspect > WORLD_ASPECT)
   {
      // Window is wider than world
      float w = height * WORLD_ASPECT;
      float startX = (width - w) * 0.5f;
      glViewport(startX, 0, w, height);
   }
   else
   {
      // Window is taller than world
      float h = width / WORLD_ASPECT;
      float startY = (height - h) * 0.5f;
      glViewport(0, startY, width, h);
   }
}



/**
 * Construct the scene
 */
void ConstructScene()
{
	// Construct the scene layout
   SceneRoot = new SceneNode;
   // Construct the geometry objects
   UnitSquareFlatSurface* unitSquare = new UnitSquareFlatSurface(10,10,true);

   // Construct Walls
	TransformNode* floorTransform = new TransformNode;
	floorTransform->Scale(200.0f, 200.0f, 1.0f);
	floorTransform->AddChild(unitSquare);
	MaterialNode* floorColor = new MaterialNode(0.44f,.66f,0.29f,5);
	floorColor->AddChild(floorTransform);
	SceneRoot->AddChild(floorColor);

	TransformNode* sideWall = new TransformNode;
	MaterialNode* wallColor = new MaterialNode(0.8f, 0.5f, 0.5f,15);
	sideWall->Translate(0.0f,100,40);
	sideWall->Rotate(90,1,0,0);
	sideWall->Scale(200.0f,80.0f,1.0f);
	for(int i=0;i<4;i++){
		TransformNode* wall = new TransformNode;
		wall->Rotate(90*i,0,0,1);
		wall->AddChild(sideWall);
		SceneRoot->AddChild(wall);
	}
	wallColor->AddChild(unitSquare);
	sideWall->AddChild(wallColor);

   TransformNode* ceiling = new TransformNode;
   ceiling->Rotate(180,1,0,0);
   ceiling->Translate(0,0,-80);
   ceiling->AddChild(floorTransform);
   MaterialNode* ceilingColor = new MaterialNode(1.0f,1.0f,1.0f,40);
   ceilingColor->AddChild(ceiling);
   SceneRoot->AddChild(ceilingColor);

   //Build Table
   //Couldn't get the lighting to work exactly like the example. But the
   //Shading definitely works. Just look at the table while facing the back
   //Right corner
   TransformNode* tablePlacement = new TransformNode;
   tablePlacement->Translate(-50,50,0);
   tablePlacement->Rotate(30,0,0,1);
   MaterialNode* tableWood = new MaterialNode(.8,.5,.3,30);
   tableWood->AddChild(tablePlacement);
   SceneRoot->AddChild(tableWood);

   TransformNode* tableTop = new TransformNode;
   tableTop->Translate(0,0,23);
   tableTop->Scale(60,40,6);
   tableTop->AddChild(new UnitCube(10,true));
   tablePlacement->AddChild(tableTop);

   TransformNode* tableLeg = new TransformNode;
   tableLeg->Scale(1,1,20);
   tableLeg->Translate(0,0,.5);
   tableLeg->AddChild(new UnitCylinder(3,3,10,10,true));
   
   TransformNode* leg1 = new TransformNode;
   leg1->Translate(25,15,0);
   leg1->AddChild(tableLeg);
   tablePlacement->AddChild(leg1);
   
   TransformNode* leg2 = new TransformNode;
   leg2->Translate(-25,15,0);
   leg2->AddChild(tableLeg);
   tablePlacement->AddChild(leg2);
   
   TransformNode* leg3 = new TransformNode;
   leg3->Translate(-25,-15,0);
   leg3->AddChild(tableLeg);
   tablePlacement->AddChild(leg3);

   TransformNode* leg4 = new TransformNode;
   leg4->Translate(25,-15,0);
   leg4->AddChild(tableLeg);
   tablePlacement->AddChild(leg4);
   
   //Create Teapot
   TransformNode* teapotPlacement = new TransformNode;
   teapotPlacement->Rotate(90,1,0,0);
   teapotPlacement->Translate(0,29.8,0);
   MaterialNode* teapotMaterial = new MaterialNode(0.19225f, 0.19225f, 0.19225f,
													0.50754f, 0.50754f, 0.50754f,
													0.508273f, 0.508273f, 0.508273f,
													51.2);
	teapotMaterial->AddChild(new TeapotNode);
	teapotPlacement->AddChild(teapotMaterial);
	tablePlacement->AddChild(teapotPlacement);	
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
    // Set up depth buffer and double buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 480);
   glutInit(&argc, argv);
   
	
   // Set callback methods
   glutCreateWindow("Lighting and Viewing");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glewInit();

   // Set the clear color to black. Any part of the window outside the
   // viewport should appear black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Construct scene. Seed the random number generator
   srand(929233);

   // Enable depth testing
   glEnable(GL_DEPTH_TEST);

   // Enable back face polygon removal
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   // Set a fixed perspective projection
   glMatrixMode(GL_PROJECTION);
   gluPerspective(50.0f, WORLD_ASPECT, 1.0f, 290.0f);
   glMatrixMode(GL_MODELVIEW);

   // it being a window) looking parallel to the floor
   	initCamera();
	Point3 look = position+lookAt;
	gluLookAt(position.x,position.y,position.z,look.x,look.y,look.z,normal.x,normal.y,normal.z);
   // Set up a simple lighting environment
   glEnable(GL_LIGHTING);
   setupLighting();

   // Enable normalization of vectors (so we can do scaling!)
   glEnable(GL_NORMALIZE);
   ConstructScene();
   glutTimerFunc(1000 / FRAMES_PER_SEC, timerFunction, 0);
   glutPostRedisplay();
   glutMainLoop();
   
   
   
    
   return 0;
}