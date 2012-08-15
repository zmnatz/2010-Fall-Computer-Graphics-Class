//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    LightingViewing.cpp
//	Purpose: OpenGL and GLUT program to draw a simple scene with
//          viewing controls and lighting and shading.
//
//============================================================================


#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <fcntl.h>
#include <io.h>
#include "geometry/geometry.h"
#include "scene/scene.h"
#include "ShaderSupport/GLSLShader.h"

#define FRAME_RATE 72

// While mouse button is down, the camera will be updated
bool  Animate = false;
bool  Forward = true;
float Velocity = 1.0f;
int   MouseX;
int   MouseY;

int RenderWidth  = 640;
int RenderHeight = 480;

// Global camera
CameraNode* MyCamera;

// Shaders
bool CanUseShaders = true;
GLSLVertexShader   VertexShader;
GLSLFragmentShader FragmentShader;
GLSLShaderProgram  ShaderProgram;

// Root of the scene graph
SceneNode* SceneRoot;
WheelNode* wheel;
UnitSquareFlatSurface* movingSquare;
ParticleNode* waterfall;

int delayedTurn;

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("PhongShading.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

GLuint LoadTextureBMP( const char * filename, int wrap,int size )
{
	GLuint texture;
	int width, height;
	void* data;
	FILE * file;

	// open texture data
	file = fopen(filename, "rb");
	if ( file == NULL ) return 0;

	// allocate buffer
	width = size;
	height = size;
	data = malloc( width * height * 3 );

	// read texture data
	fread( data, width * height * 3, 1, file );
	fclose( file );

	// allocate a texture name
	glGenTextures( 1, &texture );

	// select our current texture
	glBindTexture( GL_TEXTURE_2D, texture );

	// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST );
	// when texture area is large, bilinear filter the first mipmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP );

	// build our texture mipmaps
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
						GL_BGR, GL_UNSIGNED_BYTE, data );
	// free buffer
	free( data );
	return texture;
}

/**
 * Convencience method to add a material, then a transform, then a geometry
 * node as a child to a specified node.
 */
void AddSubTree(SceneNode* parent, SceneNode* material, 
                SceneNode* transform, SceneNode* geometry)
{
   parent->AddChild(material);
   material->AddChild(transform);
   transform->AddChild(geometry);
}

/**
 * Construct a box with the given dimensions.
 */
SceneNode* ConstructBox(float x, float y, float z)
{
   UnitSquareFlatSurface* xySurf = new UnitSquareFlatSurface(x,y,true,Vector2(),.05f);
   UnitSquareFlatSurface* xzSurf = new UnitSquareFlatSurface(x,z,true,Vector2(),.05f);
   UnitSquareFlatSurface* yzSurf = new UnitSquareFlatSurface(z,y,true,Vector2(),.05f);
   x *= .5;
   y *= .5;
   z *= .5;

   // Contruct transform nodes for the sides of the box.
   // Perform rotations so the sides face outwards
   // Bottom is rotated 180 degrees so it faces outwards
   TransformNode* bottomTransform = new TransformNode;
   bottomTransform->Translate(0.0f, 0.0f, -z);
   bottomTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);   

   // Back is rotated -90 degrees about x: (z -> y)
   TransformNode* backTransform = new TransformNode;
   backTransform->Translate(0.0f, y, 0.0f);
   backTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);   

   // Front wall is rotated 90 degrees about x: (y -> z)
   TransformNode* frontTransform = new TransformNode;
   frontTransform->Translate(0.0f, -y, 0.0f);
   frontTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   

   // Left wall is rotated -90 about y: (z -> -x)
   TransformNode* leftTransform = new TransformNode;
   leftTransform->Translate(-x, 0.0f, 00.0f);
   leftTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);

   // Right wall is rotated 90 degrees about y: (z -> x)
   TransformNode* rightTransform = new TransformNode;
   rightTransform->Translate(x, 0.0f, 0.0f);
   rightTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);

   // Top 
   TransformNode* topTransform = new TransformNode;
   topTransform->Translate(0.0f, 0.0f, z);

   // Create a SceneNode and add the 6 sides of the box.
   SceneNode* box = new SceneNode;
   box->AddChild(backTransform);
   backTransform->AddChild(xzSurf);
   box->AddChild(leftTransform);
   leftTransform->AddChild(yzSurf);
   box->AddChild(rightTransform);
   rightTransform->AddChild(yzSurf);
   box->AddChild(frontTransform);
   frontTransform->AddChild(xzSurf);
   box->AddChild(bottomTransform);
   bottomTransform->AddChild(xySurf);
   box->AddChild(topTransform);
   topTransform->AddChild(xySurf);
   return box;
}

