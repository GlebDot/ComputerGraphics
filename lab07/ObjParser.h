#pragma once
#include <string>
#include <fstream>
#include <vector>

struct Vector3
{
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
	float x;
	float y;
	float z;
};

class ObjParser
{
public:
	ObjParser() = default;

	void parseFile(std::string filename);

	void displayObject();

	void translateObject(float x = 0, float y = 0, float z = 0);
	void rotateObject(float pitch, float yaw);

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

