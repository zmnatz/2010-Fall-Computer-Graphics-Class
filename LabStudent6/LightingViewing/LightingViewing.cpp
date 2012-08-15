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

// Include OpenGL support
#include <GL/glew.h>
#include <GL/glut.h>

// Include local libraries (geometry first)
#include "geometry/geometry.h"
#include "scene/scene.h"

// While mouse button is down, the view will be updated
bool  Animate = false;
bool  Forward = true;
float Velocity = 1.0f;
int   MouseX;
int   MouseY;

int RenderWidth  = 640;
int RenderHeight = 480;

const float FrameRate = 72.0f;

// Global camera
CameraNode MyCamera;

// Root of the scene graph
SceneNode* SceneRoot;

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

// Teapot class - very simple class to use GLUT to draw a teapot
class Teapot: public GeometryNode
{
public:
	Teapot() { }
	virtual ~Teapot() { }
	virtual void Draw()
	{
      // Teapot is defined with CW faces and is "open" at the top. Disable
      // back face culling, draw, then re-enable culling. You might try enabling
      // 2-sided lighting and see what happens. If you don't change the front face
      // to CW, the lighting on the teapot is messed up!
      glFrontFace(GL_CW);
      glDisable(GL_CULL_FACE);
      glutSolidTeapot(5.0f);
      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);
	}
};

/**
 * Updates the view given the mouse position and whether to move 
 * forward or backward
 */
void UpdateView(const int x, const int y, bool forward)
{
   // Find relative dx and dy relative to center of the window
   float dx = 4.0f * ((x - ((float)RenderWidth * 0.5f))  / (float)RenderWidth);
   float dy = 4.0f * ((((float)RenderHeight * 0.5f) - y) / (float)RenderHeight);
   float dz = (forward) ? Velocity : -Velocity;
   MyCamera.MoveAndTurn(dx * Velocity, dy * Velocity, dz);
   glutPostRedisplay();
}

void ConstructLighting(void)
{
   // Set ambient light in the lighting model - unless you are attenuating
   // the individual lights it is probably best to set ambient light with 
   // the model and keep individual lights without it. Also, note that the
   // materials have low values for ambient reflection so this intensity is
   // set to be high. If you keep the ambient reflection at the same values
   // as the diffuse, then you can lower the ambient light model intensity
   GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

   // Note: setting this to false will be more efficient
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

   // 2-sided lighting (uncomment this to see the change to the 
   // lighting inside the teapot
 //  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

   // Enable lighting
   glEnable(GL_LIGHTING);

   // Set white light. Somewhat dim for positioning
   // in the back corner
   GLfloat diffuse0[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
   GLfloat specular0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse0);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
   glEnable(GL_LIGHT0);

   // Set a brighter white light for use as a directional
   // light from the ceiling
   GLfloat diffuse1[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat specular1[] = { 0.7f, 0.7f, 0.7f, 1.0f };
   glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse1);
   glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
   glEnable(GL_LIGHT1);
}

/**
 * Convenience method to add a material, then a transform, then a geometry
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
 * Construct a unit box.
 * @param  unitSquare  Geometry node to use
 */
