#ifndef CUTILS_H_DEFINED
#define CUTILS_H_DEFINED

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Callbacks.h"

using namespace std;

class CUtils
{
public:
	static GLFWwindow* InitAll(int w, int h, bool isFullscreen, const char* title);
	static void ClearAll();
	static bool LoadOBJ(const char* path, vector<glm::vec3>& outVertices, vector<glm::vec2>& outTextures, vector<glm::vec3>& outNormals);
	static void MergeTwoVectors(vector<float>& floatVec, vector<glm::vec3>& glmThree, vector<glm::vec2>& glmTwo);
	static void MergeThreeVectors(vector<float>& outVec, vector<glm::vec3>& coordinates, vector<glm::vec3>& normals, vector<glm::vec2>& textures);
    static void GenSphere(vector<GLuint>& outIndices, vector <glm::vec3>& outVertices, vector <glm::vec2>& outTextures, vector <glm::vec3>& outNormals, float radius, int sectorCount, int stackCount);
	static void GenCylinder(vector<GLuint>& outIndexes, vector<glm::vec3>& outVertices, vector<glm::vec2>& outTextures, vector<glm::vec3>& outNormals,
		float baseRadius, float topRadius, float height, int sectorCount, int stackCount);
private:
	static vector<glm::vec3> GetUnitCircleVertices(int sectorCount);
};

#endif