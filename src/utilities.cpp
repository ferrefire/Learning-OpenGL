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

glm::vec3 Utilities::RotateVec3(glm::vec3 vec, float angle, glm::vec3 axis)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::rotate(rotation, angle, axis);
    return (rotation * glm::vec4(vec, 0.0f));
}