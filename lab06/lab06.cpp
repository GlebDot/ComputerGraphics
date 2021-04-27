#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

void framebuffer_szie_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
void checkErrors(unsigned int shader, std::string type);

void genSphereSegment(std::vector<float>& verticies, std::vector<int>& indicies, float radius, int fineness);

void updateSphere();

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;
unsigned int VBO, VAO, EBO;
int FINENESS = 4;
int IndexSize;
float SegmentAngle = M_PI_2;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lab 05", NULL, NULL);
	if (window == NULL) {
		std::cout << "Everything is bad" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_szie_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	int shaderProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	
	updateSphere();

	std::vector<float> coordsVert = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f
	};

	unsigned int VAO_coords, VBO_coords;
	glGenVertexArrays(1, &VAO_coords);
	glGenBuffers(1, &VBO_coords);

	glBindVertexArray(VAO_coords);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
	glBufferData(GL_ARRAY_BUFFER, coordsVert.size() * 4, coordsVert.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (float*)0);
	glEnableVertexAttribArray(0);

	// Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный 
	//объект для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот VAO (но подобное довольно редко случается).
	// Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f); // сначала инициализируем единичную матрицу
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() / 10 * glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.75f, -3.0f));
		view = glm::rotate(view, glm::radians(15.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

		// ...передаем их в шейдеры (разными способами)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glPointSize(4.0f);
		glDrawElements(GL_LINE_LOOP, IndexSize, GL_UNSIGNED_INT, 0);
		
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO_coords);
		glDrawArrays(GL_LINE_LOOP, 0, coordsVert.size());
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

void framebuffer_szie_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		FINENESS++;
		if (FINENESS > 100) {
			FINENESS = 100;
		}
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		FINENESS--;
		if (FINENESS < 1) {
			FINENESS = 1;
		}
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		SegmentAngle -= M_PI_4 / 2.0f;
		if (SegmentAngle < M_PI_4) {
			SegmentAngle = M_PI_4;
		}
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		SegmentAngle += M_PI_4 / 2.0f;
		if (SegmentAngle > M_PI) {
			SegmentAngle = M_PI;
		}
	}

	updateSphere();

}

int createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Убеждаемся, что объекты ifstream могут выбросить исключение:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Открываем файлы
		vShaderFile.open(vertexShaderFileName);
		fShaderFile.open(fragmentShaderFileName);
		std::stringstream vShaderStream, fShaderStream;

		// Читаем содержимое файловых буферов
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Закрываем файлы
		vShaderFile.close();
		fShaderFile.close();

		// Конвертируем в строковую переменную данные из потока
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Компилируем шейдеры
	unsigned int vertex, fragment;

	// Вершинный шейдер
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkErrors(vertex, "VERTEX");

	// Фрагментный шейдер
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkErrors(fragment, "FRAGMENT");

	// Шейдерная программа
	int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	checkErrors(program, "PROGRAM");

	// После того, как мы связали шейдеры с нашей программой, удаляем их, т.к. они нам больше не нужны
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

void checkErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void updateSphere() {
	std::vector<float> vertexArr;
	std::vector<int> indexArr;
	//genDensePlane(indexArr, vertexArr, 50U);
	genSphereSegment(vertexArr, indexArr, 0.5, FINENESS);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * 4, vertexArr.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * 4, indexArr.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (float*)0);
	glEnableVertexAttribArray(0);

	// Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный 
	//объект для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот VAO (но подобное довольно редко случается).
	// Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
	glBindVertexArray(0);

	IndexSize = indexArr.size();
}

void genSphereSegment(std::vector<float>& verticies, std::vector<int>& indicies, float radius, int fineness) {
	verticies.push_back(0);
	verticies.push_back(radius);
	verticies.push_back(0);

	int curIndex = 0;
	float presition = 0.01f;
	indicies.push_back(curIndex);
	for (float teta = M_PI / fineness; teta < M_PI - presition; teta += M_PI / fineness) {
		for (float phi = SegmentAngle / fineness; phi < SegmentAngle - presition; phi += SegmentAngle / fineness) {
			verticies.push_back(radius * sin(teta) * cos(phi));
			verticies.push_back(radius * cos(teta));
			verticies.push_back(radius * sin(teta) * sin(phi));
			
			curIndex++;
			indicies.push_back(curIndex);
		}
		verticies.push_back(0);
		verticies.push_back(radius * cos(teta));
		verticies.push_back(0);

		curIndex++;
		indicies.push_back(curIndex);
		indicies.push_back(curIndex - fineness + 1);
	}
	verticies.push_back(0);
	verticies.push_back(-radius);
	verticies.push_back(0);

	curIndex++;
	indicies.push_back(curIndex);
	int maxIndex = curIndex;
	int maxLayer = fineness - 1;
	int dir = 1;
	for (int curLayer = 2; curLayer <= maxLayer; curLayer++) {
		if (curIndex == maxIndex) {
			curIndex = maxIndex - (maxLayer - curLayer) - 2;
			dir = -1;
		}

		if (curIndex == 0) {
			curIndex = curLayer;
			dir = 1;
		}

		indicies.push_back(curIndex);

		while (true) {
			curIndex = curIndex + dir * fineness;
			if (curIndex > maxIndex) {
				curIndex = maxIndex;
				indicies.push_back(curIndex);
				break;
			}

			if (curIndex < 0) {
				curIndex = 0;
				indicies.push_back(curIndex);
				break;
			}

			indicies.push_back(curIndex);
		}
	}

	if (maxLayer % 2 == 0) {
		indicies.push_back(maxIndex);
	}
}