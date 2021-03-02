// lab03.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdlib.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>

//Размеры окна
GLint Width = 512;
GLint Height = 512;

//Это узлы, как видишь они распределены равномерно
float KnotVector[10] = {0.0f, 1.0f/9.0f, 2.0f/9.0f, 3.0f/9.0f, 4.0f/9.0f, 5.0f/9.0f, 6.0f/9.0f, 7.0f/9.0f, 8.0f/9.0f, 1.0f};
//Это веса для точек, они от 0 до 1 пусть изменяются, я нигде не нашел должны ли быть у них границы или нет.
float WeigthVector[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

//Это наши стартовые параметры
int P = 3; //Это степень NURBS
int N = 5; //Это вот я не знаю что, но скорее всего это количество контрольных СЕГМЕНТОВ (на один меньше, чем точек, потому что отрезок можно провести только через 2 точки)

//Это массив контрольных точек, их 6, каждая из двух координат.
GLfloat ctrlpoints[6][2] = { {20.0f, 20.0f}, {100.0f, 100.0f}, 
							 {120.0f, 10.0f}, {150.0f, 60.0f}, 
							 {190.0f, 30.0f}, {170.0f, 100.0f} };


//Базисная функция, она реккурентная
float basisFunc(int k, int p, float t) {
	//Это условие выхода из функции
	if (p == 0) {
		//Усли попали в промежуток между k и k+1 узлом, то возвращаем 1, если нет, то 0.
		if (KnotVector[k] <= t && t < KnotVector[k + 1]) {
			return 1.0f;
		}
		else {
			return 0.0f;
		}
	}

	//Эти две переменные являются коэффициентами для последующий реккурентных вызовов функции
	//Фишка заключается в том, что чем больше будет реккурентных вызовов, тем больше будет этих коэффициентов 
	//А значит базисная функция будет менее линейной и будет больше сглаживаться 
	//Количество реккурентных вызовов зависит от прааметра P, который является степенью NURBS
	float f = (t - KnotVector[k]) / (KnotVector[k + p] - KnotVector[k]);
	float g = (KnotVector[k + p + 1] - t) / (KnotVector[k + p + 1] - KnotVector[k + 1]);

	//Реккурентный вызов функции
	return f * basisFunc(k, p - 1, t) + g * basisFunc(k + 1, p - 1, t);
}

//Функция для построения того самого NURBS сплайна
//Возвращает координаты точки в момент времени t 
float* NURBSSpline(float t) {
	//Тут просто переменные для координат и для коэффициента базисов и весов
	//Вся формула вот тут http://www.graph.unn.ru/rus/materials/CG/CG05_Splines.pdf на странице 19 в самом верху.
	float x = 0.0f;
	float y = 0.0f;
	float basisWeigthSum = 0.0f;
	//Пройдемся по всем точкам
	for (int i = 0; i <= N; i++) {
		//Подсчитаем базисную функцию и помножим на вес.
		float coeff = basisFunc(i, P, t) * WeigthVector[i];
		//Потом все проссумируем
		basisWeigthSum += coeff;
		x += ctrlpoints[i][0] * coeff;
		y += ctrlpoints[i][1] * coeff;
	}
	//Запишем ответ в виде массива
	float answer[2] = { x / basisWeigthSum, y / basisWeigthSum };

	//Вернем его
	return answer;
}

//Тут штуки с Безье, они вроде работают, я писал, чтобы на себе прочувствовать что вообще должно происходить

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

//Это уже для OpenGL функции

//Отображение
void Display(void) {

	// Ну тут в принципе все стандартно
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Сначала отображаю контрольные точки
	glColor3f(0.0, 1.0, 0.0); //Цвет контрольных точек
	glPointSize(5.0); //Размер точек
	glBegin(GL_POINTS); //Точки
	for (int i = 0; i <= N; i++)
		glVertex2fv(ctrlpoints[i]);
	glEnd();

	//Потом рисую линии между контрольными точками (контрольные сегменты)
	glLineWidth(1.0f); //Размер линий
	glBegin(GL_LINE_STRIP); //Линии
	for (int i = 0; i <= N; i++)
		glVertex2fv(ctrlpoints[i]);
	glEnd();

	//Наконец рисую сам NURBS сплайн
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 0.0f);
	//Мелкость нашего шага. Суть заключается в том, что нужно пройти отрезок от 0 до 1. Этот параметр определяет как часто мы будем просчитывать сам сплайн
	int quality = 30;
	for (int i = 0; i <= quality; i++)
		//Считаем сплайн и добавляем к точкам 
		glVertex2fv(NURBSSpline((float)i / (float)quality));
	glEnd();

	//Все
	glFlush();
}

//Функция изменения размеров окна
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

//Это функции с которыми тебе возможно стоит поработать
//Я думаю вот этого материла хватит http://grafika.me/node/133

//Эта функция отслеживает нажатие клавиш мыши
//Попробуй пощелкать мышкой и посмотри что выведется в консоль
void MouseButtonHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		std::cout << "LEFT\t" << x << "\t" << y << std::endl;
	}
	
	if (button == GLUT_RIGHT_BUTTON) {
		std::cout << "RIGHT\t" << x << "\t" << y << std::endl;
	}
}

//Эта функция отвечает за движение мыши
//Как ее проверить я не знаю
void MouseMotionHandler(int x, int y) {

}

//King
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Lab03");

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButtonHandler);
	glutMotionFunc(MouseMotionHandler);

	glutMainLoop();
}