/**
 * Construct a room. Like a box but looking inward with a texture for each side
 */
SceneNode* ConstructRoom() {
	GLuint textures[6];
	textures[0]=LoadTextureBMP("images/skybox0.bin",true,256);
	textures[1]=LoadTextureBMP("images/skybox1.bin",true,256);
	textures[2]=LoadTextureBMP("images/skybox2.bin",true,256);
	textures[3]=LoadTextureBMP("images/skybox3.bin",true,256);
	textures[4]=LoadTextureBMP("images/skybox4.bin",true,256);
	textures[5]=LoadTextureBMP("images/skybox5.bin",true,256);

	SceneNode* skybox = new SceneNode;
	int SKYBOX_SIZE=2000;
	int HEIGHT = 800;
	int HALF_HEIGHT = HEIGHT * .5f;
	int HALF_SKYBOX = SKYBOX_SIZE * .5f;
	UnitSquareFlatSurface* unitSquare = new UnitSquareFlatSurface(SKYBOX_SIZE,
		SKYBOX_SIZE,true,Vector2(),.01);
	TransformNode* transform[6];

	UnitSquareFlatSurface* wall = new UnitSquareFlatSurface(1,1,true,Vector2(),1);

	// Construct transform nodes for the walls. Perform rotations so the 
	// walls face inwards
	transform[0] = new TransformNode;
	//transform[0]->Scale(SKYBOX_SIZE,SKYBOX_SIZE,1);

	// Back wall is rotated +90 degrees about x: (y -> z)
	transform[1] = new TransformNode;
	transform[1]->Translate(0.0f, HALF_SKYBOX, HALF_HEIGHT);
	transform[1]->Scale(SKYBOX_SIZE,1,HEIGHT);
	transform[1]->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   

	// Front wall is rotated -90 degrees about x: (z -> y)
	transform[2] = new TransformNode;
	transform[2]->Translate(0.0f, -HALF_SKYBOX, HALF_HEIGHT);
	transform[2]->Scale(SKYBOX_SIZE,1,HEIGHT);
	transform[2]->Rotate(-90.0f, 1.0f, 0.0f, 0.0f); 

	// Left wall is rotated 90 degrees about y: (z -> x)
	transform[3] = new TransformNode;
	transform[3]->Translate(-HALF_SKYBOX, 0.0f, HALF_HEIGHT);
	transform[3]->Scale(1,SKYBOX_SIZE,HEIGHT);
	transform[3]->Rotate(90.0f, 0.0f, 1.0f, 0.0f);

	// Right wall is rotated -90 about y: (z -> -x)
	transform[4]= new TransformNode;
	transform[4]->Translate(HALF_SKYBOX, 0.0f, HALF_HEIGHT);
	transform[4]->Scale(1,SKYBOX_SIZE,HEIGHT);
	transform[4]->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);

	// Ceiling is rotated 180 about x so it faces inwards
	transform[5]= new TransformNode;
	transform[5]->Translate(0.0f, 0.0f, HEIGHT);
	transform[5]->Scale(SKYBOX_SIZE,SKYBOX_SIZE,1);
	transform[5]->Rotate(180.0f, 1.0f, 0.0f, 0.0f);

	for(int i=0;i<6;i++){
		PresentationNode* tex = new PresentationNode();
		tex->SetMaterialAmbientAndDiffuse(Color4(0.8f, 0.8f, 0.8f));
		tex->SetMaterialSpecular(Color4(0.3f, 0.3f, 0.3f));
		tex->SetMaterialShininess(32.0f);
		tex->setTexture(textures[i]);
		tex->AddChild(transform[i]);
		transform[i]->AddChild(i==0 ? unitSquare : wall);
		skybox->AddChild(tex);
	}
	return skybox;
}
 
