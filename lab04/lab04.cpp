#include <glut.h>
#include <string>
#include <iostream>

//Для подключения математических функций
#define _USE_MATH_DEFINES
#include <math.h>

//Размеры окна
GLint Width = 512;
GLint Height = 512;

//Угол поворта
float Angle = M_PI_4;
//Аксиома
std::string Axiom = "BABA";
//Правила для первичных букв
std::string Rule_A = "CBAZ";
std::string Rule_B = "ZBAD";
std::string Rule_C = "ECDA";
std::string Rule_D = "BCDE";
std::string Rule_E = "CBAD";
std::string Rule_Z = "BCDA";

//Правила для перевода первичных букв в рисующие
std::string Rule_AB_TO_F = "+F+";
std::string Rule_CD_TO_F = "-F-";
std::string Rule_EZ_TO_F = "FF";

//функция поворота 
void RotateDirection(float* direction, float clockwise) {
	float x = direction[0];
	float y = direction[1];

	direction[0] = x * cos(Angle) + clockwise * y * sin(Angle);
	direction[1] = -clockwise * x * sin(Angle) + y * cos(Angle);
}

//Функция для преобразования первичных правил
void CreateSystem(std::string& system) {
	for (int i = 0; i < system.length(); i++) {
		switch (system[i])
		{
		case 'A':
			system.erase(i, 1);
			system.insert(i, Rule_A);
			i += Rule_A.length() - 1;
			break;
		case 'B':
			system.erase(i, 1);
			system.insert(i, Rule_B);
			i += Rule_B.length() - 1;
			break;
		case 'C':
			system.erase(i, 1);
			system.insert(i, Rule_C);
			i += Rule_C.length() - 1;
			break;
		case 'D':
			system.erase(i, 1);
			system.insert(i, Rule_D);
			i += Rule_D.length() - 1;
			break;
		case 'E':
			system.erase(i, 1);
			system.insert(i, Rule_E);
			i += Rule_E.length() - 1;
			break;
		case 'Z':
			system.erase(i, 1);
			system.insert(i, Rule_Z);
			i += Rule_Z.length() - 1;
			break;
		default:
			break;
		}
	}
}

//Функция для преобразования первичных правил в рисующие
void ParseSystemToF(std::string& system) {
	for (int i = 0; i < system.length(); i++) {
		if (system[i] == 'A' || system[i] == 'B') {
			system.erase(i, 1);
			system.insert(i, Rule_AB_TO_F);
			i += Rule_AB_TO_F.length() - 1;
		}

		if (system[i] == 'C' || system[i] == 'D') {
			system.erase(i, 1);
			system.insert(i, Rule_CD_TO_F);
			i += Rule_CD_TO_F.length() - 1;
		}

		if (system[i] == 'E' || system[i] == 'Z') {
			system.erase(i, 1);
			system.insert(i, Rule_EZ_TO_F);
			i += Rule_EZ_TO_F.length() - 1;
		}
	}
}


//Нарисовать линию
void DrawLine(float startX, float startY, float endX, float endY) {
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(startX, startY, 0.0f);
	glVertex3f(endX, endY, 0.0f);
	glEnd();
}

//Функция фрактала
void DrawFractal(float startCoords, float endCoords, int fractions = 2) {
	float size = endCoords - startCoords;
	float lineLength = size / powf(2, fractions);
	std::string system = Axiom;
	for (int i = 1; i < fractions; i++) {
		CreateSystem(system);
	}

	std::cout << system << std::endl;

	ParseSystemToF(system);

	std::cout << system << std::endl;

	float xStart = startCoords;
	float yStart = startCoords;
	float xEnd = 0.0f;
	float yEnd = 0.0f;
	float direction[2] = { 0.0f, 1.0f };
	for (int i = 0; i < system.length(); i++) {
		switch (system[i])
		{
		case '-':
			RotateDirection(direction, -1.0f);
			break;
		case '+':
			RotateDirection(direction, 1.0f);
			break;
		case 'F':
			xEnd = xStart + lineLength * direction[0];
			yEnd = yStart + lineLength * direction[1];
			DrawLine(xStart, yStart, xEnd, yEnd);
			xStart = xEnd;
			yStart = yEnd;
			break;
		default:
			break;
		}
	}
}

//Стандартное
void Display(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int Fractions = 3;

	DrawFractal(400.0f, 800.0f, 1);

	glFlush();
}

void Reshape(GLint w, GLint h) {
	Width = w;
	Height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//King
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Lab03");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}



