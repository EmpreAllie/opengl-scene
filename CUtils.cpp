#include "CUtils.h"

const float PI = 3.141528f;

GLFWwindow* CUtils::InitAll(int w, int h, bool isFullscreen, const char* title) {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(-1);
	}

	GLFWwindow* window;

	if (isFullscreen) {
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* vmode = glfwGetVideoMode(mon);
		w = vmode->width;
		h = vmode->height;
		window = glfwCreateWindow(w, h, title, mon, NULL);
	}
	else {
		window = glfwCreateWindow(w, h, title, NULL, NULL);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDebugMessageCallback, NULL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return window;
}

void CUtils::ClearAll() {
	glfwTerminate();
}

bool CUtils::LoadOBJ(const char* path, vector<glm::vec3>& outVertices, vector<glm::vec2>& outTextures, vector<glm::vec3>& outNormals) {
    string prev_buf;

    //��������� ������� ��� �������� �������� � �������� �� �����
    std::vector< unsigned int > vertexIndices, vtIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;

    string buf; //������ ��� �������� ������� �� �����

    std::stringstream sstr; //�����, �������� ���������� �����

    bool cont = false; //���������� ��� ���������� ������ � ����������� ������ ������ � �����

    ifstream file(path, ios::in); //���������� �����
    if (file.is_open())
    {
        sstr << file.rdbuf();
        file.close();
        cout << "[loadOBJ] File opened and closed correctly" << endl;
    }
    else
        return false;
    //������� �������� ��������
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    cout << "Loading OBJ file: " << path << " . . ." << endl;

    while (cont || sstr >> buf)//������� ������� ����� �� �����
    {
        cont = false;
        //cout << buf << endl;
        //��������� ���������
        if (buf == "v")
        {
            glm::vec3 vertex;
            sstr >> vertex.x;
            sstr >> vertex.y;
            sstr >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        //��������� ���������� ���������
        else if (buf == "vt")
        {
            glm::vec2 texture;
            sstr >> texture.x;
            sstr >> texture.y;
            temp_textures.push_back(texture);
        }
        //��������� ��������
        else if (buf == "vn")
        {
            glm::vec3 normal;
            sstr >> normal.x;
            sstr >> normal.y;
            sstr >> normal.z;
            temp_normals.push_back(normal);
        }
        //��������� ������
        else if (buf == "f")
        {
            //��������� ������� ��� �������� ���������� � �����
            vector<string> vertexes;
            vector<int> vertindexes;
            while (!cont)
            {
                //����������� �����
                sstr >> buf;
                if (buf == prev_buf)
                    break;
                prev_buf = buf;
                //cout << buf << endl;
                //���� ����� �������� ������ � ������� � ��� ������������ � ������
                if (buf.find('/') != string::npos)
                    vertexes.push_back(buf);
                //���� ����� � ������ ��������� ������, �� ����������� � ���������� ��������� �����
                else
                {
                    //��� ������ �� ���������� ������ ������������ ������� ������
                    for (string vert : vertexes)
                    {
                        std::replace(vert.begin(), vert.end(), '/', ' ');
                        std::stringstream tmpstream(vert);
                        int v, vt, n;
                        tmpstream >> v;
                        tmpstream >> vt;
                        tmpstream >> n;
                        //������� ��������� � ��������� ������
                        vertindexes.push_back(v);
                        vertindexes.push_back(vt);
                        vertindexes.push_back(n);
                    }
                    //������ ��� ������� ��������� � ������� ��������
                    for (int i = 0; i < 3; i++)
                    {
                        vertexIndices.push_back(vertindexes[i * 3 + 0]);
                        vtIndices.push_back(vertindexes[i * 3 + 1]);
                        normalIndices.push_back(vertindexes[i * 3 + 2]);
                    }
                    //������ ������� � ���� ��� �������� �������, ���� � �� �������
                     //�� ���� ������������� �� ����� ������������� � ����� �������������
                    size_t tmpsize = vertexes.size();
                    if (tmpsize > 3)
                    {
                        //��� ������ �� ������ ����������� ��� �������, ���������� �����������
                        for (int i = 3; i < tmpsize; i++)
                        {
                            vertexIndices.push_back(vertindexes[0]);
                            vtIndices.push_back(vertindexes[1]);
                            normalIndices.push_back(vertindexes[2]);
                            vertexIndices.push_back(vertindexes[(i - 1) * 3 + 0]);
                            vtIndices.push_back(vertindexes[(i - 1) * 3 + 1]);
                            normalIndices.push_back(vertindexes[(i - 1) * 3 + 2]);
                            vertexIndices.push_back(vertindexes[i * 3 + 0]);
                            vtIndices.push_back(vertindexes[i * 3 + 1]);
                            normalIndices.push_back(vertindexes[i * 3 + 2]);
                        }
                    }
                    cont = true; //����� �� ��������� ��������� �����, �������� ���� � ���, ��� �� �������
                }
            }
        }
    }
}

void CUtils::MergeTwoVectors(vector<float>& floatVec, vector<glm::vec3>& glmThree, vector<glm::vec2>& glmTwo) {

    for (int i = 0; i < glmThree.size(); i++) {
        floatVec.push_back(glmThree[i].x);
        floatVec.push_back(glmThree[i].y);
        floatVec.push_back(glmThree[i].z);
        floatVec.push_back(glmTwo[i].x);
        floatVec.push_back(glmTwo[i].y);
    }
}

void CUtils::MergeThreeVectors(vector<float>& outVec, vector<glm::vec3>& coordinates, vector<glm::vec3>& normals, vector<glm::vec2>& textures) {
    size_t size = coordinates.size();
    for (int i = 0; i < size; i++) {
        outVec.push_back(coordinates[i].x);
        outVec.push_back(coordinates[i].y);
        outVec.push_back(coordinates[i].z);
        outVec.push_back(normals[i].x);
        outVec.push_back(normals[i].y);
        outVec.push_back(normals[i].z);
        outVec.push_back(textures[i].x);
        outVec.push_back(textures[i].y);
    }
}

void CUtils::GenSphere(
    std::vector<GLuint>& outIndexes, //�������� ������ ��������
    std::vector <glm::vec3>& outVertices, //�������� ������ ���������
    std::vector <glm::vec2>& outTextures, //�������� ������ ���������� ���������
    std::vector <glm::vec3>& outNormals, //�������� ������ �������� ��������
    float radius, //������ �����
    int sectorCount, //����� ����������
    int stackCount //����� ����������
)
{
    const float PI = 3.14159265359f;
    //������� �������� ��������
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();
    //��������������� ���������� ��� �������� ������������� ������
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;
    float sectorStep = 2.0f * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    //���� �� ������ ���������
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep; // ������� �� PI/2 � �� -PI/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)
        // �� ������ ��������� ����������� (sectorCount+1) ������
        // ��� ������ � ��������� ��������� ������� � �������, �� ���������� ���������� ����������
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // �� 0 �� 2PI
            // ������������� ���������� (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            glm::vec3 vert;
            vert.x = x;
            vert.y = y;
            vert.z = z;
            outVertices.push_back(vert);
            // ������������� ������ ������� (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 norm;
            norm.x = nx;
            norm.y = ny;
            norm.z = nz;
            outNormals.push_back(norm);
            // ������������� ���������� ���������� (s, t) � ��������� [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            glm::vec2 vt;
            vt.s = s;
            vt.t = t;
            outTextures.push_back(vt);
        }

        //�� ��������� ���� - ����� ������� ������, �.�. �������
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1); // ������ �������� ���������
            k2 = k1 + sectorCount + 1; // ������ ���������� ���������
            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // ��� ������ � ��������� ��������� �� 1 ������������, ��� ��������� � �� ���
                    // k1 => k2 => k1+1
                if (i != 0)
                {
                    outIndexes.push_back(k1);
                    outIndexes.push_back(k2);
                    outIndexes.push_back(k1 + 1);
                }
                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    outIndexes.push_back(k1 + 1);
                    outIndexes.push_back(k2);
                    outIndexes.push_back(k2 + 1);
                }
            }
        }
    }
}

