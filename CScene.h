#ifndef CSCENE_H_DEFINED
#define CSCENE_H_DEFINED

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AdditionalIncludes/objloader.hpp"

#include "CModel.h"
#include "CCamera.h"
#include "CDogPart.h"

using namespace std;

class CModel;
class CDogPart;

class CScene
{
public:
	CScene(CScene const&) = delete;
	void operator= (CScene const&) = delete;
	static CScene& GetInstance();

	~CScene();
	void Init(GLFWwindow* window);
	void Loop();
	void Render();

	float GetLastX() { return m_LastX; }
	float GetLastY() { return m_LastY; }
	void SetLastX(float lastx) { m_LastX = lastx; }
	void SetLastY(float lasty) { m_LastY = lasty; }
	CCamera* GetCamera() { return m_Camera; }
	//CShader* GetBasicShader() { return m_Shader; }
	glm::vec3 GetLightColor() { return m_LightColor; }
	glm::vec3 GetLightPos() { return m_LightPos; }

private:
	CScene();
	vector<CModel> m_Models;
	vector<CModel> m_DogParts;
	void LoadModelsData();
	void InitScene();
	void ConfigureMovingObjects();
	void ProcessInput();

	GLFWwindow* m_Window = NULL;
	CShader* m_Shader = NULL;
	CShader* m_LightEmitterShader = NULL;
	CCamera* m_Camera = NULL;

	vector<float> m_FloorVertices;
	vector<GLuint> m_FloorIndices;
	vector<glm::vec3> m_FloorColors;
	vector<glm::vec2> m_FloorTextureCoords;
	vector<glm::vec3> m_FloorPositions;

	vector<float> m_TableLegVertices;

	float m_DeltaTime = 0.0f;
	float m_LastFrame = 0.0f;

	float m_LastX = 0.0f;
	float m_LastY = 0.0f;
	
	float m_LightSphereMovingSpeed = 2.0f;
	glm::vec3 m_LightPos;
	glm::vec3 m_LightColor;

	float maxAng = glm::radians(30.0f);
	float minAng = glm::radians(-30.0f);
	float angle = 0.0f;
	float coeff = 1.0f;
	float rotSpeed = glm::radians(5.0f);

	float m_TailAngle = 0.0f;
	float m_TailRotSpeed = 0.01f;
	float m_TailCoeff = 1.0f;
	float m_MaxTailAngle = 30.0f;
	float m_MinTailAngle = -30.0f;

	glm::vec3 m_DogPos;
	float m_DogMovingSpeed = 0.9f;
	float m_DogAngle = 0.0f;
	float m_DogRotSpeed = 0.75f;
};

#endif