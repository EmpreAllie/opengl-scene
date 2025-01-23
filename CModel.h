#ifndef CMODEL_H_DEFINED
#define CMODEL_H_DEFINED

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "CShader.h"
#include "CUtils.h"

using namespace std;

class CModel
{
public:
	CModel(string name);
	~CModel();

	CModel& operator=(const CModel& other);

	void LoadData(float* vertices, size_t count);
	void LoadLightEmitterData(float* data, size_t count);

	void LoadIndices(GLuint* indices, size_t count);
	void LoadColors(glm::vec3* colors, size_t count);
	void SetTexture(const char* path, int num);
	//void LoadTwoTextures(glm::vec2* texcoords, size_t count, const char* path1, const char* path2);
	void LoadNormals(glm::vec3* normals, size_t count);
	void Render();

	string GetName() { return m_Name; }
	void SetRenderMode(GLuint mode) { m_RenderMode = mode; }
	void SetShaderProgram(GLuint prog) { m_ShaderProgram = prog; }
	void SetModelMatrix(glm::mat4 matrix) { m_ModelMatrix = matrix; }
	void SetViewMatrix(glm::mat4 matrix) { m_ViewMatrix = matrix; }
	void SetProjectionMatrix(glm::mat4 matrix) { m_ProjectionMatrix = matrix; }

	glm::mat4 GetModelMatrix() { return m_ModelMatrix; }

	//GLuint GetShaderID() { return m_ShaderProgram; }

	void SetDefaultScale(glm::vec3 scale) { m_DefaultScale = scale; }
	void SetDefaultPos(glm::vec3 pos) { m_DefaultPos = pos; }
	glm::vec3 GetDefaultScale() { return m_DefaultScale; }
	glm::vec3 GetDefaultPos() { return m_DefaultPos; }
private:
	string m_Name = "";

	//GLuint m_VAO = -1;
	GLuint m_LightVAO = -1;
	GLuint m_DefaultVAO = -1;
	bool m_IsLightEmitter;

	GLuint m_RenderMode;
	size_t m_VerticesCount = 0;
	size_t m_IndicesCount = 0;
	GLuint m_ShaderProgram = -1;
	GLuint m_Texture = -1;
	glm::mat4 m_ModelMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	bool m_IsColors;

	glm::vec3 m_DefaultScale;
	glm::vec3 m_DefaultPos;
};

#endif