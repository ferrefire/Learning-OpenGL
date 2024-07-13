#include "utilities.hpp"

void Utilities::PrintVec3(std::vector<glm::vec3> vec)
{
    for (const glm::vec3 &i : vec)
    {
        std::cout << i.x << ", " << i.y << ", " << i.z << std::endl;
    }
}

void Utilities::PrintVec3(glm::vec3 vec)
{
	std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
}

glm::vec3 Utilities::RotateVec3(glm::vec3 &vec, float angle, glm::vec3 axis)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(angle), axis);
    vec = rotation * glm::vec4(vec, 0.0f);
	return (vec);
}

void Utilities::Replace(std::string &str, const std::string &from, const std::string &to)
{
    size_t startPosition = str.find(from);
    if (startPosition == std::string::npos) return ;
    str.replace(startPosition, from.length(), to);
}

std::string Utilities::FileToString(const char *path)
{
    std::string fileString;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);

        std::stringstream fileStream;
        fileStream << file.rdbuf();

        file.close();
        fileString = fileStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::FILE_TO_STRING::FILE_NOT_SUCCESFULLY_READ" << " -error: " << e.what() << std::endl;
        quit(EXIT_FAILURE);
    }

    return (fileString);
}

void Utilities::Upper(std::string &str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        str[i] = toupper(str[i]);
    }
}

bool Utilities::Contains(const std::string &str, const std::string &find)
{
    return (str.find(find) != std::string::npos);
}