SceneNode* ConstructHouse(float x, float z)
   {
		SceneNode* house = new SceneNode;
		SceneNode* topBottom = ConstructBox(x,x,1);
		SceneNode* windowWall = new SceneNode;
	
		SceneNode* partialWall = new SceneNode;
		TransformNode* wallBottom = new TransformNode;
		TransformNode* wallTop = new TransformNode;
	
		float panelSize = .2f*x;

		wallBottom->Translate(0,0,-(z*.25f));
		wallTop->Translate(0,0,.4f*z);

		wallBottom->AddChild(ConstructBox(panelSize,1,z*.5f));
		wallTop->AddChild(ConstructBox(panelSize,1,z*.2f));
		partialWall->AddChild(wallBottom);
		partialWall->AddChild(wallTop);

		SceneNode* fullWall = ConstructBox(panelSize,1,z);

		for(int i=0;i<5;i++){
			TransformNode* wall = new TransformNode;
			wall->AddChild(i%2 ? partialWall : fullWall);
			wall->Translate((float)i*panelSize-.4*x,0,0);
			windowWall->AddChild(wall);
		}
		for(int i=0;i<3;i++){
			TransformNode* wall = new TransformNode;
			wall->AddChild(windowWall);
			wall->Rotate(90*i,0,0,1);
			wall->Translate(0,.5f*x,.5f*z);
			house->AddChild(wall);
		}

		TransformNode* doorWall = new TransformNode;
		doorWall->Rotate(-90,0,0,1);
		doorWall->Translate(0,.5f*x,.5f*z);
		house->AddChild(doorWall);

		for(int i=-1;i<2;i+=2){
			TransformNode* wall = new TransformNode;
			wall->Translate(i*.4f*x,0,0);
			wall->AddChild(fullWall);
			doorWall->AddChild(wall);
		}
		
		TransformNode* floor = new TransformNode;
		floor->AddChild(topBottom);
	
		TransformNode* ceiling = new TransformNode;
		ceiling->Translate(0,0,z);
		ceiling->AddChild(topBottom);

		house->AddChild(floor);
		house->AddChild(ceiling);
		return house;
   }

SceneNode* ConstructWall(PresentationNode* material,PresentationNode* containedMaterial,SceneNode* contained){
	SceneNode* wall = new TransformNode;
	wall->AddChild(material);
	wall->AddChild(containedMaterial);

	SceneNode* box = ConstructBox(200,6,25);

	TransformNode* t = new TransformNode;
	t->AddChild(box);
	t->Translate(0,14.6f,12.5f);
	material->AddChild(t);

	TransformNode* t2 = new TransformNode;
	t2->AddChild(box);
	t2->Translate(0,-14.6f,12.5f);
	material->AddChild(t2);

	TransformNode* inner = new TransformNode;
	inner->AddChild(ConstructBox(200,26.2,15));
	inner->Translate(0,0,7.5f);
	material->AddChild(inner);

	TransformNode* top = new TransformNode;
	top->AddChild(contained);
	top->Translate(0,0,20);
	containedMaterial->AddChild(top);
	return wall;
 }
/**
 * Construct the scene
 */
