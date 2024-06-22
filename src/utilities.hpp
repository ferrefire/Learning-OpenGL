#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class Utilities
{
    private:
        /* data */
    public:
		static void PrintVec3(std::vector<glm::vec3> vec);
		static void PrintVec3(glm::vec3 vec);
};

#endif