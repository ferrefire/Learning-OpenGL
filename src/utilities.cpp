#include "utilities.hpp"

void Utilities::PrintVec3(std::vector<glm::vec3> vec)
{
    for (const glm::vec3 &i : vec)
    {
        std::cout << i.x << "," << i.y << "," << i.z << std::endl;
    }
}

void Utilities::PrintVec3(const glm::vec3 &vec)
{
	std::cout << vec.x << "," << vec.y << "," << vec.z << std::endl;
}

glm::vec3 Utilities::RotateVec3(glm::vec3 &vec, float angle, glm::vec3 axis)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(angle), axis);
    vec = rotation * glm::vec4(vec, 0.0f);
	return (vec);
}

glm::vec3 Utilities::RotateNewVec3(glm::vec3 vec, float angle, glm::vec3 axis)
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

float Utilities::Random11()
{
	return ((Random01() - 0.5f) * 2.0f);
}

float Utilities::Random01()
{
	//srand(seed);
	seed = rand();
	return float(double(seed) / double(RAND_MAX));
}

float Utilities::SignedFloor(float x)
{
    if (x > 0) return floor(x);
    else return ceil(x);
}

int Utilities::Fits(float part, float whole)
{
    float times = whole / part;
    int result = SignedFloor(times);
    return result;
}

std::string Utilities::GetPath()
{
	std::filesystem::path currentPath = std::filesystem::current_path();

	while (Utilities::Contains(currentPath.string(), "build"))
	{
		currentPath = currentPath.parent_path();
	}
	
    std::string path = currentPath.string();
	
	return path;
}

float Utilities::Dot(glm::vec2 p1, glm::vec2 p2)
{
	p1 = glm::normalize(p1);
	p2 = glm::normalize(p2);

	float dot = 0;
	dot += p1.x * p2.x;
	dot += p1.y * p2.y;

	return (dot);
}

float Utilities::Angle(glm::vec2 p1, glm::vec2 p2)
{
	
}

glm::vec2 Utilities::Normalize(const glm::vec2 &vec)
{
	float unit = abs(vec.x) + abs(vec.y);
	if (unit == 0) return (glm::vec2(0));
	glm::vec2 result = glm::vec2(vec.x / unit, vec.y / unit);
	return (result);
}

glm::vec3 Utilities::Normalize(const glm::vec3 &vec)
{
	float unit = abs(vec.x) + abs(vec.y) + abs(vec.z);
	glm::vec3 result = glm::vec3(vec.x / unit, vec.y / unit, vec.z / unit);
	return (result);
}

glm::vec3 Utilities::Direction(const glm::vec3 &from, const glm::vec3 &to)
{
	glm::vec3 direction = glm::vec3(0);

	direction = to - from;
	direction = Normalize(direction);

	return (direction);
}

glm::vec3 Utilities::RandomDirection()
{
	glm::vec3 direction = glm::vec3(0);
	direction.x = Random11() * 10.0;
	direction.y = Random01() * 10.0;
	direction.z = Random11() * 10.0;

	direction = Normalize(direction);

	return (direction);
}
