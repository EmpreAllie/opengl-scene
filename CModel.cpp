#include "CModel.h"

#define STB_IMAGE_IMPLEMENTATION
#include "AdditionalIncludes/stb_image.h"

CModel::CModel(string name) {
	m_Name = name;
	m_IsColors = true;
	m_IsLightEmitter = false;
	m_RenderMode = GL_TRIANGLES;
	m_ModelMatrix = glm::mat4(1.0f);
	m_ViewMatrix = glm::mat4(1.0f);
	m_ProjectionMatrix = glm::mat4(1.0f);
	glGenVertexArrays(1, &m_DefaultVAO);
	glGenVertexArrays(1, &m_LightVAO);
}

CModel::~CModel() {
	// ПИДОРСКАЯ ФУНКЦИЯ, ОТНЯВШАЯ У МЕНЯ ЧАСА ДВА НАХУЙ. 
	// ПОШЛО ОНО ВСЁ НАХУЙ, Я ЕБАЛ

	//glDeleteVertexArrays(1, &m_DefaultVAO); // НЕ РАСКОММЕНТИРОВАТЬ!!!!!!!
}

CModel& CModel::operator=(const CModel& other) {
	if (this != &other) {
		m_Name = other.m_Name;
	}
	return *this;
}

// один массив с координатами вершин и текстур вместе
void CModel::LoadData(float* vertices, size_t count) {
	m_VerticesCount = count;
	GLuint coords_vbo = 0; //Создание VBO
	glGenBuffers(1, &coords_vbo); // Генерация некоего буфера (пока ещё неизвестно какого), 1 - количество буферов
	glBindBuffer(GL_ARRAY_BUFFER, coords_vbo); // Привязка нашего непонятного ещё coords_vbo к конкретному типу GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW); // Копирование данных в текущий буфер типа GL_ARRAY_BUFFER из массива vertices[]

	glBindVertexArray(m_DefaultVAO); //связь VBO с VAO

	// координаты вершин
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// координаты нормалей
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// текстурные координаты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void CModel::LoadLightEmitterData(float* data, size_t count) {
	m_VerticesCount = count;
	m_IsLightEmitter = true;
	GLuint lightEmitterVBO = 0;
	glGenBuffers(1, &lightEmitterVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightEmitterVBO);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
	glBindVertexArray(m_LightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void CModel::LoadColors(glm::vec3* colors, size_t count) {
	m_IsColors = true;
	//Аналогично координатам
	//Размер сохранять не требуется – важно только число вершин
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), colors, GL_STATIC_DRAW);
	if (m_IsLightEmitter == true) {
		glBindVertexArray(m_LightVAO);
	}
	else {
		glBindVertexArray(m_DefaultVAO);
	}
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
}

// загрузка только текстуры через path
void CModel::SetTexture(const char* path, int num) {
	m_IsColors = false;
	glGenTextures(1, &m_Texture);
	glActiveTexture(GL_TEXTURE0 + num); // явно указываем на активную текстуру, можно этого не делать если она одна

	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	//cout << "nrChannels: " << nrChannels << endl;

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture\n";
	}


	stbi_image_free(data);
}

void CModel::LoadIndices(GLuint* indices, size_t count) {
	//Сохранение числа индексов
	m_IndicesCount = count;
	glBindVertexArray(m_DefaultVAO);
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer); // Генерация одного объекта буфера вершин
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	// Привязка элементного буфера
	//Загрузка индексов в используемый элементный буфер
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void CModel::LoadNormals(glm::vec3* normals, size_t count) {
	GLuint normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), normals,
		GL_STATIC_DRAW);
	glBindVertexArray(m_DefaultVAO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}

// эта функция выполняется каждый кадр
void CModel::Render() {
	glActiveTexture(GL_TEXTURE0); // устанавливаем активной текстуру с индексом 0
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	// подключаем шейдерную прграмму по ID
	glUseProgram(m_ShaderProgram);

	if (m_IsLightEmitter == false) {
		GLuint isColorsLocationID = glGetUniformLocation(m_ShaderProgram, "isColors");
		glUniform1i(isColorsLocationID, (int)m_IsColors);

		GLuint lightColLocId = glGetUniformLocation(m_ShaderProgram, "lightColor");
		glUniform3fv(lightColLocId, 1, glm::value_ptr(CScene::GetInstance().GetLightColor()));

		GLuint lightPosLocId = glGetUniformLocation(m_ShaderProgram, "lightPos");
		glUniform3fv(lightPosLocId, 1, glm::value_ptr(CScene::GetInstance().GetLightPos()));

		GLuint viewPosLocId = glGetUniformLocation(m_ShaderProgram, "viewPos");
		glUniform3fv(viewPosLocId, 1, glm::value_ptr(CScene::GetInstance().GetCamera()->Position));

		
		if (m_Name == "TEAPOT") {
			glUniform3f(glGetUniformLocation(m_ShaderProgram, "material.ambient"), 0.2295f, 0.08825f, 0.0275f);
			glUniform3f(glGetUniformLocation(m_ShaderProgram, "material.diffuse"), 0.5508f, 0.2118f, 0.066f);
			glUniform3f(glGetUniformLocation(m_ShaderProgram, "material.specular"), 0.580594f, 0.223257f, 0.0695701f);
			glUniform1f(glGetUniformLocation(m_ShaderProgram, "material.shininess"), 51.2f);
		}
		else {
			glUniform3f(glGetUniformLocation(m_ShaderProgram, "material.ambient"), 0.4f, 0.4f, 0.4f);
			glUniform3f(glGetUniformLocation(m_ShaderProgram, "material.diffuse"), 0.8f, 0.8f, 0.8f);
			glUniform3f(glGetUniformLocation(m_ShaderProgram, "material.specular"), 0.7f, 0.7f, 0.7);
			glUniform1f(glGetUniformLocation(m_ShaderProgram, "material.shininess"), 2.0f);
		}
	}

	// передача в шейдер на каждом кадре обновленной матрицы, чтобы отображались движения
	GLuint modelLocationID = glGetUniformLocation(m_ShaderProgram, "model");
	glUniformMatrix4fv(modelLocationID, 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

	GLuint viewLocationID = glGetUniformLocation(m_ShaderProgram, "view");
	glUniformMatrix4fv(viewLocationID, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));

	GLuint projectionLocationID = glGetUniformLocation(m_ShaderProgram, "projection");
	glUniformMatrix4fv(projectionLocationID, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));

	if (m_IsLightEmitter) {
		glBindVertexArray(m_LightVAO);
	}
	else {
		glBindVertexArray(m_DefaultVAO);
	}
	
	//Если индексы есть - рисуем по ним
	if (m_IndicesCount > 0) {
		glDrawElements(m_RenderMode, (GLsizei)m_IndicesCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(m_RenderMode, 0, (GLsizei)m_VerticesCount);
	}
}