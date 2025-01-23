#include "CScene.h"

CScene::CScene() {
}

CScene& CScene::GetInstance() {
	static CScene instance;
	return instance;
}

CScene::~CScene() {
	delete m_Shader;
	delete m_LightEmitterShader;
	delete m_Camera;
	/*
	for (CModel* dp : m_DogParts) {
		delete dp;
	}*/
	glfwDestroyWindow(m_Window);
}

// ������ ������������
void CScene::Init(GLFWwindow* window) {
	m_Window = window;
	m_Shader = new CShader("Shaders/default.vs", "Shaders/default.fs");
	m_LightEmitterShader = new CShader("Shaders/lightEmitter.vs", "Shaders/lightEmitter.fs");
	m_Camera = new CCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_LightPos = glm::vec3(0.0f, 1.0f, 0.0f);
	m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_DogPos = glm::vec3(0.0f, 0.0f, 0.0f);

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	m_LastX = (float)w / 2;
	m_LastY = (float)h / 2;

	glEnable(GL_DEPTH_TEST);

	LoadModelsData();
	InitScene(); // �������� �������� � �� ����������� �������
}

// ������� ������ � ��������, ������ ��������� �����
void CScene::Loop() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	while (!glfwWindowShouldClose(m_Window)) {
		Render();
	}
}

// ���������� ��������� ����� while
void CScene::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float currentFrame = glfwGetTime();
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	ProcessInput();
	ConfigureMovingObjects();

	for (CModel m : m_Models) {
		//cout << "Rendering CModel[i] . . ." << m.GetName() << endl;
		m.Render();
	}
	//cout << endl;

	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

