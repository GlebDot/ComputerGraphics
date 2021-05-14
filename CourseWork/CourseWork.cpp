#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <algorithm>
#include "ObjParser.h"

#define _USE_MATH_DEFINES
#include <math.h>

static int StartX = 0;
static int StartY = 0;

static bool ShouldRotate = false;
bool eyeLight = true;
bool pointLight = false;
bool spotLight = false;
bool attenuation = false;

float pitch = M_PI_4;
float yaw = M_PI_4;
float eyeDistance = 10.0f;
float eyeLoc[3] = { 0.0f, 1.0f, 10.0f };
float eyeDelta[3] = { 0.0f, 0.0f, 0.0f };

float ligthPositionOffset[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

int matherial = 0;
int subMenuLight, mainMenu, subMenuMatherial;

std::vector<ObjParser*> Objects;

void recomputeEyeLoc() {
	eyeLoc[0] = eyeDistance * sin(pitch) * cos(yaw);
	eyeLoc[2] = eyeDistance * sin(pitch) * sin(yaw);
	eyeLoc[1] = eyeDistance * cos(pitch);
}

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

//@TODO Настрой свет
void Light() {
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);

	GLfloat amb0[] = { 0.0,0.0,0.0,1.0 };

	GLfloat diff0[] = { 0.2,0.15,0.45, 1.0 };
	GLfloat diff1[] = { 0.5,0.1,0.1, 1.0 };
	GLfloat diffWHITE[] = { 0.8,0.8,0.8, 1.0 };

	GLfloat spec0[] = { 1.0,0.8,1.0,1.0 };

	//Настрой позиции для каждого цвета отдельно, но для каждой координаты прибавь ligthPositionOffset. 
	//Изменяй только ligthPositionOffset при нажатии клавиш например.

	GLfloat position0[] = { 0 + ligthPositionOffset[0], 0 + ligthPositionOffset[1], 0 + ligthPositionOffset[2], 1 };//начальное положение источника света
	GLfloat position1[] = { 0 + ligthPositionOffset[0], 0 + ligthPositionOffset[1],-3 + ligthPositionOffset[2], 1 };

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
	Light();
	
	// установить камеру
	gluLookAt(eyeLoc[0], eyeLoc[1], eyeLoc[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	for (auto& object : Objects) {
		object->displayObject();
	}
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	std::cout << key << std::endl;
	if (key == 27) {
		exit(0);
	}

	if (key == 'w') {
		eyeDistance -= 0.1f;
		recomputeEyeLoc();
	}

	if (key == 's') {
		eyeDistance += 0.1f;
		recomputeEyeLoc();
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

		float savePitch = pitch;

		yaw += (x - StartX) * xDir / 100.0f;
		pitch -= (y - StartY) * yDir / 100.0f;

		if (pitch > M_PI || pitch < 0) {
			pitch = savePitch;
		}

		recomputeEyeLoc();

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



int main(int argc, char** argv) {

	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 4800);
	glutCreateWindow("CourseWork");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);

	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(MouseButtonHandler);
	glutMotionFunc(MouseMovementHandler);
	glutPassiveMotionFunc(MouseMovementHandler);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);


	//@TODO Настрой объекты
	//Создаем объекты
	ObjParser Parser1 = ObjParser();
	ObjParser Parser2 = ObjParser();

	Parser1.parseFile("1.obj");
	Parser2.parseFile("2.obj");

	//Настраиваем  материалы
	MaterialInfo mat1 = MaterialInfo();
	mat1.ambient = { 0.1, 0.6, 0.2, 1.0 };
	mat1.diffuse = { 0.1, 0.6, 0.2, 1.0 };
	mat1.emmision = { 0.1, 0.2, 0.1, 1.0 };
	mat1.shininess = { 0.2, 0.2, 0.2, 0.2 };
	mat1.specular = { 0.75, 0.75, 0.75, 1.0 };

	//Каждому объекту присваиваем свой материал
	Parser1.material = mat1;

	//Пушим все объекты в массив объектов, который нужно отображать
	Objects.push_back(&Parser1);
	Objects.push_back(&Parser2);

	recomputeEyeLoc();

	// основной цикл
	glutMainLoop();

	return 1;
}