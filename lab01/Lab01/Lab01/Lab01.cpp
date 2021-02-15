#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include "Actions.h"

GLint Width = 512;
GLint Height = 512;

GLenum DrawMethod = GL_POINTS;

int RectSize = 200;
int mainMenu, DrawModeMenu;

void Display(void) {
	float x1 = (Width - RectSize) / 2;
	float x2 = x1 + RectSize;
	float y1 = (Height - RectSize) / 2;
	float y2 = y1 + RectSize;
	float x3 = (Width - RectSize / 2) / 2;
	float x4 = x3 + RectSize;
	float y3 = (Height - RectSize / 2) / 2;
	float y4 = y3 + RectSize;

	glClearColor(0.25, 0.25, 0.25, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(DrawMethod);
	glColor3f(1, 0, 0);
	glVertex2f(x3, y4);
	glColor3f(0, 1, 0);
	glVertex2f(x2, y1);
	glColor3f(0, 0, 1);
	glVertex2f(x1, y1);
	glColor3f(1, 1, 0);
	glVertex2f(x2, y2);
	glColor3f(1, 0, 1);
	glVertex2f(x3, y3);
	glColor3f(0, 1, 1);
	glVertex2f(x4, y4);
	glColor3f(1, 1, 1);
	glVertex2f(x1, y2);
	glColor3f(0, 0, 0);
	glVertex2f(x4, y3);
	glEnd();

	glFinish();
}

void Reshape(GLint w, GLint h) {
	Width = w;
	Height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ChangeDrawMode(int DrawMode) {
	switch(DrawMode) {
	case ACTION_GL_POINTS:
		DrawMethod = GL_POINTS;
		break;
	case ACTION_GL_LINES:
		DrawMethod = GL_LINES;
		break;
	case ACTION_GL_LINE_STRIP:
		DrawMethod = GL_LINE_STRIP;
		break;
	case ACTION_GL_LINE_LOOP:
		DrawMethod = GL_LINE_LOOP;
		break;
	case ACTION_GL_TRIANGLES:
		DrawMethod = GL_TRIANGLES;
		break;
	case ACTION_GL_TRIANGLE_STRIP:
		DrawMethod = GL_TRIANGLE_STRIP;
		break;
	case ACTION_GL_TRIANGLE_FAN:
		DrawMethod = GL_TRIANGLE_FAN;
		break;
	case ACTION_GL_QUADS:
		DrawMethod = GL_QUADS;
		break;
	case ACTION_GL_QUAD_STRIP:
		DrawMethod = GL_QUAD_STRIP;
		break;
	case ACTION_GL_POLYGON:
		DrawMethod = GL_POLYGON;
		break;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == '\033') {
		exit(0);
	}
	else {
		ChangeDrawMode(key);
	}

}

void processMenuStatus(int status, int x, int y) {}

void processMainMenu(int option) {}

void processDrawModeMenu(int option) {
	ChangeDrawMode(option);
}

void createPopupMenus() {
	DrawModeMenu = glutCreateMenu(processDrawModeMenu);
	glutAddMenuEntry("GL_POINTS", ACTION_GL_POINTS);
	glutAddMenuEntry("GL_LINES", ACTION_GL_LINES);
	glutAddMenuEntry("GL_LINE_STRIP", ACTION_GL_LINE_STRIP);
	glutAddMenuEntry("GL_LINE_LOOP", ACTION_GL_LINE_LOOP);
	glutAddMenuEntry("GL_TRIANGLES", ACTION_GL_TRIANGLES);
	glutAddMenuEntry("GL_TRIANGLE_STRIP", ACTION_GL_TRIANGLE_STRIP);
	glutAddMenuEntry("GL_TRIANGLE_FAN", ACTION_GL_TRIANGLE_FAN);
	glutAddMenuEntry("GL_QUADS", ACTION_GL_QUADS);
	glutAddMenuEntry("GL_QUAD_STRIP", ACTION_GL_QUAD_STRIP);
	glutAddMenuEntry("GL_POLYGON", ACTION_GL_POLYGON);
	mainMenu = glutCreateMenu(processMainMenu);
	glutAddSubMenu("Polygon Mode", DrawModeMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMenuStatusFunc(processMenuStatus);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Lab01");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	createPopupMenus();

	glutMainLoop();
}