SceneNode* ConstructUnitBox(UnitSquareFlatSurface* unitSquare)
{
   // Contruct transform nodes for the sides of the box.
   // Perform rotations so the sides face outwards

   // Bottom is rotated 180 degrees so it faces outwards
   TransformNode* bottomTransform = new TransformNode;
   bottomTransform->Translate(0.0f, 0.0f, -0.5f);
   bottomTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);   

   // Back is rotated -90 degrees about x: (z -> y)
   TransformNode* backTransform = new TransformNode;
   backTransform->Translate(0.0f, 0.5f, 0.0f);
   backTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);   

   // Front wall is rotated 90 degrees about x: (y -> z)
   TransformNode* frontTransform = new TransformNode;
   frontTransform->Translate(0.0f, -0.5f, 0.0f);
   frontTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   

   // Left wall is rotated -90 about y: (z -> -x)
   TransformNode* leftTransform = new TransformNode;
   leftTransform->Translate(-0.5f, 0.0f, 00.0f);
   leftTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);

   // Right wall is rotated 90 degrees about y: (z -> x)
   TransformNode* rightTransform = new TransformNode;
   rightTransform->Translate(0.5f, 0.0f, 0.0f);
   rightTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);

   // Top 
   TransformNode* topTransform = new TransformNode;
   topTransform->Translate(0.0f, 0.0f, 0.50f);

   // Create a SceneNode and add the 6 sides of the box.
   SceneNode* box = new SceneNode;
   box->AddChild(backTransform);
   backTransform->AddChild(unitSquare);
   box->AddChild(leftTransform);
   leftTransform->AddChild(unitSquare);
   box->AddChild(rightTransform);
   rightTransform->AddChild(unitSquare);
   box->AddChild(frontTransform);
   frontTransform->AddChild(unitSquare);
   box->AddChild(bottomTransform);
   bottomTransform->AddChild(unitSquare);
   box->AddChild(topTransform);
   topTransform->AddChild(unitSquare);

   return box;
}

/**
 * Construct room as a child of the specified node
 * @param  parent      Parent node
 * @param  unitSquare  Geometry node to use
 */
void ConstructRoom(SceneNode* parent, UnitSquareFlatSurface* unitSquare)
{
   // Contruct transform nodes for the walls. Perform rotations so the 
   // walls face inwards
   TransformNode* floorTransform = new TransformNode;
   floorTransform->Scale(200.0f, 200.0f, 1.0f);

   // Back wall is rotated +90 degrees about x: (y -> z)
   TransformNode* backWallTransform = new TransformNode;
   backWallTransform->Translate(0.0f, 100.0f, 40.0f);
   backWallTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   
   backWallTransform->Scale(200.0f, 80.0f, 1.0f);

   // Front wall is rotated -90 degrees about x: (z -> y)
   TransformNode* frontWallTransform = new TransformNode;
   frontWallTransform->Translate(0.0f, -100.0f, 40.0f);
   frontWallTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);   
   frontWallTransform->Scale(200.0f, 80.0f, 1.0f);

   // Left wall is rotated 90 degrees about y: (z -> x)
   TransformNode* leftWallTransform = new TransformNode;
   leftWallTransform->Translate(-100.0f, 0.0f, 40.0f);
   leftWallTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);
   leftWallTransform->Scale(80.0f, 200.0f, 1.0f);

   // Right wall is rotated -90 about y: (z -> -x)
   TransformNode* rightWallTransform = new TransformNode;
   rightWallTransform->Translate(100.0f, 0.0f, 40.0f);
   rightWallTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);
   rightWallTransform->Scale(80.0f, 200.0f, 1.0f);

   // Ceiling is rotated 180 about x so it faces inwards
   TransformNode* ceilingTransform = new TransformNode;
   ceilingTransform->Translate(0.0f, 0.0f, 80.0f);
   ceilingTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);
   ceilingTransform->Scale(200.0f, 200.0f, 1.0f); 

   // Floor should be tan, mostly dull
   PresentationNode* floorMaterial = new PresentationNode;
   floorMaterial->SetMaterialAmbientAndDiffuse(Color4(0.3f, 0.45f, 0.1f));
   floorMaterial->SetMaterialSpecular(Color4(0.1f, 0.1f, 0.1f));
   floorMaterial->SetMaterialShininess(2.0f);

   // Make the walls reddish, slightly shiny
   PresentationNode* wallMaterial = new PresentationNode;
   wallMaterial->SetMaterialAmbientAndDiffuse(Color4(0.7f, 0.55f, 0.55f));
   wallMaterial->SetMaterialSpecular(Color4(0.4f, 0.4f, 0.4f));
   wallMaterial->SetMaterialShininess(16.0f);

   // Ceiling should be white, moderately shiny
   PresentationNode* ceilingMaterial = new PresentationNode;
   ceilingMaterial->SetMaterialAmbientAndDiffuse(Color4(1.0f, 1.0f, 1.0f));
   ceilingMaterial->SetMaterialSpecular(Color4(0.9f, 0.9f, 0.9f));
   ceilingMaterial->SetMaterialShininess(64.0f);

   // Add floo and ceiling to the parent. Use convenience method to add material,
   // then presentation, then geometry.
   AddSubTree(parent, floorMaterial,   floorTransform,   unitSquare);
   AddSubTree(parent, ceilingMaterial, ceilingTransform, unitSquare);

   // Walls. We can group these all under a single presentation node.
   parent->AddChild(wallMaterial);
   wallMaterial->AddChild(backWallTransform);
   backWallTransform->AddChild(unitSquare);
   wallMaterial->AddChild(leftWallTransform);
   leftWallTransform->AddChild(unitSquare);
   wallMaterial->AddChild(rightWallTransform);
   rightWallTransform->AddChild(unitSquare);
   wallMaterial->AddChild(frontWallTransform);
   frontWallTransform->AddChild(unitSquare);
}