void ConstructScene()
{
   // -------------------- Geometry -------------------- //
   movingSquare = new UnitSquareFlatSurface(200,32.2f*.814f,true,Vector2(-.01f,0),.03f);

   SceneNode* house = ConstructHouse(70,90);

   // Construct a unit cylinder surface
   WasherSurface* cylinder = new WasherSurface(.9f, 1.0f, 18);
   
   //Wheel
   WasherSurface* washer = new WasherSurface(10,20,14);
   std::vector<LightNode*> lights;
   lights.push_back(new LightNode(GL_LIGHT3));
   lights.push_back(new LightNode(GL_LIGHT4));
   wheel = new WheelNode(10,20,washer,ConstructBox(9,19.5f,1),8,4,lights);

   waterfall = new ParticleNode(25);

   //-------------------- Materials ------------------------- //

   // Wood
   PresentationNode* wood = new PresentationNode;
   wood->SetMaterialAmbientAndDiffuse(Color4(0.5f, 0.5f, 0.5f));
   wood->SetMaterialSpecular(Color4(0.3f, 0.3f, 0.3f));
   wood->SetMaterialShininess(32.0f);
   wood->setTexture(LoadTextureBMP("images/wood.bin",true,128));

   // Stone
   PresentationNode* stone = new PresentationNode;
   stone->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
   stone->SetMaterialDiffuse(Color4(0.50754f, 0.50754f, 0.50754f));
   stone->SetMaterialSpecular(Color4(0.508273f, 0.508273f, 0.508273f));
   stone->SetMaterialShininess(10.2f);
   stone->setTexture(LoadTextureBMP("images/stone.bin",true,128));

   PresentationNode* steel = new PresentationNode;
   steel->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
   steel->SetMaterialDiffuse(Color4(0.7f, 0.7f, 0.7f));
   steel->SetMaterialSpecular(Color4(0.7f, 0.7f, 0.7f));
   steel->SetMaterialShininess(90.2f);

   // Water
   PresentationNode* water = new PresentationNode;
   water->SetMaterialAmbient(Color4(0.01f, 0.01f, 0.1f));
   water->SetMaterialDiffuse(Color4(0.2f, 0.2f, 0.6f));
   water->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
   water->SetMaterialShininess(10);
   water->setTexture(LoadTextureBMP("images/water.bin",true,128));

   PresentationNode* blue = new PresentationNode;
   blue->SetMaterialAmbient(Color4(0.01f, 0.01f, 0.1f));
   blue->SetMaterialDiffuse(Color4(0.2f, 0.2f, 0.6f));
   blue->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
   blue->SetMaterialShininess(10);
   // ------------------ Transformations ------------------- //
   TransformNode* wheelTransform = new TransformNode;
   wheelTransform->Translate(-80.0f, 35.0f, 40.0f);
   wheelTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);

   TransformNode* houseTransform = new TransformNode;
   houseTransform->Translate(-100,35,0);

   TransformNode* house2Transform = new TransformNode;
   house2Transform->Rotate(180,0,0,1);
   house2Transform->Translate(-100,-35,0);

   TransformNode* wallTransform = new TransformNode;
   wallTransform->Translate(0,35,0);

   TransformNode* waterfallTransform = new TransformNode;
   waterfallTransform->Translate(-128.0f,35,15.0f);

   TransformNode* pipeTransform = new TransformNode;
   pipeTransform->Rotate(-90,0,1,0);
   pipeTransform->Translate(76.5f,35,125);
   pipeTransform->Scale(5,5,20);

   // -------------------- Lighting --------------------------/

   // Light 0 - point light source in back right corner
	LightNode* light0 = new LightNode(GL_LIGHT0);
	light0->SetDiffuse(Color4(0.5f, 0.5f, 0.5f, 1.0f));
	light0->SetSpecular(Color4(0.5f, 0.5f, 0.5f, 1.0f));
	light0->SetPosition(HPoint3(90.0f, 90.0f, 30.f, 1.0f));	
	light0->Enable();

   // Light1 - directional light from the ceiling
	LightNode* light1 = new LightNode(GL_LIGHT1);
	light1->SetDiffuse(Color4(0.7f, 0.7f, 0.7f, 1.0f ));
	light1->SetSpecular(Color4(0.7f, 0.7f, 0.7f, 1.0f));
	light1->SetPosition(HPoint3(0.0f, 0.0f, 1.0f, 0.0f));	
	light1->Enable();

   // Light2 - spotlight - we will place at the camera location
   // shining along -VPN
	LightNode* light2 = new LightNode(GL_LIGHT2);
	light2->SetDiffuse(Color4(0.8f, 0.8f, 0.8f, 1.0f ));
	light2->SetSpecular(Color4(0.8f, 0.8f, 0.8f, 1.0f));
	light2->SetPosition(HPoint3(0.0f, 0.0f, 0.0f, 1.0f));	
	light2->SetSpotlight(Vector3(0.0f, 0.0f, -1.0f), 32.0f, 30.0f);
	light2->Enable();

	lights[0]->SetDiffuse(Color4(0.8f, 0.8f, 0.8f, 1.0f));
	lights[0]->SetSpecular(Color4(0.8f, 0.8f, 0.8f, 1.0f));
	lights[0]->SetPosition(HPoint3(-100.0f,35.0f,90.0f,1.0f));
	lights[0]->SetSpotlightDirection(Vector3(0,0,-1));
	lights[0]->SetSpotlight(Vector3(0,0,-1),1,90);
	lights[0]->Disable();

	lights[1]->SetDiffuse(Color4(0.8f, 0.8f, 0.8f, 1.0f));
	lights[1]->SetSpecular(Color4(0.8f, 0.8f, 0.8f, 1.0f));
	lights[1]->SetPosition(HPoint3(100.0f,35.0f,90.0f,1.0f));
	lights[1]->SetSpotlightDirection(Vector3(0,0,-1));
	lights[1]->SetSpotlight(Vector3(0,0,-1),1,90);
	lights[1]->Disable();
  
   // --------------------------- Camera ----------------------- //
   MyCamera = new CameraNode;
   MyCamera->SetPosition(Point3(100.0f, -100.0f, 50.0f));
   MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 50.0f));
   MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
   MyCamera->SetPerspective(50.0, 1.0, 1.0, 2400);

   // --------------------- Scene construction ----------------- //

   // Construct the scene root node
   SceneRoot = new SceneNode;

   // Create a scene node to hold all scene objects (other than camera
   // and lights)
   SceneNode* myScene = new SceneNode;

   // Add the spotlight as the first child of the root node. Since this is 
   // accessed before the camera it will position the light relative to the
   // camera
   SceneRoot->AddChild(light2);

   // Set the camera

	SceneRoot->AddChild(MyCamera);
	MyCamera->AddChild(light0);
	MyCamera->AddChild(light1);
	MyCamera->AddChild(lights[0]);
	MyCamera->AddChild(lights[1]);
	MyCamera->AddChild(myScene);

   // Construct the room (walls, floor, ceiling)
	SceneNode* skybox = ConstructRoom();

   wallTransform->AddChild(ConstructWall(stone,water,movingSquare));
   myScene->AddChild(wallTransform);

   // Construct the wheel
   AddSubTree(myScene,wood,wheelTransform,wheel);

    // Place 2 Houses
   wood->AddChild(houseTransform);
   houseTransform->AddChild(house);
   wood->AddChild(house2Transform);
   house2Transform->AddChild(house);
   myScene->AddChild(skybox);

   AddSubTree(myScene,steel,pipeTransform,cylinder);
   AddSubTree(myScene,blue,waterfallTransform,waterfall);
}

