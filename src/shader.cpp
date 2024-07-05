#include "shader.hpp"
#include "manager.hpp"

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

void Shader::setFloat2(std::string name, glm::vec2 vec)
{
	useShader();
	glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
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

void Shader::setBoolGlobal(std::string name, bool value)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setBool(name, value);
	}
}

void Shader::setIntGlobal(std::string name, int value)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setInt(name, value);
	}
}

void Shader::setFloatGlobal(std::string name, float value)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat(name, value);
	}
}

void Shader::setFloat2Global(std::string name, float x, float y)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat2(name, x, y);
	}
}

void Shader::setFloat2Global(std::string name, glm::vec2 vec)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat2(name, vec);
	}
}

void Shader::setFloat3Global(std::string name, float x, float y, float z)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat3(name, x, y, z);
	}
}

void Shader::setFloat3Global(std::string name, glm::vec3 vec)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat3(name, vec);
	}
}

void Shader::setFloat4Global(std::string name, float x, float y, float z, float w)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat4(name, x, y, z, w);
	}
}

void Shader::setFloat4Global(std::string name, glm::vec4 vec)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setFloat4(name, vec);
	}
}

void Shader::setMatrix4Global(std::string name, glm::mat4 transformation)
{
	for (Shader *shader : Manager::shaders)
	{
		shader->setMatrix4(name, transformation);
	}
}