// ��� ����������� raw data, �� ���� ���������� � ��
void CScene::LoadModelsData() {
/*
	m_FloorVertices = {    
		{-0.5f, -0.5f, -0.5f}, // 0 ����� ����� �����
		{-0.5f, 0.5f, -0.5f}, // 1 ����� ������ �����
		{0.5f, 0.5f, -0.5f}, // 2 ������ ������ �����
		{0.5f, -0.5f, -0.5f}, // 3 ������ ����� �����
		// �������� �����
		{-0.5f, -0.5f, 0.5f}, // 4 ����� ����� �������
		{-0.5f, 0.5f, 0.5f}, // 5 ����� ������ �������
		{0.5f, 0.5f, 0.5f}, // 6 ������ ������ �������
		{0.5f, -0.5f, 0.5f} // 7 ������ ����� �������
	};
	*/
	m_FloorVertices = {
		// ��� ����
		// positions          // normals		 // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,  2.0f, 2.0f

		/*
		// ��� ����������� ����
		//positions           //textures
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-right
		// right face		 
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left     
		 // bottom face
		 -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // top-right
		  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, // top-left
		  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // bottom-left
		  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // bottom-left
		 -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // bottom-right
		 -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // top-right
		 // top face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // top-left
		  1.0f,  1.0f , 1.0f, 1.0f, 0.0f, // bottom-right
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // top-right     
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // top-left
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f  // bottom-left 
		 */
	};

	m_FloorIndices = {
		// ��������
		4, 5, 6,
		4, 7, 6,

		// ������
		3, 2, 6,
		6, 7, 3,
		
		// �����
		0, 1, 5,
		5, 4, 0,
		
		// ������
		0, 3, 7,
		7, 4, 0,

		// �������
		1, 2, 6, 
		6, 5, 1,
		
		// ������ �����
		0, 1, 2,
		2, 3, 0,	
	};

	m_FloorColors = {
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f},   
		{ 0.0f, 0.0f, 1.0f},
		{ 1.0f, 1.0f, 0.0f }
	};
	/*
	m_FloorTextureCoords = {
		
		// ��� �� ���� ���������, �� ����� �� ��������
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		
		
		// ��� ����� �����������, �� 4 �� 6 ������� ������������ ���������
		// ������
		{1.0f, 0.0f}, // 3
		{1.0f, 1.0f}, // 2
		{0.0f, 1.0f}, // 6
		{0.0f, 0.0f}, // 7

		// ��������
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},

		// ??? ������� � ������ ???
		{1.0f, 1.0f}, // 0
		{1.0f, 0.0f}, // 3
		{0.0f, 0.0f}, // 7
		{0.0f, 1.0f}, // 4
		
	};
	*/
	/*
	m_FloorPositions = {
		{0.0f, 0.0f, 0.0f},
		{2.0f, 5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, -0.4f, -3.5f},
		{-1.7f, 3.0f, -7.5f},
		{1.3f, -2.0f, -2.5f},
		{1.5f, 2.0f, -2.5f},
		{1.5f, 0.2f, -1.5f},
		{-1.3f, 1.0f, -1.5f},
		
	};*/

	// ���
	CModel floor("FLOOR");
	floor.SetTexture("Images/chess.jpg", 0); // ������ �������� - ����� �������� ��������. �� ���� ���� ��� ������������ ��� �������� ���������� �������
	floor.LoadData(m_FloorVertices.data(), m_FloorVertices.size());
	//floor.LoadColors(floorColors.data(), floorColors.size());
	floor.SetShaderProgram(m_Shader->GetID());
	floor.SetRenderMode(GL_TRIANGLES);
	m_Models.push_back(floor);

	// ������ �����
	vector<float> tableTopCoords = {
		// ��� ������ �����
		 //positions			// normals          //textures
		 // back face
		 -1.0f,  0.8f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		  1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
		  1.0f,  0.8f, -1.0f,	0.0f, 0.0f, -1.0f,  1.0f, 0.0f, // bottom-right         
		  1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
		 -1.0f,  0.8f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		 -1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, 1.0f, // top-left
		 // front face
		 -1.0f,  0.8f,  1.0f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom-left
		  1.0f,  0.8f,  1.0f,	0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom-right
		  1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right
		  1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right
		 -1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // top-left
		 -1.0f,  0.8f,  1.0f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom-left
		 // left face
		 -1.0f,  1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-left
		 -1.0f,  1.0f,  1.0f,	-1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-right
		 -1.0f,  0.8f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
		 -1.0f,  0.8f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
		 -1.0f,  0.8f,  1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		 -1.0f,  1.0f,  1.0f,	-1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-right
		 // right face		 
		  1.0f,  1.0f,  1.0f,	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top-left
		  1.0f,  0.8f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // bottom-right
		  1.0f,  1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top-right         
		  1.0f,  0.8f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // bottom-right
		  1.0f,  1.0f,  1.0f,	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top-left
		  1.0f,  0.8f,  1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom-left     
		 // bottom face
		 -1.0f,	 0.8f, -1.0f,	0.0f, -1.0f, 0.0f,  0.0f, 1.0f, // top-right
		  1.0f,	 0.8f, -1.0f,	0.0f, -1.0f, 0.0f,  1.0f, 1.0f, // top-left
		  1.0f,	 0.8f,  1.0f,	0.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		  1.0f,	 0.8f,  1.0f,	0.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		 -1.0f,	 0.8f,  1.0f,	0.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		 -1.0f,	 0.8f, -1.0f,	0.0f, -1.0f, 0.0f,  0.0f, 1.0f, // top-right
		 // top face
		 -1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top-left
		  1.0f,  1.0f , 1.0f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom-right
		  1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top-right     
		  1.0f,  1.0f,  1.0f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom-right
		 -1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top-left
		 -1.0f,  1.0f,  1.0f,	0.0f, 1.0f, 0.0f,   0.0f, 0.0f  // bottom-left 
	};
	CModel tableTop("TABLE_TOP");
	tableTop.SetTexture("Images/wood.jpg", 1);
	tableTop.LoadData(tableTopCoords.data(), tableTopCoords.size());
	tableTop.SetShaderProgram(m_Shader->GetID());
	tableTop.SetRenderMode(GL_TRIANGLES);
	m_Models.push_back(tableTop);

	// ����� �����
	m_TableLegVertices = {
		// ��� ����� �����
		//positions		        //normals			//textures
		
		// back face
		-1.0f,  0.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		-0.8f,  0.8f, -1.0f,	0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
		-0.8f,  0.0f, -1.0f,	0.0f, 0.0f, -1.0f,  1.0f, 0.0f, // bottom-right         
		-0.8f,  0.8f, -1.0f,	0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
		-1.0f,  0.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		-1.0f,  0.8f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, 1.0f, // top-left

		// front face
		-1.0f,  0.0f, -0.8f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom-left
		-0.8f,  0.0f, -0.8f,	0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom-right
		-0.8f,  0.8f, -0.8f,	0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right
		-0.8f,  0.8f, -0.8f,	0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right
		-1.0f,  0.8f, -0.8f,	0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // top-left
		-1.0f,  0.0f, -0.8f,	0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom-left

		// left face
		-1.0f,  0.8f, -1.0f,	-1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-left
		-1.0f,  0.8f, -0.8f,	-1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-right
		-1.0f,  0.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
		-1.0f,  0.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
		-1.0f,  0.0f, -0.8f,	-1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		-1.0f,  0.8f, -0.8f,	-1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-right

		 // right face		 
		-0.8f,  0.8f, -0.8f,	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top-left
		-0.8f,  0.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // bottom-right
		-0.8f,  0.8f, -1.0f,	1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top-right         
		-0.8f,  0.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // bottom-right
		-0.8f,  0.8f, -0.8f,	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top-left
		-0.8f,  0.0f, -0.8f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom-left     

		// bottom face
		-1.0f,  0.0f, -1.0f,	0.0f, -1.0f, 0.0f,  0.0f, 1.0f, // top-right
		-0.8f,  0.0f, -1.0f,	0.0f, -1.0f, 0.0f,  1.0f, 1.0f, // top-left
		-0.8f,  0.0f, -0.8f,	0.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		-0.8f,  0.0f, -0.8f,	0.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		-1.0f,  0.0f, -0.8f,	0.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		-1.0f,  0.0f, -1.0f,	0.0f, -1.0f, 0.0f,  0.0f, 1.0f, // top-right

		// top face
		-1.0f,  0.8f, -1.0f,	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top-left
		-0.8f,  0.8f, -0.8f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom-right
		-0.8f,  0.8f, -1.0f,	0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top-right     
		-0.8f,  0.8f, -0.8f,	0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom-right
		-1.0f,  0.8f, -1.0f,	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top-left
		-1.0f,  0.8f, -0.8f,	0.0f, 1.0f, 0.0f,   0.0f, 0.0f  // bottom-left 
	};
	CModel tableLeg1("TABLE_LEG_1");
	tableLeg1.SetTexture("Images/wood.jpg", 1);
	tableLeg1.LoadData(m_TableLegVertices.data(), m_TableLegVertices.size());
	tableLeg1.SetShaderProgram(m_Shader->GetID());
	tableLeg1.SetRenderMode(GL_TRIANGLES);
	m_Models.push_back(tableLeg1);

	CModel tableLeg2("TABLE_LEG_2");
	tableLeg2.SetTexture("Images/wood.jpg", 1);
	tableLeg2.LoadData(m_TableLegVertices.data(), m_TableLegVertices.size());
	tableLeg2.SetShaderProgram(m_Shader->GetID());
	tableLeg2.SetRenderMode(GL_TRIANGLES);
	m_Models.push_back(tableLeg2);

	CModel tableLeg3("TABLE_LEG_3");
	tableLeg3.SetTexture("Images/wood.jpg", 1);
	tableLeg3.LoadData(m_TableLegVertices.data(), m_TableLegVertices.size());
	tableLeg3.SetShaderProgram(m_Shader->GetID());
	tableLeg3.SetRenderMode(GL_TRIANGLES);
	m_Models.push_back(tableLeg3);

	CModel tableLeg4("TABLE_LEG_4");
	tableLeg4.SetTexture("Images/wood.jpg", 1);
	tableLeg4.LoadData(m_TableLegVertices.data(), m_TableLegVertices.size());
	tableLeg4.SetShaderProgram(m_Shader->GetID());
	tableLeg4.SetRenderMode(GL_TRIANGLES);
	m_Models.push_back(tableLeg4);

	// ������
	vector<glm::vec3> teapotVertices;
	vector<glm::vec2> teapotTextures;
	vector<glm::vec3> teapotNormals;
	vector<glm::vec3> teapotColors;
	bool loaded = loadOBJ("OBJModels/BrownBettyTeapot.obj", teapotVertices, teapotTextures, teapotNormals);
	if (loaded == false) cout << "Failed to load OBJ file." << endl;
	vector<float> teapotData;
	CUtils::MergeThreeVectors(teapotData, teapotVertices, teapotNormals, teapotTextures);
	CModel teapot("TEAPOT");
	teapot.LoadData(teapotData.data(), teapotData.size());
	teapot.SetTexture("Images/graysquare.png", 2);
	teapot.SetShaderProgram(m_Shader->GetID());
	teapot.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(teapot);

	// ����� - �������� �����
	vector<glm::vec3> lightSphereVertices;
	vector<glm::vec2> lightSphereTextures;
	vector<glm::vec3> lightSphereNormals;
	vector<GLuint> lightSphereIndices;
	CUtils::GenSphere(lightSphereIndices, lightSphereVertices, lightSphereTextures, lightSphereNormals, 0.025f, 100, 100);
	vector<glm::vec3> lightSphereColors;
	for (int i = 0; i < lightSphereVertices.size(); i++) 
		lightSphereColors.push_back(m_LightColor);
	vector<float> lightSphereData;
	CUtils::MergeTwoVectors(lightSphereData, lightSphereVertices, lightSphereTextures);
	CModel lightSphere("LIGHT_SPHERE");
	lightSphere.LoadLightEmitterData(lightSphereData.data(), lightSphereData.size());
	lightSphere.LoadColors(lightSphereColors.data(), lightSphereColors.size());
	lightSphere.SetShaderProgram(m_LightEmitterShader->GetID());
	lightSphere.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(lightSphere);

	// ����� � ��������� (������)
	CModel sphere("SNOWMAN_SPHERE_DOWN");
	vector<float> sphereData;
	CUtils::MergeThreeVectors(sphereData, lightSphereVertices, lightSphereNormals, lightSphereTextures);
	vector<glm::vec3> sphereColors;
	size_t size = lightSphereVertices.size();
	for (int i = 0; i < size; i++) {
		sphereColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	sphere.LoadData(sphereData.data(), sphereData.size());
	sphere.LoadColors(sphereColors.data(), sphereColors.size());
	sphere.SetShaderProgram(m_Shader->GetID());
	sphere.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(sphere);

	// ����� � ��������� (�������)
	CModel sphere2("SNOWMAN_SPHERE_UP");
	sphere2.LoadData(sphereData.data(), sphereData.size());
	sphere2.LoadColors(sphereColors.data(), sphereColors.size());
	sphere2.SetShaderProgram(m_Shader->GetID());
	sphere2.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(sphere2);

	// �����-��������
	vector<glm::vec3> coneVertices;
	vector<glm::vec3> coneNormals;
	vector<glm::vec2> coneTextures;
	vector<glm::vec3> coneColors;
	vector<GLuint> coneIndices;
	CUtils::GenCylinder(coneIndices, coneVertices, coneTextures, coneNormals, 0.02f, 0.0001f, 0.125f, 100, 100);
	vector<float> coneData;
	CUtils::MergeThreeVectors(coneData, coneVertices, coneNormals, coneTextures);
	size = coneVertices.size();
	for (int i = 0; i < size; i++) {
		coneColors.push_back(glm::vec3(1.0f, 0.5f, 0.0f));
	}
	CModel cone("SNOWMAN_CONE");
	cone.LoadData(coneData.data(), coneData.size());
	cone.LoadColors(coneColors.data(), coneColors.size());
	cone.SetShaderProgram(m_Shader->GetID());
	cone.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(cone);

	// �����
	vector<glm::vec3> stickVerts;
	vector<glm::vec3> stickNormals;
	vector<glm::vec2> stickTextures;
	loaded = loadOBJ("OBJModels/stick.obj", stickVerts, stickTextures, stickNormals);
	if (!loaded) cout << "Unable to load STICK.obj" << endl;
	//cout << stickVerts.size() << " " << stickTextures.size() << " " << stickNormals.size() << " " << endl;
	vector<float> stickData;
	CUtils::MergeThreeVectors(stickData, stickVerts, stickNormals, stickTextures);
	CModel stick("STICK");
	stick.LoadData(stickData.data(), stickData.size());
	stick.SetTexture("Images/stick.png", 3);
	stick.SetRenderMode(GL_TRIANGLES);
	stick.SetShaderProgram(m_Shader->GetID());
	m_Models.push_back(stick);


	// ������ - ���� ������
	sphereColors.clear();
	for (int i = 0; i < size; i++) {
		sphereColors.push_back(glm::vec3(0.81f, 0.69f, 0.5f));
	}
	CModel dog_body("DOG_BODY");
	dog_body.LoadData(sphereData.data(), sphereData.size());
	dog_body.LoadColors(sphereColors.data(), sphereColors.size());
	dog_body.SetShaderProgram(m_Shader->GetID());
	dog_body.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dog_body);
	dog_body.SetDefaultPos(glm::vec3(1.05f, -0.07f, 1.0f));
	dog_body.SetDefaultScale(glm::vec3(2.5f, 1.0f, 1.0f) * glm::vec3(3.0f));
	m_DogParts.push_back(dog_body);

	// ����� �������� ����
	CModel dlfl("DOG_LEG_FRONT_LEFT");
	dlfl.LoadData(sphereData.data(), sphereData.size());
	dlfl.LoadColors(sphereColors.data(), sphereColors.size());
	dlfl.SetShaderProgram(m_Shader->GetID());
	dlfl.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dlfl);
	dlfl.SetDefaultPos(glm::vec3(0.95f, -0.15f, 1.03f));
	dlfl.SetDefaultScale(glm::vec3(1.0f, 3.0f, 1.0f));
	m_DogParts.push_back(dlfl);

	//cout << glm::to_string(m_Models[1].GetDefaultPos()) << endl;

	// ������ �������� ����
	CModel dlfr("DOG_LEG_FRONT_RIGHT");
	dlfr.LoadData(sphereData.data(), sphereData.size());
	dlfr.LoadColors(sphereColors.data(), sphereColors.size());
	dlfr.SetShaderProgram(m_Shader->GetID());
	dlfr.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dlfr);
	dlfr.SetDefaultPos(glm::vec3(0.95f, -0.15f, 0.97f));
	dlfr.SetDefaultScale(glm::vec3(1.0f, 3.0f, 1.0f));
	m_DogParts.push_back(dlfr);

	// ����� ������ ����
	CModel dlbl("DOG_LEG_BACK_LEFT");
	dlbl.LoadData(sphereData.data(), sphereData.size());
	dlbl.LoadColors(sphereColors.data(), sphereColors.size());
	dlbl.SetShaderProgram(m_Shader->GetID());
	dlbl.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dlbl);
	dlbl.SetDefaultPos(glm::vec3(1.15f, -0.15f, 1.03f));
	dlbl.SetDefaultScale(glm::vec3(1.0f, 3.0f, 1.0f));
	m_DogParts.push_back(dlbl);

	// ������ ������ ����
	CModel dlbr("DOG_LEG_BACK_RIGHT");
	dlbr.LoadData(sphereData.data(), sphereData.size());
	dlbr.LoadColors(sphereColors.data(), sphereColors.size());
	dlbr.SetShaderProgram(m_Shader->GetID());
	dlbr.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dlbr);
	dlbr.SetDefaultPos(glm::vec3(1.15f, -0.15f, 0.97f));
	dlbr.SetDefaultScale(glm::vec3(1.0f, 3.0f, 1.0f));
	m_DogParts.push_back(dlbr);

	// ������
	CModel head("DOG_HEAD");
	head.LoadData(sphereData.data(), sphereData.size());
	head.LoadColors(sphereColors.data(), sphereColors.size());
	head.SetShaderProgram(m_Shader->GetID());
	head.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(head);
	head.SetDefaultPos(glm::vec3(0.9f, 0.02f, 1.0f));
	head.SetDefaultScale(glm::vec3(2.25f));
	m_DogParts.push_back(head);

	// ����� ���
	CModel del("DOG_EAR_LEFT");
	del.LoadData(sphereData.data(), sphereData.size());
	del.LoadColors(sphereColors.data(), sphereColors.size());
	del.SetShaderProgram(m_Shader->GetID());
	del.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(del);
	del.SetDefaultPos(glm::vec3(0.92f, 0.05f, 0.97f));
	del.SetDefaultScale(glm::vec3(1.0f, 2.0f, 1.0f));
	m_DogParts.push_back(del);

	// ������ ���
	CModel der("DOG_EAR_RIGHT");
	der.LoadData(sphereData.data(), sphereData.size());
	der.LoadColors(sphereColors.data(), sphereColors.size());
	der.SetShaderProgram(m_Shader->GetID());
	der.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(der);
	der.SetDefaultPos(glm::vec3(0.92f, 0.05f, 1.03f));
	der.SetDefaultScale(glm::vec3(1.0f, 2.0f, 1.0f));
	m_DogParts.push_back(der);

	// ���
	CModel dn("DOG_NOSE");
	dn.LoadData(sphereData.data(), sphereData.size());
	dn.LoadColors(sphereColors.data(), sphereColors.size());
	dn.SetShaderProgram(m_Shader->GetID());
	dn.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dn);
	dn.SetDefaultPos(glm::vec3(0.87f, 0.01f, 1.0f));
	dn.SetDefaultScale(glm::vec3(1.5f, 0.8f, 1.4f));
	m_DogParts.push_back(dn);

	// ����� ������
	CModel dt("DOG_TAIL");
	dt.LoadData(sphereData.data(), sphereData.size());
	dt.LoadColors(sphereColors.data(), sphereColors.size());
	dt.SetShaderProgram(m_Shader->GetID());
	dt.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(dt);
	dt.SetDefaultPos(glm::vec3(1.25f, -0.1f, 1.0f));
	dt.SetDefaultScale(glm::vec3(3.5f, 1.0f, 1.0f));
	m_DogParts.push_back(dt);

	sphereColors.clear();
	for (int i = 0; i < size; i++) {
		sphereColors.push_back(glm::vec3(0.05f));
	}

	// ����� ���� ������
	CModel deyeleft("DOG_EYE_LEFT");
	deyeleft.LoadData(sphereData.data(), sphereData.size());
	deyeleft.LoadColors(sphereColors.data(), sphereColors.size());
	deyeleft.SetShaderProgram(m_Shader->GetID());
	deyeleft.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(deyeleft);
	deyeleft.SetDefaultPos(glm::vec3(0.86f, 0.04f, 0.97f));
	deyeleft.SetDefaultScale(glm::vec3(0.3f));
	m_DogParts.push_back(deyeleft);

	// ����� ���� ������
	CModel deyeright("DOG_EYE_RIGHT");
	deyeright.LoadData(sphereData.data(), sphereData.size());
	deyeright.LoadColors(sphereColors.data(), sphereColors.size());
	deyeright.SetShaderProgram(m_Shader->GetID());
	deyeright.SetRenderMode(GL_TRIANGLE_FAN);
	m_Models.push_back(deyeright);
	deyeright.SetDefaultPos(glm::vec3(0.86f, 0.04f, 1.03f));
	deyeright.SetDefaultScale(glm::vec3(0.3f));
	m_DogParts.push_back(deyeright);
}

