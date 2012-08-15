//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.461 Principles of Computer Graphics
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

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("DrawLines.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

std::vector<LineSegment2> lines;
std::vector<Point2> intersectPts;
LineSegment2 curSeg;
std::vector<Point2> curPts;
bool drawingLine(false);
bool refreshNeeded(false);

//Draws a set of points (unless its an empty set)
void drawPts(std::vector<Point2> pts){
	if(!pts.empty()){
		glColor3f(0,0,.5);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,sizeof(Point2),&pts[0].x);
		glDrawArrays(GL_POINTS,0,pts.size());
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

//If something has changed, clears the display and redraws everything
void drawDisplay(void)
{
	if(refreshNeeded){
		//Clear the display
		glClearColor(1,1,1,0);
		glClear(GL_COLOR_BUFFER_BIT);

		//If there are lines saved, draw them
		if(!lines.empty()){
			glLineWidth(1);
			glColor3f(1,.5,0);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2,GL_FLOAT,sizeof(Point2),&lines[0].A.x);
			glDrawArrays(GL_LINES,0,lines.size()*2);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		//Draw intersect points
		drawPts(intersectPts);
	
			//If mouse is down, draw current line and its intersect points
		if(drawingLine){
			glLineWidth(3);
			glEnable(GL_LINE_STIPPLE);
			glColor3f(1,0,0);
			glBegin(GL_LINES);
				glVertex2f(curSeg.A.x,curSeg.A.y);
				glColor3f(0,1,0);
				glVertex2f(curSeg.B.x,curSeg.B.y);
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			//Draw intersect points with the current line being drawn
			drawPts(curPts);
		}
		glFlush();
		glutSwapBuffers();
		refreshNeeded = false;
	}
}

//Handles keyboard actions
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'c':								//Clear lines and intersect points
		lines.clear();			
		intersectPts.clear();
		curPts.clear();
		refreshNeeded=true;
		break;
	case 'A':								//Toggle anti-aliasing
		refreshNeeded=true;
		if(glIsEnabled(GL_LINE_SMOOTH)){	
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_BLEND);
		}else{
			glEnable(GL_LINE_SMOOTH);		// Enables line anti-aliasing
			glEnable(GL_POINT_SMOOTH);		//Enable pt anti-aliasing
			glEnable(GL_BLEND);				// Requires blending
		}
		break;
	case 0x1B:	//Escape key
      exit(0);
    break;
  }
}

//Adjusts the line currently being drawn when the mouse moves, 
//recalculates intersect points
void mouseMove(int x,int y){
	curSeg.B.Set(x,y);
	refreshNeeded=true;
	if(!lines.empty() || curSeg.A==curSeg.B){		//Doesn't need to check if there's no lines or curSeg has no magnitude
		if(!curPts.empty()) curPts.clear();			//Erases existing intersectPts

		//Find IntersectPts by iterating through the other segments on display
		Point2 intersectPt;
		std::vector<LineSegment2>::iterator i;
		for(i=lines.begin(); i != lines.end(); ++i){
			if(curSeg.Intersect(*i,intersectPt))
				curPts.push_back(intersectPt);
		}
	}
}

//Registers mouse up and mouse down
//Mouse Up: Set currentSeg to the given x and y, set drawingLine state to true
//Mouse Down: Add currentSeg to the main line list, Add intersectPts to permanent list
//				set drawingLine state to false
void mouseClick(int btn,int state, int x,int y){
	if(btn==GLUT_LEFT_BUTTON){
		refreshNeeded=true;
		if(state == GLUT_DOWN){			//Mouse Down, start drawing a new line
			curSeg.A.Set(x,y);
			curSeg.B=curSeg.A;
			drawingLine=true;
		}else{
			drawingLine=false;
			lines.push_back(curSeg);
			if(!curPts.empty())
				intersectPts.insert(intersectPts.end(),curPts.begin(),curPts.end());
		}
	}
}

void reshape(int width,int height){
	// Reset the viewport to full window dimensions
	glViewport(0, 0, width, height);
	// Set world coordinates to match window coordinates
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,height,0);
	refreshNeeded=true;
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

	//Initialize System
	glutInit(&argc, argv);
	glutInitWindowSize(640,480);
	glutInitWindowPosition (100, 100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Draw Lines - Zaven Mnatzakanian");

	//Declare Listeners
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawDisplay);	
	glutIdleFunc(drawDisplay);
	
	//Environment Settings
	glLineStipple(1,0x00FF);
	glPointSize(5);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Best quality

	glutMainLoop();	//Loop
	return 0;
}