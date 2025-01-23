#ifndef CSHADER_H_DEFINED
#define CSHADER_H_DEFINED

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class CShader
{
public:
	CShader(const char* vertexPath, const char* fragmentPath);
	~CShader();
	GLuint GetID();
	void Use();
	void SetBool(const string& name, bool value);
	void SetInt(const string& name, int value);
	void SetFloat(const string& name, float value);
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }

    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) {
        //glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
        //glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));

        GLuint nameLocationID = glGetUniformLocation(m_ID, name.c_str());
        glUniform3fv(nameLocationID, 1, glm::value_ptr(value));
    }

    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }

    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
	string LoadShader(const char* filename);
	void CreateShaderProgram(const char* vert, const char* frag);
	GLuint m_ID;
};

#endif