// ������������� ������ ���� �������� �� �����
void CScene::InitScene() {
	glm::mat4 model(1.0f);
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i].GetName() == "FLOOR") {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
		}
		if (m_Models[i].GetName() == "TABLE_TOP") {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -3.0f));
		}
		if (m_Models[i].GetName() == "TABLE_LEG_1") {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, glm::vec3(-2.8f, -0.5f, -2.8f));
		}
		if (m_Models[i].GetName() == "TABLE_LEG_2") {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, glm::vec3(-1.4f, -0.5f, -1.4f));
		}
		if (m_Models[i].GetName() == "TABLE_LEG_3") {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, glm::vec3(-1.4f, -0.5f, -2.8f));
		}
		if (m_Models[i].GetName() == "TABLE_LEG_4") {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, glm::vec3(-2.8f, -0.5f, -1.4f));
		}
		if (m_Models[i].GetName() == "TEAPOT") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.4f, 0.25f, -1.4f));
			model = glm::scale(model, glm::vec3(0.025f));
			model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (m_Models[i].GetName() == "LIGHT_SPHERE") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_LightPos);
		}
		if (m_Models[i].GetName() == "SNOWMAN_SPHERE_DOWN") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.05f, 1.0f));
			model = glm::scale(model, glm::vec3(12.0f));
		}
		if (m_Models[i].GetName() == "SNOWMAN_SPHERE_UP") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.45f, 1.0f));
			model = glm::scale(model, glm::vec3(4.0f));
		}
		if (m_Models[i].GetName() == "SNOWMAN_CONE") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.94f, 0.45f, 1.0f));			
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (m_Models[i].GetName() == "STICK") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.1f, 0.9f));
			model = glm::scale(model, glm::vec3(0.015f));
			model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		if (m_Models[i].GetName() == "DOG_BODY") { 
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_DogParts[0].GetDefaultPos());
			model = glm::scale(model, m_DogParts[0].GetDefaultScale());
		} // ������ � m_DogParts: 0
		if (m_Models[i].GetName() == "DOG_LEG_FRONT_LEFT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[1].GetDefaultPos());
			model = glm::scale(model, m_DogParts[1].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_LEG_FRONT_RIGHT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[2].GetDefaultPos());
			model = glm::scale(model, m_DogParts[2].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_LEG_BACK_LEFT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[3].GetDefaultPos());
			model = glm::scale(model, m_DogParts[3].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_LEG_BACK_RIGHT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[4].GetDefaultPos());
			model = glm::scale(model, m_DogParts[4].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_HEAD") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[5].GetDefaultPos());
			model = glm::scale(model, m_DogParts[5].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_EAR_LEFT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[6].GetDefaultPos());
			model = glm::scale(model, m_DogParts[6].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_EAR_RIGHT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[7].GetDefaultPos());
			model = glm::scale(model, m_DogParts[7].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_NOSE") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[8].GetDefaultPos());
			model = glm::scale(model, m_DogParts[8].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_TAIL") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[9].GetDefaultPos());
			model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, m_DogParts[9].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_EYE_LEFT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[10].GetDefaultPos());
			model = glm::scale(model, m_DogParts[10].GetDefaultScale());
		}
		if (m_Models[i].GetName() == "DOG_EYE_RIGHT") {
			model = glm::mat4(1.0f);
			model = translate(model, m_DogParts[11].GetDefaultPos());
			model = glm::scale(model, m_DogParts[11].GetDefaultScale());
		}
		m_Models[i].SetModelMatrix(model);
	}
}