/**
 * Construct table
 * @param  unitSquare  Geometry node to use for table top
 * @param  legs        Geometry node to use for legs
 * @return  Returns a scene node representing the table
 */
SceneNode* ConstructTable(SceneNode* box, ConicSurface* leg)
{
   // Table legs (relative to center of table)
   TransformNode* lfLegTransform = new TransformNode;
   lfLegTransform->Translate(-20.0f, -10.0f, 10.0f);
   lfLegTransform->Scale(3.0f, 3.0f, 20.0f);
   TransformNode* lrLegTransform = new TransformNode;
   lrLegTransform->Translate(-20.0f, 10.0f, 10.0f);
   lrLegTransform->Scale(3.0f, 3.0f, 20.0f);
   TransformNode* rfLegTransform = new TransformNode;
   rfLegTransform->Translate(20.0f, -10.0f, 10.0f);
   rfLegTransform->Scale(3.0f, 3.0f, 20.0f);
   TransformNode* rrLegTransform = new TransformNode;
   rrLegTransform->Translate(20.0f, 10.0f, 10.0f);
   rrLegTransform->Scale(3.0f, 3.0f, 20.0f);
   
   // Construct dimensions for the table top
   TransformNode* topTransform = new TransformNode;
   topTransform->Translate(0.0f, 0.0f, 23.0f);
   topTransform->Scale(60.0f, 30.0f, 6.0f);

   // Create the tree
   SceneNode* table = new SceneNode;
   table->AddChild(topTransform);
   topTransform->AddChild(box);
   table->AddChild(lfLegTransform);
   lfLegTransform->AddChild(leg);
   table->AddChild(rfLegTransform);
   rfLegTransform->AddChild(leg);
   table->AddChild(lrLegTransform);
   lrLegTransform->AddChild(leg);
   table->AddChild(rrLegTransform);
   rrLegTransform->AddChild(leg);

   return table;
}

/**
 * Construct the scene
 */
