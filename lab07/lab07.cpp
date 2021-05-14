#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <algorithm>
#include "ObjParser.h"

static int StartX = 0;
static int StartY = 0;

static bool ShouldRotate = false;
bool eyeLight = true;
bool pointLight = false;
bool spotLight = false;
bool attenuation = false;

int matherial = 0;
static ObjParser Parser;
int subMenuLight, mainMenu, subMenuMatherial;
void Reshape(int w, int h) {
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void Matherial() {

	GLfloat defaultAmbient[] = { 0.2,0.2,0.2,1.0 };
	GLfloat defaultDiffuse[] = { 0.8,0.8,0.8,1.0 };
	GLfloat defaultSpecular[] = { 0.0,0.0,0.0,1.0 };
	GLfloat defaultEmmision[] = { 0.0,0.0,0.0,1.0 };
	GLfloat sh[] = { 0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sh);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmmision);

	if (matherial == 1) {
		GLfloat myDiffuse[] = { 0.9, 0.9, 0.8, 1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, myDiffuse);
		GLfloat myAmb[] = { 0.0, 0.0, 0.2, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, myAmb);
		GLfloat mySpec[] = { 0.3, 0.3, 0.3, 1.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, mySpec);
		GLfloat sh[] = { 128 };
		glMaterialfv(GL_FRONT, GL_SHININESS, sh);
		GLfloat myEmm[] = { 0.3, 0.3, 0.4, 1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, myEmm);
	}
	if (matherial == 2) {
		GLfloat myDiffuse[] = { 0.1, 0.9, 0.1, 1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, myDiffuse);
		GLfloat myAmb[] = { 0.1, 0.1, 0.9, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, myAmb);
	}

}

void Light() {
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);

	GLfloat amb0[] = { 0.0,0.0,0.0,1.0 };

	GLfloat diff0[] = { 0.2,0.15,0.45, 1.0 };
	GLfloat diff1[] = { 0.5,0.1,0.1, 1.0 };
	GLfloat diffWHITE[] = { 0.8,0.8,0.8, 1.0 };

	GLfloat spec0[] = { 1.0,0.8,1.0,1.0 };

	GLfloat position0[] = { 0,0,0,1 };//начальное положение источника света
	GLfloat position1[] = { 0, 0 ,-3,1 };

	GLfloat dir[] = { 0.0,0.0,-1.0 };// направление прожектора


	glMatrixMode(GL_MODELVIEW);//очистка буферов глубины и цвета
	glLoadIdentity();
	glPushMatrix();

	//направленный свет 


	//точечный источник света 
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	// прожектор

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);// угол пропускания -20
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);// значение Е
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
	if (attenuation) {
		//ослабление света с расстоянием (применяется к точечному источнику света) 
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);

		//ослабление света с расстоянием (применяется к прожектору) 
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.001);
	}
	else {
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
	}
	//источник света из глаза
	glLightfv(GL_LIGHT2, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffWHITE);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spec0);


	//включение света
	glEnable(GL_LIGHTING);
	if (pointLight) {
		glEnable(GL_LIGHT0);
	}
	if (spotLight) {
		glEnable(GL_LIGHT1);
	}
	if (eyeLight) {
		glEnable(GL_LIGHT2);
	}
	glPopMatrix();
}


void Display(void) {

	// очистить буфер цвета и глубины.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	Light();
	Matherial();
	// установить камеру
	gluLookAt(0.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	Parser.displayObject();
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	std::cout << key << std::endl;
	if (key == 27) {
		exit(0);
	}

	if (key == 'd') {

		Parser.translateObject(0.25f, 0.0f, 0.0f);
	}

	if (key == 'a') {
		Parser.translateObject(-0.25f, 0.0f, 0.0f);
	}

	if (key == 'w') {
		Parser.translateObject(0.0f, 0.25f, 0.0f);
	}

	if (key == 's') {
		Parser.translateObject(0.0f, -0.25f, 0.0f);
	}
	if (key == 'q') {

		Parser.translateObject(0.0f, 0.0f, -0.25f);
	}
	if (key == 'e') {
		Parser.translateObject(0.0f, 0.0f, 0.25f);
	}
}

void MouseMovementHandler(int x, int y) {
	if (ShouldRotate) {
		float xDir = 0.0f;
		float yDir = 0.0f;

		if (abs(x - StartX) > 0) {
			xDir = 1.0f;
		}

		if (abs(y - StartY) > 0) {
			yDir = 1.0f;
		}
		Parser.rotateObject((x - StartX) * xDir / 2, -(y - StartY) * yDir / 2);
		StartX = x;
		StartY = y;
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

void menu(int optoin) {}
void menuL(int option) {
	switch (option)
	{
	case 1:
		eyeLight = false;
		pointLight = true;
		spotLight = false;
		attenuation = false;
		break;
	case 2:
		eyeLight = false;
		pointLight = true;
		spotLight = false;
		attenuation = true;
		break;
	case 3:
		eyeLight = false;
		pointLight = false;
		spotLight = true;
		attenuation = false;
		break;
	case 4:
		eyeLight = false;
		pointLight = false;
		spotLight = true;
		attenuation = true;
		break;
	case 5:
		eyeLight = false;
		pointLight = true;
		spotLight = true;
		attenuation = false;
		break;
	case 6:
		eyeLight = true;
		pointLight = false;
		spotLight = false;
		attenuation = false;
		break;
	default:
		break;
	}
}
void menuM(int option) {
	switch (option)
	{
	case 1:
		matherial = 0;
		break;
	case 2:
		matherial = 1;
		break;
	case 3:
		matherial = 2;
		break;
	default:
		break;
	}
}
void createPopupMenu() {
	subMenuLight = glutCreateMenu(menuL);
	glutAddMenuEntry("Точечный источник света", 1);
	glutAddMenuEntry("Точечный источник света с затуханием", 2);
	glutAddMenuEntry("Прожектор", 3);
	glutAddMenuEntry("Прожектор с затуханием", 4);
	glutAddMenuEntry("Прожектор + Точечный источник света", 5);
	glutAddMenuEntry("Свет из глаза", 6);
	subMenuMatherial = glutCreateMenu(menuM);
	glutAddMenuEntry("Базовый", 1);
	glutAddMenuEntry("Металл", 2);
	glutAddMenuEntry("Зеленый материал с синими тенями", 3);
	mainMenu = glutCreateMenu(menu);
	glutAddSubMenu("Свет", subMenuLight);
	glutAddSubMenu("Материал", subMenuMatherial);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
int main(int argc, char** argv) {

	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("lab7");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);

	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(MouseButtonHandler);
	glutMotionFunc(MouseMovementHandler);
	glutPassiveMotionFunc(MouseMovementHandler);



	Parser = ObjParser();

	Parser.parseFile("TV.obj");

	createPopupMenu();
	// основной цикл
	glutMainLoop();

	return 1;
}