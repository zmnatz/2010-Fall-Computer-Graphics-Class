//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    DrawLines.cpp
//	Purpose: OpenGL and GLUT program to draw line segments entered with 
//			   the mouse.  Line intersection calculation is also demonstrated.
//
//============================================================================

#include <iostream>
#include <vector>

#include <GL/glut.h>
#include "geometry/geometry.h"

// Are we drawing a line with the mouse? This global flag allows us to 
// follow mouse motion events and draw if the left button is down.
bool DrawLine = false;

// Current line being drawn (while dragging the mouse)
LineSegment2 CurrentLine;

// List of all previously entered PreviousLines
std::vector<LineSegment2> PreviousLines;

// List of intersection points among PreviousLines (except with CurrentLine). 
// This allows us to store all intersections among "static" PreviousLines so 
// we don't have to calculate them during each redraw.
std::vector<Point2> IntersectionPoints;

/**
 * Draw a list of points.  We will use this to draw the intersect points.
 * @param   pts   List of points (floating point x,y)
 */
void drawPoints(std::vector<Point2>& pts)
{
   if (pts.size() > 0)
   {
      // Use vertex list to draw the points
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, (void*)&pts.front());
      glDrawArrays(GL_POINTS, 0, (GLsizei)pts.size());
      glDisableClientState(GL_VERTEX_ARRAY);
   }
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw all previous PreviousLines. Note that count in glDrawArrays is the
   // number of vertices in the list, not the number of PreviousLines.
   if (PreviousLines.size() > 0)
   {
      // Orange PreviousLines
      glColor3f(1.0f, 0.5f, 0.0f);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, (void*)&PreviousLines[0]);
      glDrawArrays(GL_LINES, 0, (GLsizei)PreviousLines.size()*2);
      glDisableClientState(GL_VERTEX_ARRAY);
   }

   // Draw the current line. Use smooth shading and stipple pattern.
   if (DrawLine)
   {
      glShadeModel(GL_SMOOTH);
      glEnable(GL_LINE_STIPPLE);
      glLineWidth(3.0);
      glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(CurrentLine.A.x, CurrentLine.A.y);
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex2f(CurrentLine.B.x, CurrentLine.B.y);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
      glLineWidth(1.0);
      glShadeModel(GL_FLAT);
   }

   // Draw all the previous intersection points (dark blue)
   glColor3f(0.0f, 0.0f, 0.4f);
   drawPoints(IntersectionPoints);

   // Calculate the intersection of the current line with all others
   if (DrawLine)
   {
      Point2 intersectPt;
      std::vector<Point2> intPts;
      std::vector<LineSegment2>::iterator line;
      for (line = PreviousLines.begin(); line != PreviousLines.end(); line++)
         if (line->Intersect(CurrentLine, intersectPt))
            intPts.push_back(intersectPt);
      drawPoints(intPts);
   }

   // Swap buffers (no need for glFlush since glutSwapBuffers does that)
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

    // Clear the list of PreviousLines, intersection points, and the current line. Note use of glutPostRedisplay
    // to force a display callback
    case 'c':
      PreviousLines.clear();
      IntersectionPoints.clear();
      CurrentLine.A.Set(0.0f, 0.0f);
      CurrentLine.B.Set(0.0f, 0.0f);
      glutPostRedisplay();
	   break;

    // Enable anti-aliasing. We can do this here and not within display callback since it is the only
    // time the state changes.
    case 'A':
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_BLEND);
      glutPostRedisplay();
      break;

    // Disable anti-aliasing. We can do this here and not within display callback since it is the only
    // time the state changes.
    case 'a':
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_BLEND);
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
   // On a button down event add a point as the start of a line
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      // Set the current line endpoints (both) to the mouse position
      CurrentLine.A.Set((float)x, (float)y);
      CurrentLine.B.Set((float)x, (float)y);

	   // Force a redisplay
      DrawLine = true;
      glutPostRedisplay();
   }

   // On a button up event add the CurrentLine to the list of PreviousLines
   if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   {
      PreviousLines.push_back(CurrentLine);

      // Get all the intersections of the current line with all other PreviousLines
      //    and add them to the list. This avoids having to calculate 
      //    these intersections again.
      Point2 intersectPt;
      std::vector<LineSegment2>::iterator line;
      for (line = PreviousLines.begin(); line != PreviousLines.end(); line++)
         if (line->Intersect(CurrentLine, intersectPt))
            IntersectionPoints.push_back(intersectPt);

	   // Force a redisplay
      DrawLine = false;
      glutPostRedisplay();
   }
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void mouseMotion(int x, int y)
{
   // Set the current line endpoints to the mouse position
   if (DrawLine)
   {
      CurrentLine.B.Set((float)x, (float)y);
      glutPostRedisplay();
   }
}

/**
 * Reshape callback. Reverse the 0 and height in gluOrtho2D so window
 * coordinates in mouse events can be drawn directly (w/o transformation).
 */
void reshape(int width, int height)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, width, height, 0);  
   glViewport(0, 0, width, height);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   std::cout << "Keyboard Controls:" << std::endl;
   std::cout << "  c - Clear all points" << std::endl;
   std::cout << "  A - Enable anti-aliasing  a - Disable anti-aliasing" 
             << std::endl;
   std::cout << "ESC - Exit program" << std::endl;

   // Double buffer
   glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 480);
   glutInit(&argc, argv);

   glutCreateWindow("Draw PreviousLines by David Nesbitt");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutKeyboardFunc(keyboard);

   // Set the clear color to white
   glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

   // Blending function for anti aliasing. Nio need to reset this - we will use glEnable and glDisable to
   // enable / disable blending 
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Set up a dot line pattern
   glLineStipple(4, 0xaaaa);

   // Points size is constant over entire application.
   glPointSize(5);

   glutMainLoop();
   return 0;
}