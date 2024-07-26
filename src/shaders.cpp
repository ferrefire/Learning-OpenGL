#include "shaders.hpp"
#include "manager.hpp"

//"/home/ferrefire/Projects/OpenGL/LearningOpenGL/shaders/simple_fragment.glsl"

void replaceShaderIncludes(std::string &shaderString, std::string &currentPath)
{
    size_t includePosition = shaderString.find("#include \"");
    while (includePosition != std::string::npos)
    {
        includePosition += 10;
        std::string includeName = shaderString.substr(includePosition, shaderString.find(".glsl", includePosition) - includePosition);
        std::string includeFile = shaderString.substr(includePosition, shaderString.find("\"", includePosition) - includePosition);
        std::string replaceString = "#include \"" + includeFile + "\"";
        std::string includeString = "";
        Utilities::Upper(includeName);
        includeName.append("_INCLUDED");
        if (!Utilities::Contains(shaderString, includeName))
            includeString = Utilities::FileToString((currentPath + "/shaders/includes/" + includeFile).c_str());
        Utilities::Replace(shaderString, replaceString, includeString);

        includePosition = shaderString.find("#include \"");
    }
    //std::cout << includeFile << std::endl;
}

long compileShader(int type, const char *path)
{
    std::string currentPath = Utilities::GetPath();
    std::string shaderString = Utilities::FileToString((currentPath + "/shaders/" + path).c_str());
    //std::string includeString = Utilities::FileToString((currentPath + "/shaders/includes/culling.glsl").c_str());

    replaceShaderIncludes(shaderString, currentPath);

    //std::string replaceString = "#include \"culling.glsl\"";

    //Utilities::Replace(shaderString, replaceString, includeString);
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
        std::cout << path << "::ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        Manager::Quit(EXIT_FAILURE);
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
		Manager::Quit(EXIT_FAILURE);
	}

    return (shaderProgram);
}

unsigned int createShaderProgram(unsigned int computeShader)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, computeShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(computeShader);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		Manager::Quit(EXIT_FAILURE);
	}

    return (shaderProgram);
}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int tesselationControlShader, unsigned int tesselationEvaluationShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, tesselationControlShader);
    glAttachShader(shaderProgram, tesselationEvaluationShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(tesselationControlShader);
    glDeleteShader(tesselationEvaluationShader);
    glDeleteShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		Manager::Quit(EXIT_FAILURE);
	}

    return (shaderProgram);
}

unsigned int createShaderProgram(const char *vertexPath, const char *fragmentPath)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    return (createShaderProgram(vertexShader, fragmentShader));
}

unsigned int createShaderProgram(const char *computePath)
{
    unsigned int computeShader = compileShader(GL_COMPUTE_SHADER, computePath);

    return (createShaderProgram(computeShader));
}

unsigned int createShaderProgram(const char *vertexPath, const char *tesselationControlPath, const char *tesselationEvaluationPath, const char *fragmentPath)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int tesselationControlShader = compileShader(GL_TESS_CONTROL_SHADER, tesselationControlPath);
    unsigned int tesselationEvaluationShader = compileShader(GL_TESS_EVALUATION_SHADER, tesselationEvaluationPath);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    return (createShaderProgram(vertexShader, tesselationControlShader, tesselationEvaluationShader, fragmentShader));
}