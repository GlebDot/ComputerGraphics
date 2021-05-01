#include "ObjParser.h"
#include <fstream>
#include <iostream>
#include <glut.h>

void ObjParser::parseFile(std::string filename)
{
	std::fstream fs;
	fs.open(filename, std::fstream::in);

	std::string str;

	while (!fs.eof())
	{
		fs >> str;
		if (str == "v" || str == "vn") {
			std::string label = str;
			fs >> str;
			float x = atof(str.c_str());
			fs >> str;
			float y = atof(str.c_str());
			fs >> str;
			float z = atof(str.c_str());

			if (label == "v") {
				vertecies.push_back(Vector3(x, y, z));
			}
			else {
				normals.push_back(Vector3(x, y, z));
			}
		}

		if (str == "f") {
			for (int i = 0; i < 3; i++) {
				fs >> str;
				std::string indexStr;
				int ind = str.find("/");
				indexStr.assign(str, 0, ind);
				vertexIndicies.push_back(atoi(indexStr.c_str()));

				ind = str.find("/", ind + 1);
				indexStr.clear();
				indexStr.assign(str, ind + 1, str.length());
				normalIndicies.push_back(atoi(indexStr.c_str()));
			}
		}
	}


	fs.close();
}

void ObjParser::displayObject()
{
	glTranslatef(TranslateX, TranslateY, 0.0f);
	glRotatef(RotatePitch, 0.0f, 1.0f, 0.0f);
	glRotatef(RotateYaw, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glColor3f(0.5, 0.5, 0.5);
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < vertexIndicies.size(); i++) {
		glNormal3f(normals[normalIndicies[i] - 1].x, normals[normalIndicies[i] - 1].y, normals[normalIndicies[i] - 1].z);
		glVertex3f(vertecies[vertexIndicies[i] - 1].x, vertecies[vertexIndicies[i] - 1].y, vertecies[vertexIndicies[i] - 1].z);
	}
	glEnd();
}

void ObjParser::translateObject(float x, float y)
{
	TranslateX += x;
	TranslateY += y;
}

void ObjParser::rotateObject(float pitch, float yaw)
{
	RotatePitch += pitch;
	RotateYaw += yaw;
}
