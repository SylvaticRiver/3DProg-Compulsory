#include "main.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>

using namespace std;

//most the code here is from the lecture powerpoints uploaded on canvas (although I don't exactly know how I'd do it differently anyway when it's a fairly formulaic process
//also some of it is from your github https://github.com/Taheri-meisam/MyOpenGL1/tree/master/MyOpenGL1

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

ofstream out;
string name("C:/Users/chris/source/repos/Render4/files");

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
		cout << "x = " << write[i].x << " y = " << write[i].y << " z = " << write[i].z << " r = " << write[i].r << " g = " << write[i].g << " b = " << write[i].b << endl;
	}

	verts = write.size();

	return write;
}

void function(array<Point3D, 100> verticies) {
	float x = 0;
	float y = pow(2 * x, 2);

	vector<float> Df = { 0, 8 };

	float h = (Df[1] - Df[0]) / verticies.size();

	for (int i = 0; i < verticies.size(); i++) {
		x = i * h;
		float fxi = 2 * pow(x, -x + 3);
		//divides by a constant 5 to reduce the size of the graph so that it fits the screen
		//adds a -1 to the x to shove the graph over to the left edge for better visibility
		verticies[i].x = -1 + (x / 5);
		verticies[i].y = fxi / 5.0f;
		verticies[i].z = 0;
		//this was for a part of the math section of the assignment
		//float a = ((2 * pow(x + h, (-x + h) + 3)) - (2 * pow(x, -x + 3))) / h;
		//cout << verticies[i].x << " = " << a << endl;
		if (i != 0) {
			float diff = verticies[i].y - verticies[i - 1].y;
			if (diff >= 0) {
				verticies[i].r = 0;
				verticies[i].g = 1;
				verticies[i].b = 0;
			}
			else {
				verticies[i].r = 1;
				verticies[i].g = 0;
				verticies[i].b = 0;
			}
		}
	}
	out.open(name);

	out << verticies.size() << endl;

	for (Point3D vertex : verticies) {
		out << vertex.x << " " << vertex.y << " " << vertex.z << " " << vertex.r << " " << vertex.g << " " << vertex.b << endl;
		cout << vertex.x << " " << vertex.y << " " << vertex.z << endl;
	}
	out.close();
	cout << "filen " << name << "er lagret" << endl;
}

array<Point3D, 100> spiralFunction(array<Point3D, 100> verticies) {
	vector<float> func;

	vector<float> Df = { 0, 20 };
	float h = (Df[1] - Df[0]) / verticies.size();
	float converter = 180 / 3.14;

	for (int i = 0; i < verticies.size(); i++) {
		float t = i * h;
		float x = cos(t * 10) / 10;
		float y = sin(t * 10) / 10;
		float z = 2 * t / 30;
		float r = cos(x * 25);
		float g = cos(y * 25);
		float b = cos(z * 25);

		verticies[i] = Point3D{ x, y , z, r, g, b };
	}
	out.open(name);

	out << verticies.size() << endl;

	for (Point3D vertex : verticies) {
		out << vertex.x << " " << vertex.y << " " << vertex.z << " " << vertex.r << " " << vertex.g << " " << vertex.b << endl;
	}
	out.close();
	cout << "filen " << name << "er lagret" << endl;

	return verticies;
}

void draw3Dplane(array<Point3D, 100> arr) {
	float x = 0;
	float y = 0;
	float z = 0;
	int index = 0;
	vector<float> Dfx = { 0, 20 };
	float Dx = (Dfx[1] - Dfx[0]) / arr.size();
	vector<float> Dfy = { 0, 20 };
	float Dy = (Dfy[1] - Dfy[0]) / arr.size();
	float func = cos(x / 5) * sin(y / 5);
	out.open(name);
	int amount = pow((arr.size() / 2), 2);
	out << arr.size() << endl;
	for (int i = 0; i < arr.size() / 2; i++) {
		for (int k = 0; k < arr.size() / 2; k++) {
			x = (Dfx[0] + i * Dx) / 10;
			y = (Dfy[0] + k * Dy) / 10;

			z = (cos(x / 5) * sin(y / 5) / 10);

			arr[i + k] = Point3D{ x, y, z, 0, 0, 1 };

			out << arr[i + k].x << " " << arr[i + k].y << " " << arr[i + k].z << " " << arr[i + k].r << " " << arr[i + k].g << " " << arr[i + k].b << endl;
		}
	}
	out.close();
	cout << "filen " << name << "er lagret" << endl;
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "Compulsory window", NULL, NULL);
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

	array<Point3D, 100> arr{};

	function(arr);

	readArray3DFile(name);

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

	VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render(int amount) {
	processInput(window);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glPointSize(5.0f);

	glDrawArrays(GL_LINE_STRIP, 0, verts);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main() {
	init();

	while (!glfwWindowShouldClose(window)) {
		render(verts);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}