// ��� �������������� ������� model, view � projection: �������� model ��� ���������� ��������, � ����� view � projection ��� ���� �����
void CScene::ConfigureMovingObjects() {
	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);

	//glm::mat4 dogmodel(1.0f);

	view = m_Camera->GetViewMatrix();

	int w, h;
	glfwGetWindowSize(m_Window, &w, &h);
	projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)w / (float)h, 0.01f, 100.0f);

	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i].GetName() == "LIGHT_SPHERE") {
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_LightPos);
			m_Models[i].SetModelMatrix(model);
		}
		if (m_Models[i].GetName() == "STICK") {
			model = glm::mat4(1.0f);
			if (angle >= maxAng || angle <= minAng) coeff *= -1;

			angle += rotSpeed * coeff * m_DeltaTime;
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.9f));
			model = glm::scale(model, glm::vec3(0.015f));
			model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			glm::vec3 origin(-1.0f, 0.1f, 0.9f);
			glm::vec3 axis(1.0f, 0.0f, 0.0f);
			model = glm::translate(model, -origin);
			model = glm::rotate(model, angle, glm::normalize(axis));
			model = glm::translate(model, origin);

			m_Models[i].SetModelMatrix(model);
		}

		m_Models[i].SetViewMatrix(view);
		m_Models[i].SetProjectionMatrix(projection);
	}

	int offset = 12;
	for (int j = 0; j < m_DogParts.size(); j++) {
		//cout << "Configuring moving effects for m_DogParts . . .  m_Name = " << m_DogParts[j].GetName() << endl;

		model = glm::mat4(1.0f);
		glm::vec3 curPos = m_DogPos + m_DogParts[j].GetDefaultPos();
		model = glm::translate(model, curPos);

		// �������� ���� ������ ������ ������������ �����
		model = glm::translate(model, -curPos + glm::vec3(1.0f));
		model = glm::rotate(model, m_DogAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, curPos - glm::vec3(1.0f));

		if (m_DogParts[j].GetName() == "DOG_TAIL") {
			// ������ - ���������� � ��������� �������
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_DogPos + m_DogParts[j].GetDefaultPos());
			model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			// �����, �� ����, �������������� ������� �� �������
			curPos = m_DogPos + m_DogParts[j].GetDefaultPos();
			model = glm::translate(model, -curPos + glm::vec3(1.0f));
			model = glm::rotate(model, m_DogAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, curPos - glm::vec3(1.0f));			

			// �����, �� ����, ������� ������ ������ �����-������
			m_TailAngle = 0.5f * sin(static_cast<float>(glfwGetTime()));
			glm::vec3 tailOrigin(0.1f, 0.0f, 0.0f);
			glm::vec3 tailAxis(0.0f, 1.0f, 0.0f);
			model = glm::translate(model, -tailOrigin);
			model = glm::rotate(model, m_TailAngle, glm::normalize(tailAxis));
			model = glm::translate(model, tailOrigin);


			model = glm::scale(model, glm::vec3(3.5f, 1.0f, 1.0f)); // scale � ����� �����

			m_Models[j + offset].SetModelMatrix(model);
			continue;
		}

		model = glm::scale(model, m_DogParts[j].GetDefaultScale());
		m_Models[j + offset].SetModelMatrix(model);
	}
}