/**
 * Use a timer method to try to do a consistent update rate or 72Hz.
 * Without using a timer, the speed of movement will depend on how fast
 * the program runs (fast movement on a fast PC and slow movement on a
 * slower PC)
 */
void timerFunction(int value)
{
   // If mouse button is down, generate another view
   if (Animate)
   {
      // Find relative dx and dy relative to center of the window
      float dx = 4.0f * ((MouseX - ((float)RenderWidth * 0.5f))  / (float)RenderWidth);
      float dy = 4.0f * ((((float)RenderHeight * 0.5f) - MouseY) / (float)RenderHeight);
      float dz = (Forward) ? Velocity : -Velocity;
      MyCamera->MoveAndTurn(dx * Velocity, dy * Velocity, dz);
   }
	if(delayedTurn){
		delayedTurn--;
		if(delayedTurn==10)
			wheel->toggleTurn();
		else if(!delayedTurn)
			movingSquare->toggleMovement();
	}
    
	SceneRoot->Update();
    glutPostRedisplay();

    glutTimerFunc(1000.0 / FRAME_RATE, timerFunction, 0);
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // Draw the scene graph
   SceneRoot->Draw();

   // Swap buffers
   glutSwapBuffers();
}

/**
 * Keyboard callback.
 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {
    // Escape key
    case 27:            
      exit(0);
	   break;

    // Reset the view
    case 'i':
      MyCamera->SetPosition(Point3(0.0f, -100.0f, 50.0f));
      MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
      MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
      glutPostRedisplay();
      break;

    // Roll the camera by 5 degrees
    case 'r':
      MyCamera->Roll(5);
      glutPostRedisplay();
	   break;

    // Roll the camera by 5 degrees (clockwise)
    case 'R':
      MyCamera->Roll(-5);
      glutPostRedisplay();
	   break;
	case 't':
		if(!delayedTurn){
			delayedTurn = 40;
			waterfall->toggleFlow();
		}
		break;
    // Change the pitch of the camera by 5 degrees
    case 'p':
      MyCamera->Pitch(5);
      glutPostRedisplay();
	   break;

    // Change the pitch of the camera by 5 degrees (clockwise)
    case 'P':
      MyCamera->Pitch(-5);
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees
    case 'h':
      MyCamera->Heading(5);
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees (clockwise)
    case 'H':
      MyCamera->Heading(-5);
      glutPostRedisplay();
	   break;

    // Go faster
    case 'V':
      Velocity += 0.2f;
      break;

    // Go slower
    case 'v':
      Velocity -= 0.2f;
      if (Velocity < 0.2f)
         Velocity = 0.1f;
      break;

    // Gouraud/smooth shading
    case 'g':
      glShadeModel(GL_SMOOTH);
      glutPostRedisplay();
	   break;

    // Flat shading
    case 'f':
      glShadeModel(GL_FLAT);
      glutPostRedisplay();
	   break;

    // Use shaders
    case 'S':
       ShaderProgram.Use();
       glutPostRedisplay();
       break;

    // Use fixed function pipeline
    case 's':
       glUseProgram(0);
       glutPostRedisplay();
       break;

    default:
       break;
   }
}

/**
 * Mouse callback (called when a mouse button state changes)
 */
