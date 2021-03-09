
#include <stdlib.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>

//Размеры окна
GLint Width = 512;
GLint Height = 512;

//Узлы с равномерным распределением. 
float KnotVector[10] = { 0.0f, 1.0f / 9.0f, 2.0f / 9.0f, 3.0f / 9.0f, 4.0f / 9.0f, 5.0f / 9.0f, 6.0f / 9.0f, 7.0f / 9.0f, 8.0f / 9.0f, 1.0f };
//Веса для точек
float WeigthVector[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

//Стартовые параметры
int P = 3; //Степень NURBS
int N = 5; //Количество контрольных сегментов

//Массив контрольных точек
GLfloat ctrlpoints[6][2] = { {20.0f, 20.0f}, {100.0f, 100.0f},
               {120.0f, 10.0f}, {150.0f, 60.0f},
               {190.0f, 30.0f}, {170.0f, 100.0f} };

//Вспомогательные переменные для измененний координат/веса точки
GLfloat* activePoint = NULL;
int indexActivePoint = 0;
bool moveMode = false;

//Базисная функция
float basisFunc(int k, int p, float t) {
    //условие выхода из функции
    if (p == 0) {
        //Усли попали в промежуток между k и k+1 узлом, то возвращаем 1, если нет, то 0.
        if (KnotVector[k] <= t && t < KnotVector[k + 1]) {
            return 1.0f;
        }
        else {
            return 0.0f;
        }
    }


    float f = (t - KnotVector[k]) / (KnotVector[k + p] - KnotVector[k]);
    float g = (KnotVector[k + p + 1] - t) / (KnotVector[k + p + 1] - KnotVector[k + 1]);

    //Реккурентный вызов функции
    return f * basisFunc(k, p - 1, t) + g * basisFunc(k + 1, p - 1, t);
}

//Функция для построения NURBS сплайна
//Возвращает координаты точки в момент времени t 
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
    return answer;
}


void Display(void) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //Контрольные точки
    glColor3f(0.0, 1.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <= N; i++)
        glVertex2fv(ctrlpoints[i]);
    glEnd();

    //Линии между контрольными точками
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= N; i++)
        glVertex2fv(ctrlpoints[i]);
    glEnd();

    //NURBS сплайн

    glBegin(GL_LINE_STRIP);
    int quality = 500;
    for (int i = 0; i <= quality; i++) {
        //Считаем сплайн и добавляем к точкам 
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2fv(NURBSSpline((float)i / (float)quality));
    }
    glEnd();
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

//Функция поиска точки в области клика мыши
void SearchPiont(int x, int y) {
    GLfloat* nearestPoint = NULL;
    int distanse = 10;
    int indexNP = 0;
    for (int i = 0; i < N + 1; i++) {
        int tmpDistance = abs(ctrlpoints[i][0] - x) + abs(ctrlpoints[i][1] - y);
        //  std::cout << i << "\t" << tmpDistance << "\t" << ctrlpoints[i][0] << "\t" << ctrlpoints[i][1] << std::endl;
        if (tmpDistance < distanse) {
            nearestPoint = ctrlpoints[i];
            indexNP = i;
        }
    }
    if (nearestPoint != NULL) {
        // std::cout << nearestPoint[0] <<"\t"<<nearestPoint[1]<<"\t"<<indexNP<< std::endl;
        activePoint = nearestPoint;
        indexActivePoint = indexNP;
    }
}


//Функция перемещения активной точки
void movePoint(int x, int y) {
    if (activePoint == NULL) {
        return;
    }
    activePoint[0] = x;
    activePoint[1] = y;
    glutPostRedisplay();
}
//Функция изменения активной точки
void changeWeight() {
    if (activePoint == NULL) {
        return;
    }
    float tmpW;
    std::cout << "Enter a new weight between 0.0 and 10.0. Current weight " << WeigthVector[indexActivePoint] << std::endl;
    std::cin >> tmpW;
    if (tmpW > 10.0) {
        tmpW = 10.0;
    }
    else if (tmpW < 0.0) {
        tmpW = 0.0;
    }
    std::cout << "New weight " << tmpW << std::endl;
    WeigthVector[indexActivePoint] = tmpW;
    glutPostRedisplay();
}
//Функция обработки нажатий мыши
void MouseButtonHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        // std::cout << "LEFT\t" << x << "\t" << Height - y << std::endl;
        if (state == GLUT_DOWN) {
            SearchPiont(x, Height - y);
            if (activePoint != NULL) {
                moveMode = true;
            }
        }
        if (state == GLUT_UP) {
            activePoint = NULL;
            moveMode = false;
        }
    }

    if (button == GLUT_RIGHT_BUTTON) {
        //   std::cout << "RIGHT\t" << x << "\t" << Height - y << std::endl;
        if (state == GLUT_DOWN) {
            SearchPiont(x, Height - y);
            changeWeight();
        }
    }
}

//Функция отслеживание активного перемещения мыши
void MouseMotionHandler(int x, int y) {
    if (moveMode == true) {
        if (x < 0) {
            x = 0;
        }
        else if (x > Width) {
            x = Width;
        }
        if (y < 0) {
            y = 0;
        }
        else if (y > Height) {
            y = Height;
        }
        movePoint(x, Height - y);
    }
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == '\033') {
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Right click on point - change weight\nLeft click and drag - move point" << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Lab03");

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(MouseButtonHandler);
    glutMotionFunc(MouseMotionHandler);
    glutMainLoop();
}