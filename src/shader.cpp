#include "shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    ID = createShaderProgram(vertexPath, fragmentPath);
}

void Shader::setBool(std::string name, bool value)
{
    glUseProgram(ID);
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(std::string name, int value)
{
    glUseProgram(ID);
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(std::string name, float value)
{
    glUseProgram(ID);
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(std::string name, float x, float y)
{
	glUseProgram(ID);
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setFloat4(std::string name, float x, float y, float z, float w)
{
    glUseProgram(ID);
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}