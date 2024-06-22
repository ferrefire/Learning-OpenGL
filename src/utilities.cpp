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