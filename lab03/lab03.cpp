// lab03.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdlib.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>

GLint Width = 512;
GLint Height = 512;

int RectSize = 200;

float KnotVector[10] = {0.0f, 1.0f/9.0f, 2.0f/9.0f, 3.0f/9.0f, 4.0f/9.0f, 5.0f/9.0f, 6.0f/9.0f, 7.0f/9.0f, 8.0f/9.0f, 1.0f};
float WeigthVector[6] = { 100.0f, 50.0f, 70.0f, 100.0f, 20.0f, 150.0f };
int P = 3;
int N = 5;

GLfloat ctrlpoints[6][2] = { {20.0f, 20.0f}, {100.0f, 100.0f}, 
							 {120.0f, 10.0f}, {150.0f, 60.0f}, 
							 {190.0f, 30.0f}, {170.0f, 100.0f} };


float basisFunc(int k, int p, float t) {
	if (k < 0 || p < 0 || k > 8) {
		std::cout << k << "\t" << p << std::endl;
	}
	if (p == 0) {
		if (KnotVector[k] <= t && t > KnotVector[k + 1]) {
			return 1.0f;
		}
		else {
			return 0.0f;
		}
	}

	float f = (t - KnotVector[k]) / (KnotVector[k + p] - KnotVector[k]);
	float g = (KnotVector[k + p + 1] - t) / (KnotVector[k + p + 1] - KnotVector[k + 1]);
	return f * basisFunc(k, p - 1, t) + g * basisFunc(k + 1, p - 1, t);
}

float* NURBSSpline(float t) {
	float x = 0.0f;
	float y = 0.0f;
	float basisWeigthSum = 0.0f;
	for (int i = 0; i <= N; i++) {
		float coeff = basisFunc(i, P, t) * WeigthVector[i];
		basisWeigthSum += coeff;
		x += ctrlpoints[i][0] * coeff;
		y += ctrlpoints[i][1] * coeff;
	}
	float answer[2] = { x / basisWeigthSum, y / basisWeigthSum };
	std::cout << answer[0] << "\t" << answer[1] << std::endl;
	return answer;
}

//Beize stuff

//GLfloat ctrlpoints[6][3] = {
//		{ 80.0, 40.0, 0.0}, { 120.0, 120.0, 0.0},
//		{60.0, 120.0, 0.0}, {100.0, 40.0, 0.0} , 
//		{150.0, 200.0f, 0.0f}, {200.0f, 60.0f, 0.0f} };
//
//int m = 5;
//
//int factorial(int x) {
//	if (x == 0 || x == 1) {
//		return 1;
//	}
//	int result = 1;
//	for (int i = x; i > 1; i--) {
//		result *= i;
//	}
//	return result;
//}
//
//float* Beize(float t) {
//	float x = 0.0f;
//	float y = 0.0f;
//	float C = 0.0f;
//	for (int i = 0; i <= m; i++) {
//		C = (float)factorial(m) / (1.0f * factorial(i) * factorial(m - i));
//		x += C * powf((float)t, (float)i) * powf((float)(1.0f - t), (float)(m - i)) * ctrlpoints[i][0];
//		y += C * powf((float)t, (float)i) * powf((float)(1.0f - t), (float)(m - i)) * ctrlpoints[i][1];
//	}
//	float r[2] = {x, y};
//	return r;
//}

void Display(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int i;

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 30; i++)
		glVertex2fv(NURBSSpline((float)i / 30.0f));
	glEnd();
	glPointSize(5.0);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= N; i++)
		glVertex2fv(ctrlpoints[i]);
	glEnd();
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

int main(int argc, char* argv[]) {
	srand(time(0));
	for (int i = 0; i <= N; i++) {
		WeigthVector[i] = rand() % 150 + 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Lab03");

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	glutMainLoop();
}