// ���������� ������ ����, ������ KeyCallback'�
void CScene::ProcessInput() {
	float cameraSpeed = static_cast<float>(2.5 * m_DeltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Camera->ProcessKeyboard(ECameraMovement::FORWARD, m_DeltaTime);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Camera->ProcessKeyboard(ECameraMovement::BACKWARD, m_DeltaTime);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Camera->ProcessKeyboard(ECameraMovement::LEFT, m_DeltaTime);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Camera->ProcessKeyboard(ECameraMovement::RIGHT, m_DeltaTime);
	}

	float sphereSpeed = m_DeltaTime * m_LightSphereMovingSpeed;
	if (glfwGetKey(m_Window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		m_LightPos += glm::vec3(sphereSpeed, 0.0f, 0.0f);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		m_LightPos += glm::vec3(-sphereSpeed, 0.0f, 0.0f);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		m_LightPos += glm::vec3(0.0f, 0.0f, -sphereSpeed);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_KP_5) == GLFW_PRESS) {
		m_LightPos += glm::vec3(0.0f, 0.0f, sphereSpeed);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_KP_DIVIDE) == GLFW_PRESS) {
		m_LightPos += glm::vec3(0.0f, sphereSpeed, 0.0f);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		m_LightPos += glm::vec3(0.0f, -sphereSpeed, 0.0f);
	}

	float dogSpeed = m_DeltaTime * m_DogMovingSpeed;
	
	if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_DogPos += glm::vec3(-dogSpeed, 0.0f, 0.0f);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_DogPos += glm::vec3(dogSpeed, 0.0f, 0.0f);
	}

	float rotSpeed = m_DeltaTime * m_DogRotSpeed;
	if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_DogAngle += rotSpeed;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_DogAngle -= rotSpeed;
	}
}