void mouse(int button, int state, int x, int y)
{
   // On a left button up event MoveAndTurn the view with forward motion
   if (button == GLUT_LEFT_BUTTON)
   {
      if (state == GLUT_DOWN)
      {
         MouseX = x;
         MouseY = y;
         Forward = true;
         Animate = true;
      }
      else
         Animate = false;  // Disable animation when the button is released
   }

   // On a right button up event MoveAndTurn the view with reverse motion
   if (button == GLUT_RIGHT_BUTTON)
   {
      if (state == GLUT_DOWN)
      { 
         MouseX = x;
         MouseY = y;
         Forward = false;
         Animate = true;
      }
      else
         Animate = false;  // Disable animation when the button is released
   }
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void mouseMotion(int x, int y)
{
   // Update position used for changing the view and force a new view
   MouseX = x;
   MouseY = y;
}

/**
 * Reshape callback.  Reset the perpective projection so the field of
 * view matches the window's aspect ratio.
 */
void reshape(int width, int height)
{
   RenderWidth  = width;
   RenderHeight = height;

   // Reset the viewport
   glViewport(0, 0, width, height);

   // Reset the perspective projection to reflect the change of 
   // the aspect ratio 
   MyCamera->ChangeAspectRatio((double)width / (double)height);
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
   glutCreateWindow("Phong Shading / Shaders Example");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutKeyboardFunc(keyboard);

   // Set the clear color to black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Smooth shading by default
   glShadeModel(GL_SMOOTH);

   // Enable the depth buffer
   glEnable(GL_DEPTH_TEST);

   // Enable normalization of vectors (so we can do scaling!)
   glEnable(GL_NORMALIZE);

   // Enable back face polygon removal
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   // Set ambient light in the lighting model and enable lighting
   GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
   glEnable(GL_LIGHTING);

   //Enable 2D Textures
   glEnable( GL_TEXTURE_2D );

   // Print the keyboard commands
   printf("i - Reset to initial view\n");
   printf("R - Rotate roll 5 degrees clockwise\n");
   printf("r - Rotate roll 5 degrees counter-clockwise\n");
   printf("P - Rotate pitch 5 degrees clockwise\n");
   printf("p - Rotate pitch 5 degrees counter-clockwise\n");
   printf("H - Rotate heading 5 degrees clockwise\n");
   printf("h - Rotate heading 5 degrees counter-clockwise\n");
   printf("V - Faster mouse movement\n");
   printf("v - slower mouse movement\n");
   printf("g - Gouraud shading\n");
   printf("f - Flat shading\n");
   printf("S - Use Shaders (default)\n");
   printf("s - Use fixed function pipeline\n");
   printf("t - Toggle water flow\n");

   CanUseShaders = true;
   glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Can use GLSL as part of OpenGL 2.0\n");
	else 
   {
		printf("OpenGL 2.0 not supported - see if we can use extensions\n");
      if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) 
	      printf("Can use GLSL with extensions\n");
      else
      { 
         CanUseShaders = false;
         printf("Cannot use GLSL!\n");
      } 
	}

    // Set up shaders
   if (CanUseShaders)
   {
      if (!VertexShader.Create("shaders/phong.vert"))
      {
         printf("Vertex Shader compile failed\n");
         exit(1);
      }
      if (!FragmentShader.Create("shaders/phong.frag"))
      {
         printf("Fragment Shader compile failed\n");
         exit(1);
      }
      ShaderProgram.Create();
      if (!ShaderProgram.AttachShaders(VertexShader.Get(), FragmentShader.Get()))
      {
         printf("Shader program link failed\n");
         exit(1);
      }
      ShaderProgram.Use();
   }

   // Construct scene
   ConstructScene();
   //gluLookAt(0, 1, 3, 0, 1, 0, 0, 1, 0);

   // Set the number of lights used
   int nl = glGetUniformLocation(ShaderProgram.GetProgram(), "numLights");
   glUniform1i(nl, 5);
   //glutIdleFunc(idle);
   glutTimerFunc(1000.0 / FRAME_RATE, timerFunction, 0);
   glutMainLoop();
   return 0;
}