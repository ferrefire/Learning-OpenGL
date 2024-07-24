#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "errors.hpp"
#include <filesystem>

class Utilities
{
    private:
        /* data */
    public:
		static void PrintVec3(std::vector<glm::vec3> vec);
		static void PrintVec3(const glm::vec3 &vec);
		static glm::vec3 RotateVec3(glm::vec3 &vec, float angle, glm::vec3 axis);
		static glm::vec3 RotateNewVec3(glm::vec3 vec, float angle, glm::vec3 axis);
		static void Replace(std::string &str, const std::string &from, const std::string &to);
        static std::string FileToString(const char *path);
        static void Upper(std::string &str);
        static bool Contains(const std::string &str, const std::string &find);
		static float Random11();
		static float Random01();
        static int Fits(float part, float whole);
        static float SignedFloor(float x);
};

#endif