#include "shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    ID = createShaderProgram(vertexPath, fragmentPath);
}

Shader::Shader(const char *computePath)
{
    ID = createShaderProgram(computePath);
}

Shader::Shader(const char *vertexPath, const char *tesselationControlPath, const char *tesselationEvaluationPath, const char *fragmentPath)
{
    ID = createShaderProgram(vertexPath, tesselationControlPath, tesselationEvaluationPath, fragmentPath);
}

void Shader::useShader()
{
	if (currentActiveShader != ID)
	{
		glUseProgram(ID);
		currentActiveShader = ID;
	}
}

void Shader::setBool(std::string name, bool value)
{
    useShader();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(std::string name, int value)
{
    useShader();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(std::string name, float value)
{
    useShader();
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(std::string name, float x, float y)
{
	useShader();
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setFloat3(std::string name, float x, float y, float z)
{
    useShader();
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setFloat3(std::string name, glm::vec3 vec)
{
    useShader();
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setFloat4(std::string name, float x, float y, float z, float w)
{
    useShader();
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setFloat4(std::string name, glm::vec4 vec)
{
    useShader();
    glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMatrix4(std::string name, glm::mat4 transformation)
{
    useShader();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transformation));
}