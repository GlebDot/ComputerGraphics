#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <glut.h>

typedef std::vector<GLfloat> matParam;

struct Vector3
{
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
	float x;
	float y;
	float z;
};

struct MaterialInfo 
{
	MaterialInfo() {
		ambient = { 0.2, 0.2, 0.2, 1.0 };
		diffuse = { 0.8, 0.8, 0.8, 1.0 };
		specular = { 0.0, 0.0, 0.0, 1.0 };
		emmision = { 0.0, 0.0, 0.0, 1.0 };
		shininess = { 0.0, 0.0, 0.0, 0.0 };
	};

	matParam ambient;
	matParam diffuse;
	matParam specular;
	matParam emmision;
	matParam shininess;
};

class ObjParser
{
public:
	ObjParser() {
		material = MaterialInfo();
	};

	void parseFile(std::string filename);

	void displayObject();

	MaterialInfo material;

protected:
	std::vector<Vector3> vertecies;
	std::vector<Vector3> normals;

	std::vector<int> vertexIndicies;
	std::vector<int> normalIndicies;

	float TranslateX = 0.0f;
	float TranslateY = 0.0f;
	float TranslateZ = 0.0f;

	float RotateYaw = 0.0f;
	float RotatePitch = 0.0f;
};

