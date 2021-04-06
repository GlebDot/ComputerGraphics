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

void framebuffer_szie_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
void checkErrors(unsigned int shader, std::string type);

void genDensePlane(std::vector<int>& indicies, std::vector<float>& verticies, unsigned int size = 3, float scale = 0.5f);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	int shaderProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl");

	std::vector<float> vertexArr;
	std::vector<int> indexArr;
	genDensePlane(indexArr, vertexArr, 50U);

	//for (int i = 0; i < vertexArr.size() / 3; i++) {
	//	std::cout << vertexArr[i * 3] << "\t" << vertexArr[i * 3 + 1] << "\t" << vertexArr[i * 3 + 2] << std::endl;
	//}

	//for (int i = 0; i < indexArr.size() / 3; i++) {
	//	std::cout << indexArr[i * 3] << "\t" << indexArr[i * 3 + 1] << "\t" << indexArr[i * 3 + 2] << std::endl;
	//}

	//std::cout << sizeof(verticies) << std::endl;
	//std::cout << vertexArr.size() * 4 << std::endl;

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
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



	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f); // сначала инициализируем единичную матрицу
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() / 2 * glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.75f, -3.0f));
		view = glm::rotate(view, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		// ...передаем их в шейдеры (разными способами)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, 0);
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

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
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

void genDensePlane(std::vector<int>& indicies, std::vector<float>& verticies, unsigned int size, float scale) {
	const float space = scale * 2.0f / (size - 1);
	for (float z = scale; z >= -scale; z -= space) {
		for (float x = scale; x >= -scale; x -= space) {
			verticies.push_back(x);
			verticies.push_back(0.0f);
			verticies.push_back(z);
		}
	}

	int trianglesNumber = 2 * (size - 1) * (size - 1);
	int firstIndex = 0;
	for (int i = 0; i < trianglesNumber; i++) {
		indicies.push_back(firstIndex);
		if ((firstIndex + 1) % size == 0) {
			indicies.push_back(firstIndex + size - 1);
			indicies.push_back(firstIndex + size);
			firstIndex++;
		}
		else {
			if ((i + 1) % 2 == 1) {
				indicies.push_back(firstIndex + 1);
				indicies.push_back(firstIndex + size);
				firstIndex++;
			}
			else {
				indicies.push_back(firstIndex + size - 1);
				indicies.push_back(firstIndex + size);
			}
		}
	}
}