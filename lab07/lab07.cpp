#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <algorithm>
#include "ObjParser.h"

static int StartX = 0;
static int StartY = 0;

static bool ShouldRotate = false;

static ObjParser Parser;

void Reshape(int w, int h) {
	// предотвращение деления на ноль
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	// используем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	// обнуляем матрицу
	glLoadIdentity();
	// установить параметры вьюпорта
	glViewport(0, 0, w, h);
	// установить корректную перспективу
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// вернуться к матрице проекции
	glMatrixMode(GL_MODELVIEW);
}

void Display(void) {

	// очистить буфер цвета и глубины.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// установить камеру
	gluLookAt(0.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	Parser.displayObject();
	//glutSolidCube(2);
	/*glBegin(GL_TRIANGLES);
	glVertex3f(-2.0f, -2.0f, 10.0f);
	glVertex3f(0.0f, 2.0f, 0.0);
	glVertex3f(2.0f, -2.0f, 0.0);
	glEnd();*/

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}

	if (key == 'd') {
		Parser.translateObject(0.25f, 0.0f);
	}

	if (key == 'a') {
		Parser.translateObject(-0.25f, 0.0f);
	}

	if (key == 'w') {
		Parser.translateObject(0.0f, 0.25f);
	}

	if (key == 's') {
		Parser.translateObject(0.0f, -0.25f);
	}
}

void MouseMovementHandler(int x, int y) {
	if (ShouldRotate) {
		float xDir = 0.0f;
		float yDir = 0.0f;

		std::cout << abs(x - StartX) << "\t" << abs(y - StartY) << std::endl;
		if (abs(x - StartX) >= 20) {
			xDir = 1.0f;
		}

		if (abs(y - StartY) >= 20) {
			yDir = 1.0f;
		}
		Parser.rotateObject((x - StartX) * xDir / 100.0f, -(y - StartY) * yDir / 100.0f);
	}
	else
	{
		StartX = x;
		StartY = y;

	}
}

void MouseButtonHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		ShouldRotate = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		ShouldRotate = false;
	}
}

int main(int argc, char** argv) {

	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Урок 4");

	// регистрация
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);

	// наши новые функции
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(MouseButtonHandler);
	glutMotionFunc(MouseMovementHandler);
	glutPassiveMotionFunc(MouseMovementHandler);
	glEnable(GL_DEPTH_TEST);

	GLfloat amb0[] = { 0.2,0.4,0.6,1.0 };
	GLfloat diff0[] = { 0.8,0.9,0.5,1.0 };
	GLfloat spec0[] = { 1.0,0.8,1.0,1.0 };
	//привязываем их к LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	Parser = ObjParser();

	Parser.parseFile("TV.obj");


	// основной цикл
	glutMainLoop();

	return 1;
}