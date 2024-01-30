#include "test.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <array>

using namespace std;

//slightly modified code from your github https://github.com/Taheri-meisam/MyOpenGL1/tree/master/MyOpenGL1
//this was mostly just used to try and render something in 3d using triangles, but I couldn't get it to work

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

struct Point2D {
	float x;
	float y;
};

struct Point3D {
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
};

GLFWwindow* window;
unsigned int VAO;
unsigned int VBO;
unsigned int shaderProgram;

string name("C:/Users/chris/source/repos/WriteToFile/WriteToFile/files/verticies.txt");

int verts = 100;

float verticies[600];

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
		cout << "I have been closed" << endl;
	}
}
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	cout << "width = " << width << " height = " << height << endl;
}

array<Point3D, 100> readArray3DFile(string name) {
	ifstream in;
	array<Point3D, 100> write;
	float amount;

	in.open(name);

	in >> amount;

	for (int i = 0; i < amount; i++) {
		Point3D point;
		in >> point.x >> point.y >> point.z >> point.r >> point.g >> point.b;
		write[i] = point;
		//cout << write[i].x << " " << write[i].y << " " << write[i].z << endl;
	}

	in.close();

	for (int i = 0; i < write.size(); i++) {
		Point3D p = write[i];
		verticies[i * 6] = write[i].x;
		verticies[(i * 6) + 1] = write[i].y;
		verticies[(i * 6) + 2] = write[i].z;
		verticies[(i * 6) + 3] = write[i].r;
		verticies[(i * 6) + 4] = write[i].g;
		verticies[(i * 6) + 5] = write[i].b;
		cout << "x = " << verticies[(i * 6)] << " y = " << verticies[(i * 6) + 1] << " z = " << verticies[(i * 6) + 2] << " r = " << verticies[(i * 6) + 3] << " g = " << verticies[(i * 6) + 4] << " b = " << verticies[(i * 6) + 5] << endl;
	}

	verts = write.size();

	return write;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "Hello again", NULL, NULL);
	if (window == NULL) {
		cout << "Window was never created" << endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to init GLAD" << endl;
		glfwTerminate();
	}
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int vertexSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);

	if (!vertexSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "vertex shader compile error : " << infoLog << endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int fragmentShaderSuccess;
	char infoLogFrag[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);

	if (!fragmentShaderSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFrag);
		cout << "Fragment shader compile error : " << infoLogFrag << endl;
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int linkSuccess;
	char infoLogProgram[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogProgram);
		cout << "Linking error : " << infoLogProgram << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	readArray3DFile(name);

	unsigned int indices[] = { 
		0, 1, 3,
		1, 2, 3 
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 2500 / 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}
