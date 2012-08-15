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
#include <list>
using namespace std;

#include <GL/glut.h>
#include "geometry/geometry.h"

list<LineSegment2> lines;
LineSegment2 curSeg;
bool drawingLine(false);

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


void drawLines(void)
{
	glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);

	list<LineSegment2>::iterator i;
	for(i=lines.begin(); i != lines.end(); ++i){
		glBegin(GL_LINES);
			glVertex2f((*i).A.x,(*i).A.y);
			glVertex2f((*i).B.x,(*i).B.y);
		glEnd();
	}
	if(drawingLine){
		glBegin(GL_LINES);
			glVertex2f(curSeg.A.x,curSeg.A.y);
			glVertex2f(curSeg.B.x,curSeg.B.y);
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'c':
		lines.clear();
		break;
	case 'A':
		if(glIsEnabled(GL_LINE_SMOOTH)){
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_BLEND);
		}else{
			glEnable(GL_LINE_SMOOTH); // Enables line anti-aliasing
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_BLEND); // Requires blending
		}
		break;
	case 0x1B:	//Escape key
      exit(0);
    break;
  }
}

void mouseMove(int x,int y){
	curSeg.B.Set(x,y);
}

void mouseClick(int btn,int state, int x,int y){
	if(btn==GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			Point2 p(x,y);
			curSeg.A.Set(x,y);
			curSeg.B=curSeg.A;
			drawingLine=true;
		}else{
			lines.push_front(curSeg);
			drawingLine=false;
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
	glutInit(&argc, argv);
	glutInitWindowSize(initWidth,initHeight);
	glutInitWindowPosition (100, 100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Draw Lines - Zaven Mnatzakanian");
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawLines);	
	glutIdleFunc(drawLines);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Best quality

	glutMainLoop();
   return 0;
}