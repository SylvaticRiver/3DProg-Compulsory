#include "functions.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <array>

using namespace std;
ofstream out;
string name("C:/Users/chris/source/repos/WriteToFile/WriteToFile/files/verticies.txt");

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

//this is pretty much the file I used to get started with my functions. This was originally in its own c++ project so that I could confirm that the file read/write system worked
//I pasted it in here just so that I don't have to post multiple projects, and i pasted the finalised versions of the functions into main

void CreateAndWriteArray3DFile(string name, array<Point3D, 100> val) {
	ofstream out;

	out.open(name);

	for (Point3D point : val) {
		out << point.x << " " << point.y << " " << point.z << endl;
	}

	out.close();

	cout << "filen " << name << "er lagret" << endl;
}

array<Point3D, 100> readArray3DFile(string name) {
	ifstream in;
	array<Point3D, 100> write;
	float amount;

	in.open(name);

	in >> amount;

	for (int i = 0; i < amount; i++) {
		Point3D point;
		in >> point.x >> point.y >> point.z;
		write[i] = point;
	}

	in.close();

	for (Point3D p : write) {
		cout << "x = " << p.x << " y = " << p.y << " z = " << p.z << endl;
	}

	return write;
}

void initWrite() {
	string name("C:/Users/chris/source/repos/WriteToFile/WriteToFile/files/testText.txt");
	float x{ 5.86 };
	float y{ 0.81 };

	out.open(name);

	out << x << " " << y << endl;

	out.close();

	cout << "filen " << name << "er lagret" << endl;

	ifstream in;
	float write;
	float write2;

	in.open(name);

	in >> write >> write2;

	in.close();

	cout << "x = " << write << " y = " << write2 << endl;
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
		//cout << "x = " << verticies[i].x << " " << "y = " << verticies[i].y << " " << "z = " << verticies[i].z << endl;
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