void ConstructScene()
{
   // Construct the scene root node
   SceneRoot = new SceneNode;

   // Construct the geometry nodes (use vertex buffer objects)
   UnitSquareFlatSurface* unitSquare = new UnitSquareFlatSurface(10, true);

   // Construct a unit box
   SceneNode* box = ConstructUnitBox(unitSquare);

   // Construct a unit cylinder surface
   ConicSurface* cylinder = new ConicSurface(1.0f, 1.0f, 18, 4, true);
  
   // Construct the room as a child of the root node
   ConstructRoom(SceneRoot, unitSquare);

   // Construct the table
   SceneNode* table = ConstructTable(box, cylinder); 

   // Wood
   PresentationNode* wood = new PresentationNode;
   wood->SetMaterialAmbientAndDiffuse(Color4(0.55f, 0.45f, 0.15f));
   wood->SetMaterialSpecular(Color4(0.3f, 0.3f, 0.3f));
   wood->SetMaterialShininess(64.0f);

   // Position the table in the room
   TransformNode* tableTransform = new TransformNode;
   tableTransform->Translate(-50.0f, 50.0f, 0.0f);
   tableTransform->Rotate(30.0f, 0.0f, 0.0f, 1.0f);

   SceneRoot->AddChild(wood);
   wood->AddChild(tableTransform);
   tableTransform->AddChild(table);

   // Gold material
   PresentationNode* silver = new PresentationNode;
   silver->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
   silver->SetMaterialDiffuse(Color4(0.50754f, 0.50754f, 0.50754f));
   silver->SetMaterialSpecular(Color4(0.508273f, 0.508273f, 0.508273f));
   silver->SetMaterialShininess(51.2f);

   // Teapot transform. It is tough to place this - if you make too small then if you look
   // from above the table intersects the bottom, but if you move higher when you look from
   // outside it looks like the teapot is above the table. This is because we don't know the
   // exact dimensions of the teapot.
   TransformNode* teapotTransform = new TransformNode;
   teapotTransform->Translate(0.0f, 0.0f, 30.0f);
   teapotTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);

   // Teapot
   Teapot* teapot = new Teapot;

   tableTransform->AddChild(teapotTransform);
   teapotTransform->AddChild(silver);
   silver->AddChild(teapot);
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
      UpdateView(MouseX, MouseY, Forward);
      glutTimerFunc(1000.0 / FrameRate, timerFunction, 0);
   }
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Load the view matrix
   glLoadMatrixf(MyCamera.GetMatrix().Get());

   // Load light 0 position - basically a modeling transformation
   // to position the light. Positional light at the back, right corner
   GLfloat position0[] = { 90.0f, 90.0f, 30.f, 1.0f };
   glLightfv(GL_LIGHT0, GL_POSITION, position0);

   // Load light 1. This is a directional light from above
   GLfloat position1[] = { 0.0f, 0.0f, 1.0f, 0.0f };
   glLightfv(GL_LIGHT1, GL_POSITION, position1);

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
      MyCamera.SetPosition(Point3(0.0f, -100.0f, 20.0f));
      MyCamera.SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
      MyCamera.SetViewUp(Vector3(0.0, 0.0, 1.0));
      glutPostRedisplay();
      break;

    // Roll the camera by 5 degrees
    case 'r':
      MyCamera.Roll(5);
      glutPostRedisplay();
	   break;

    // Roll the camera by 5 degrees (clockwise)
    case 'R':
      MyCamera.Roll(-5);
      glutPostRedisplay();
	   break;

    // Change the pitch of the camera by 5 degrees
    case 'p':
      MyCamera.Pitch(5);
      glutPostRedisplay();
	   break;

    // Change the pitch of the camera by 5 degrees (clockwise)
    case 'P':
      MyCamera.Pitch(-5);
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees
    case 'h':
      MyCamera.Heading(5);
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees (clockwise)
    case 'H':
      MyCamera.Heading(-5);
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
         UpdateView(x, y, true);
         
         // Set update
         glutTimerFunc(1000.0 / FrameRate, timerFunction, 0);
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
         UpdateView(x, y, true);

         // Set update
         glutTimerFunc(1000.0 / FrameRate, timerFunction, 0);
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
   UpdateView(x, y, true);
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

   // Reset the perspective projection to reflect the change of aspect ratio 
   MyCamera.ChangeAspectRatio((double)width / (double)height);
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
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutKeyboardFunc(keyboard);

   // Initialize GLEW and see if OpenGL 2.0 is supported. Must eb done after glutCreateWindow
   glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("OpenGL 2.0 is supported\n");

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

   // Initialize the view and set a perspective projection
   MyCamera.SetPosition(Point3(0.0f, -100.0f, 20.0f));
   MyCamera.SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
   MyCamera.SetViewUp(Vector3(0.0, 0.0, 1.0));
   MyCamera.SetPerspective(50.0, 1.0, 1.0, 300.0);

   // Initialize lighting
   ConstructLighting();

   // Construct scene
   ConstructScene();

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

   glutMainLoop();
   return 0;
}