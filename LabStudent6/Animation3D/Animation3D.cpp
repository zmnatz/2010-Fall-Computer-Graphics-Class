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
#include <stdarg.h>
#include <vector>

#include <GL/glut.h>
#include "geometry/geometry.h"
#include "scene/scene.h"
#include "ColorNode.h"
#include "UnitSquareSurface.h"
#include "UnitSphere.h"
#include "BallTransform.h"

const unsigned int NUMBER_OF_BALLS       = 9;
const unsigned int NUMBER_OF_RED_BALLS   = 3;
const unsigned int NUMBER_OF_GREEN_BALLS = 3;
const unsigned int NUMBER_OF_BLUE_BALLS  = 3;

int FrameID = 0;
const float FRAMES_PER_SEC = 72.0f;

// Root of the scene graph
SceneNode* SceneRoot;

// Bounding planes of the enclosure. Used for intersection testing.
std::vector<Plane> BoundingPlanes;

// Ball transforms. We have this global so we can more easily do intersection testing
BallTransform* BallTransforms[NUMBER_OF_BALLS];

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("AnimatedBalls.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

/**
 * Use a timer method to try to do a consistent update rate.
 * Without using a timer, the speed of movement will depend on how fast
 * the program runs (fast movement on a fast PC and slow movement on a
 * slower PC)
 */
void timerFunction(int value)
{
   // Initialize all balls to have no intersection
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
      BallTransforms[i]->SetIntersectTime(0.0f);

   // Go through all balls and test for intersection with subsequent ball
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
   {
      // If intersection with a prior ball is not found, test for intersection with successive balls
      if (BallTransforms[i]->GetIntersectTime() == 0.0f)
      {
         for (unsigned int j = i+1; j < NUMBER_OF_BALLS; j++)
         {
            // If an intersection occurs, break out of loop. We will only worry about a ball intersecting
            // one other in a single frame and won't care much if it is the closest
            if (BallTransforms[i]->IntersectBall(BallTransforms[j]))
               break;
         }
      }
   }

   // Go through all ball and test for plane intersection on those that do not intersect with another ball
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
   {
      // Check for collision with any planes
      if (BallTransforms[i]->GetIntersectTime() == 0.0f)
      {
         float t = 0.0f;
         float smallestT = 1.0f;
         std::vector<Plane>::iterator plane = BoundingPlanes.begin();
         Plane intersectPlane;
         for ( ; plane != BoundingPlanes.end(); plane++)
         {
            t = BallTransforms[i]->IntersectWithPlane(*plane);
            if (t < smallestT)
            {
               // Copy the nearest intersection and the plane of intersection
               smallestT = t;
               intersectPlane = *plane;
            }
         }
         if (smallestT != 1.0f)
         {
            BallTransforms[i]->SetIntersectTime(smallestT);
            BallTransforms[i]->SetIntersectPlane(intersectPlane);
         }
      }
   }

   // Update the scene graph
   SceneRoot->Update();

   // Set update to specified frames per second
   glutTimerFunc(1000 / FRAMES_PER_SEC, timerFunction, 0);

   glutPostRedisplay();
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
   float worldAspect = 1.0f; 
   float windowAspect = (float)width / (float)height;
   if (windowAspect > worldAspect)
   {
      // Window is wider than world
      float w = height * worldAspect;
      float startX = (width - w) * 0.5f;
      glViewport(startX, 0, w, height);
   }
   else
   {
      // Window is taller than world
      float h = width / worldAspect;
      float startY = (height - h) * 0.5f;
      glViewport(0, startY, width, h);
   }
}

/**
 * Construct the scene
 */
void ConstructScene()
{
   // Form bounding planes (for intersection testing)
   Plane p0(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
   BoundingPlanes.push_back(p0);
   Plane p1(Point3(0.0f, 0.0f, 100.0f), Vector3(0.0f, 0.0f, -1.0f));
   BoundingPlanes.push_back(p1);
   Plane p2(Point3(-50.0f, 0.0f, 50.0f), Vector3(1.0f, 0.0f, 0.0f));
   BoundingPlanes.push_back(p2);
   Plane p3(Point3(50.0f, 0.0f, 50.0f), Vector3(-1.0f, 0.0f, 0.0f));
   BoundingPlanes.push_back(p3);
   Plane p4(Point3(0.0f, 50.0f, 50.0f), Vector3(0.0f, -1.0f, 0.0f));
   BoundingPlanes.push_back(p4);
   Plane p5(Point3(0.0f, -50.0f, 50.0f), Vector3(0.0f, 1.0f, 0.0f));
   BoundingPlanes.push_back(p5);

   // Construct the geometry objects
   UnitSquareSurface* unitSquare = new UnitSquareSurface;
   UnitSphere* unitSphere = new UnitSphere();

   // Contruct transform nodes for the walls. Perform rotations so the 
   // walls face inwards
   TransformNode* floorTransform = new TransformNode;
   floorTransform->Scale(100.0f, 100.0f, 1.0f);

   // Back wall is rotated +90 degrees about x: (y -> z)
   TransformNode* backWallTransform = new TransformNode;
   backWallTransform->Translate(0.0f, 50.0f, 50.0f);
   backWallTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   
   backWallTransform->Scale(100.0f, 100.0f, 1.0f);

   // Left wall is rotated 90 degrees about y: (z -> x)
   TransformNode* leftWallTransform = new TransformNode;
   leftWallTransform->Translate(-50.0f, 0.0f, 50.0f);
   leftWallTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);
   leftWallTransform->Scale(100.0f, 100.0f, 1.0f);

   // Right wall is rotated -90 about y: (z -> -x)
   TransformNode* rightWallTransform = new TransformNode;
   rightWallTransform->Translate(50.0f, 0.0f, 50.0f);
   rightWallTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);
   rightWallTransform->Scale(100.0f, 100.0f, 1.0f);

   TransformNode* ceilingTransform = new TransformNode;
   ceilingTransform->Translate(0.0f, 0.0f, 100.0f);
   ceilingTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);
   ceilingTransform->Scale(100.0f, 100.0f, 1.0f); 

   // Construct transform nodes for the balls. Each one has a 
   // random initial position with x,y values between -40 and 40
   // and z values between 25 and 75. A random radius between 3 and 
   // 7 units, a random initial direction vectors (unit length) and 
   // random velocity between 5 and 15 units per second is used.
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
      BallTransforms[i] = new BallTransform(FRAMES_PER_SEC);

   // Construct color nodes for the walls, floor, and ceiling
   ColorNode* floorColor    = new ColorNode(0.6f, 0.5f, 0.2f);
   ColorNode* backWallColor = new ColorNode(0.9f, 0.7f, 0.5f);
   ColorNode* wallColor     = new ColorNode(1.0f, 1.0f, 1.0f);
   ColorNode* ceilingColor  = new ColorNode(0.1f, 0.4f, 1.0f);

   // Construct color nodes for the balls
   ColorNode* redBall   = new ColorNode(1.0f, 0.0f, 0.0f);
   ColorNode* greenBall = new ColorNode(0.0f, 1.0f, 0.0f);
   ColorNode* blueBall  = new ColorNode(0.0f, 0.0f, 1.0f);

   // Construct the scene layout
   SceneRoot = new SceneNode;

   // Walls
   SceneRoot->AddChild(backWallColor);
   backWallColor->AddChild(backWallTransform);
   backWallTransform->AddChild(unitSquare);
   SceneRoot->AddChild(wallColor);
   wallColor->AddChild(leftWallTransform);
   leftWallTransform->AddChild(unitSquare);
   wallColor->AddChild(rightWallTransform);
   rightWallTransform->AddChild(unitSquare);
   SceneRoot->AddChild(floorColor);
   floorColor->AddChild(floorTransform);
   floorTransform->AddChild(unitSquare);
   SceneRoot->AddChild(ceilingColor);
   ceilingColor->AddChild(ceilingTransform);
   ceilingTransform->AddChild(unitSquare);

   // Add a unit sphere child node to each ball transform node 
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
      BallTransforms[i]->AddChild(unitSphere);

   // Add ball transforms under each ball presentation node
   unsigned int j = 0;
   SceneRoot->AddChild(redBall);
   for (unsigned int i = 0; i < NUMBER_OF_RED_BALLS; i++)
      redBall->AddChild(BallTransforms[j++]);
   SceneRoot->AddChild(greenBall);
   for (unsigned int i = 0; i < NUMBER_OF_GREEN_BALLS; i++)
      greenBall->AddChild(BallTransforms[j++]);
   SceneRoot->AddChild(blueBall);
   for (unsigned int i = 0; i < NUMBER_OF_BLUE_BALLS; i++)
      blueBall->AddChild(BallTransforms[j++]);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   // Set up depth buffer and double buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 640);
   glutInit(&argc, argv);

   // Set callback methods
   glutCreateWindow("Animated Balls");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   // Set the clear color to black. Any part of the window outside the
   // viewport should appear black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Construct scene. Seed the random number generator
   srand(929233);
   ConstructScene();

   // Enable depth testing
   glEnable(GL_DEPTH_TEST);

   // Enable back face polygon removal
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

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

   // Set update rate
   glutTimerFunc(1000 / FRAMES_PER_SEC, timerFunction, 0);

   glutMainLoop();
   return 0;
}