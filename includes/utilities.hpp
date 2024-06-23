#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

class Utilities
{
    private:
        /* data */
    public:
		static void PrintVec3(std::vector<glm::vec3> vec);
		static void PrintVec3(glm::vec3 vec);
        static glm::vec3 RotateVec3(glm::vec3 vec, float angle, glm::vec3 axis);
};

#endif