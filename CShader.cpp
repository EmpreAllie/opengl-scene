#include "CShader.h"

CShader::CShader(const char* vertexPath, const char* fragmentPath) {
	CreateShaderProgram(vertexPath, fragmentPath);
}

CShader::~CShader() {
	glDeleteProgram(m_ID);
}

void CShader::CreateShaderProgram(const char* vert, const char* frag) {
	GLuint vertexShaderm_ID, fragmentShaderm_ID;
	vertexShaderm_ID = glCreateShader(GL_VERTEX_SHADER);
	string vstext = LoadShader(vert);
	const char* vs = vstext.c_str();
	glShaderSource(vertexShaderm_ID, 1, &vs, NULL);
	glCompileShader(vertexShaderm_ID);

	fragmentShaderm_ID = glCreateShader(GL_FRAGMENT_SHADER);
	string fstext = LoadShader(frag);
	const char* fs = fstext.c_str();
	glShaderSource(fragmentShaderm_ID, 1, &fs, NULL);
	glCompileShader(fragmentShaderm_ID);

	int success;
	char infoLog[512] = { 0 };
	glGetShaderiv(vertexShaderm_ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderm_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	glGetShaderiv(fragmentShaderm_ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderm_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderm_ID);
	glAttachShader(m_ID, fragmentShaderm_ID);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShaderm_ID);
	glDeleteShader(fragmentShaderm_ID);
}

string CShader::LoadShader(const char* filename) {
	string res;
	ifstream file(filename, ios::in);
	if (file.is_open())
	{
		std::stringstream sstr;
		sstr << file.rdbuf();
		res = sstr.str();
		file.close();
	}
	return res;
}

GLuint CShader::GetID() {
	return m_ID;
}

void CShader::Use() {
	glUseProgram(m_ID);
}

void CShader::SetBool(const string& name, bool value) {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void CShader::SetInt(const string& name, int value) {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CShader::SetFloat(const string& name, float value) {
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}