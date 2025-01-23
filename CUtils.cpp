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

    //Временные массивы для хранения индексов и значений из файла
    std::vector< unsigned int > vertexIndices, vtIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;

    string buf; //Строка для хранения токенов из файла

    std::stringstream sstr; //Буфер, хранящий содержимое файла

    bool cont = false; //Переменная для корректной работы с неизвестным числом вершин в грани

    ifstream file(path, ios::in); //Считывание файла
    if (file.is_open())
    {
        sstr << file.rdbuf();
        file.close();
        cout << "[loadOBJ] File opened and closed correctly" << endl;
    }
    else
        return false;
    //Очистка выходных массивов
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    cout << "Loading OBJ file: " << path << " . . ." << endl;

    while (cont || sstr >> buf)//Попытка считать токен из файла
    {
        cont = false;
        //cout << buf << endl;
        //Обработка координат
        if (buf == "v")
        {
            glm::vec3 vertex;
            sstr >> vertex.x;
            sstr >> vertex.y;
            sstr >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        //Обработка текстурных координат
        else if (buf == "vt")
        {
            glm::vec2 texture;
            sstr >> texture.x;
            sstr >> texture.y;
            temp_textures.push_back(texture);
        }
        //Обработка нормалей
        else if (buf == "vn")
        {
            glm::vec3 normal;
            sstr >> normal.x;
            sstr >> normal.y;
            sstr >> normal.z;
            temp_normals.push_back(normal);
        }
        //Обработка граней
        else if (buf == "f")
        {
            //Временные массивы для хранения информации о грани
            vector<string> vertexes;
            vector<int> vertindexes;
            while (!cont)
            {
                //Считывается токен
                sstr >> buf;
                if (buf == prev_buf)
                    break;
                prev_buf = buf;
                //cout << buf << endl;
                //Если токен содержит данные о вершине – она записывается в массив
                if (buf.find('/') != string::npos)
                    vertexes.push_back(buf);
                //Если токен – начало следующей строки, он сохраняется и начинается обработка грани
                else
                {
                    //Для каждой из сохранённых вершин производится парсинг данных
                    for (string vert : vertexes)
                    {
                        std::replace(vert.begin(), vert.end(), '/', ' ');
                        std::stringstream tmpstream(vert);
                        int v, vt, n;
                        tmpstream >> v;
                        tmpstream >> vt;
                        tmpstream >> n;
                        //Индексы заносятся в временный массив
                        vertindexes.push_back(v);
                        vertindexes.push_back(vt);
                        vertindexes.push_back(n);
                    }
                    //Первые три вершины заносятся в массивы индексов
                    for (int i = 0; i < 3; i++)
                    {
                        vertexIndices.push_back(vertindexes[i * 3 + 0]);
                        vtIndices.push_back(vertindexes[i * 3 + 1]);
                        normalIndices.push_back(vertindexes[i * 3 + 2]);
                    }
                    //Дальше сложнее – если ещё остались вершины, надо и их занести
                     //Но надо преобразовать из веера треугольников в набор треугольников
                    size_t tmpsize = vertexes.size();
                    if (tmpsize > 3)
                    {
                        //Для каждой из вершин добавляются три вершины, образующие треугольник
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
                    cont = true; //Чтобы не потерялся считанный токен, ставится флаг о том, что он сохранён
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
    std::vector<GLuint>& outIndexes, //Выходной массив индексов
    std::vector <glm::vec3>& outVertices, //Выходной массив координат
    std::vector <glm::vec2>& outTextures, //Выходной массив текстурных координат
    std::vector <glm::vec3>& outNormals, //Выходной массив векторов нормалей
    float radius, //Радиус сферы
    int sectorCount, //Число меридианов
    int stackCount //Число параллелей
)
{
    const float PI = 3.14159265359f;
    //Очистка выходных массивов
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();
    //Вспомогательные переменные для хранения промежуточных данных
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;
    float sectorStep = 2.0f * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    //Цикл по каждой параллели
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep; // начиная от PI/2 и до -PI/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)
        // На каждую параллель добавляется (sectorCount+1) вершин
        // для первой и последней совпадают позиция и нормаль, но отличаются текстурные координаты
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // от 0 до 2PI
            // высчитываются координаты (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            glm::vec3 vert;
            vert.x = x;
            vert.y = y;
            vert.z = z;
            outVertices.push_back(vert);
            // высчитывается вектор нормали (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 norm;
            norm.x = nx;
            norm.y = ny;
            norm.z = nz;
            outNormals.push_back(norm);
            // высчитываются текстурные координаты (s, t) в диапазоне [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            glm::vec2 vt;
            vt.s = s;
            vt.t = t;
            outTextures.push_back(vt);
        }

        //Но координат мало - нужен порядок обхода, т.е. индексы
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1); // начало текущего меридиана
            k2 = k1 + sectorCount + 1; // начало следующего меридиана
            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // Для первой и последней параллели по 1 треугольнику, для остальных – по два
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
    // Очистка выходных массивов
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();
    float radius;
    // Вызов вспомогательной функции
    std::vector<glm::vec3> unitVertices = GetUnitCircleVertices(sectorCount);
    // Генерация координат вершин боковых граней
    // Выполняется для каждой секции
    for (int i = 0; i < stackCount + 1; ++i)
    {
        //Высота и радиус текущей секции
        float h = 0 + i * (height / stackCount);
        float t = 1.0f - i / stackCount;
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
        // Для каждого сектора в секции высчитываются вершины
        for (int j = 0, k = 0; j <= sectorCount; ++j, k++)
        {
            float ux = unitVertices[k].x;
            float uy = unitVertices[k].y;
            float uz = unitVertices[k].z;
            // Координаты
            glm::vec3 v;
            v.x = ux * radius;
            v.y = uy * radius;
            v.z = h;
            outVertices.push_back(v);
            // Вектор нормали
            glm::vec3 n;
            n.x = ux;
            n.y = uy;
            n.z = uz;
            outNormals.push_back(n);
            //Текстурные координаты
            glm::vec2 vt;
            vt.s = (float)j / sectorCount;
            vt.t = t;
            outTextures.push_back(vt);
        }
    }
    // Теперь надо сгенерировать вершины, описывающие основание и верхушку
    // На будущее сохраняются индексы начала и конца массива этих вершин
    int baseCenterIndex = (int)outVertices.size();
    int topCenterIndex = baseCenterIndex + sectorCount + 1;
    for (int i = 0; i < 2; ++i)
    {
        float h = 0 + i * height;
        float nz = (float)i;
        // Центральная точка
        outVertices.push_back(glm::vec3(0, 0, h));
        outNormals.push_back(glm::vec3(0, 0, nz));
        outTextures.push_back(glm::vec2(0.5, 0.5));
        radius = i ? topRadius : baseRadius;
        //Для каждой вершины в секторе высчитываются параметры
        for (int j = 0, k = 0; j < sectorCount; ++j, k++)
        {
            float ux = unitVertices[k].x;
            float uy = unitVertices[k].y;
            // координаты
            outVertices.push_back(glm::vec3(ux * radius, uy * radius, h));
            // вектор нормали
            outNormals.push_back(glm::vec3(0, 0, nz));
            // текстурные координаты
            outTextures.push_back(glm::vec2(-ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f));
        }
    }
    //Теперь массив индексов
    int k1 = 0;
    int k2 = sectorCount + 1;
    // Сначала грани
    for (int i = 0; i < sectorCount * (stackCount + 2); ++i, ++k1, ++k2)
    {
        // На каждый сектор по две грани
        // k1 => k1+1 => k2
        outIndexes.push_back(k1);
        outIndexes.push_back(k1 + 1);
        outIndexes.push_back(k2);
        // k2 => k1+1 => k2+1
        outIndexes.push_back(k2);
        outIndexes.push_back(k1 + 1);
        outIndexes.push_back(k2 + 1);
    }
    // Теперь индексы основания и верхушки
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
        else // Отдельно последний треугольник
        {
            outIndexes.push_back(topCenterIndex);
            outIndexes.push_back(k);
            outIndexes.push_back(topCenterIndex + 1);
        }
    }
}