vector<glm::vec3> CUtils::GetUnitCircleVertices(int sectorCount) {
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    float x, y, z;

    std::vector<glm::vec3> unitCircleVertices;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        x = cos(sectorAngle);
        y = sin(sectorAngle);
        z = 0.0f;
        unitCircleVertices.push_back({ x, y, z });
    }
    return unitCircleVertices;
}

void CUtils::GenCylinder(vector<GLuint>& outIndexes, vector<glm::vec3>& outVertices, vector<glm::vec2>& outTextures, vector<glm::vec3>& outNormals,
    float baseRadius, float topRadius, float height, int sectorCount, int stackCount) {
    // ������� �������� ��������
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();
    float radius;
    // ����� ��������������� �������
    std::vector<glm::vec3> unitVertices = GetUnitCircleVertices(sectorCount);
    // ��������� ��������� ������ ������� ������
    // ����������� ��� ������ ������
    for (int i = 0; i < stackCount + 1; ++i)
    {
        //������ � ������ ������� ������
        float h = 0 + i * (height / stackCount);
        float t = 1.0f - i / stackCount;
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
        // ��� ������� ������� � ������ ������������� �������
        for (int j = 0, k = 0; j <= sectorCount; ++j, k++)
        {
            float ux = unitVertices[k].x;
            float uy = unitVertices[k].y;
            float uz = unitVertices[k].z;
            // ����������
            glm::vec3 v;
            v.x = ux * radius;
            v.y = uy * radius;
            v.z = h;
            outVertices.push_back(v);
            // ������ �������
            glm::vec3 n;
            n.x = ux;
            n.y = uy;
            n.z = uz;
            outNormals.push_back(n);
            //���������� ����������
            glm::vec2 vt;
            vt.s = (float)j / sectorCount;
            vt.t = t;
            outTextures.push_back(vt);
        }
    }
    // ������ ���� ������������� �������, ����������� ��������� � ��������
    // �� ������� ����������� ������� ������ � ����� ������� ���� ������
    int baseCenterIndex = (int)outVertices.size();
    int topCenterIndex = baseCenterIndex + sectorCount + 1;
    for (int i = 0; i < 2; ++i)
    {
        float h = 0 + i * height;
        float nz = (float)i;
        // ����������� �����
        outVertices.push_back(glm::vec3(0, 0, h));
        outNormals.push_back(glm::vec3(0, 0, nz));
        outTextures.push_back(glm::vec2(0.5, 0.5));
        radius = i ? topRadius : baseRadius;
        //��� ������ ������� � ������� ������������� ���������
        for (int j = 0, k = 0; j < sectorCount; ++j, k++)
        {
            float ux = unitVertices[k].x;
            float uy = unitVertices[k].y;
            // ����������
            outVertices.push_back(glm::vec3(ux * radius, uy * radius, h));
            // ������ �������
            outNormals.push_back(glm::vec3(0, 0, nz));
            // ���������� ����������
            outTextures.push_back(glm::vec2(-ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f));
        }
    }
    //������ ������ ��������
    int k1 = 0;
    int k2 = sectorCount + 1;
    // ������� �����
    for (int i = 0; i < sectorCount * (stackCount + 2); ++i, ++k1, ++k2)
    {
        // �� ������ ������ �� ��� �����
        // k1 => k1+1 => k2
        outIndexes.push_back(k1);
        outIndexes.push_back(k1 + 1);
        outIndexes.push_back(k2);
        // k2 => k1+1 => k2+1
        outIndexes.push_back(k2);
        outIndexes.push_back(k1 + 1);
        outIndexes.push_back(k2 + 1);
    }
    // ������ ������� ��������� � ��������
    for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
        {
            outIndexes.push_back(baseCenterIndex);
            outIndexes.push_back(k + 1);
            outIndexes.push_back(k);
        }
        else
        {
            outIndexes.push_back(baseCenterIndex);
            outIndexes.push_back(baseCenterIndex + 1);
            outIndexes.push_back(k);
        }
    }
    for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
        {
            outIndexes.push_back(topCenterIndex);
            outIndexes.push_back(k);
            outIndexes.push_back(k + 1);
        }
        else // �������� ��������� �����������
        {
            outIndexes.push_back(topCenterIndex);
            outIndexes.push_back(k);
            outIndexes.push_back(topCenterIndex + 1);
        }
    }
}