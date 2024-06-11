#include "shaders.hpp"

//"/home/ferrefire/Projects/OpenGL/LearningOpenGL/shaders/simple_fragment.glsl"

long compileShader(int type, const char *path)
{
    std::string shaderString;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        shaderString = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << " -error: " << e.what() << std::endl;
        quit(EXIT_FAILURE);
    }

    const char *shaderCode = shaderString.c_str();

    unsigned int shader;
    shader = glCreateShader(type);

    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        quit(EXIT_FAILURE);
    }

    return (shader);
}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        quit(EXIT_FAILURE);
    }

    return (shaderProgram);
}

unsigned int createShaderProgram(const char *vertexPath, const char *fragmentPath)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    return (createShaderProgram(vertexShader, fragmentShader));
}