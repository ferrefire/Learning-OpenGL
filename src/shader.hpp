#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "shaders.hpp"

class Shader
{
    private:
        /* data */
    public:
        unsigned int ID;

        Shader(const char *vertexPath, const char *fragmentPath);

        void setBool(std::string name, bool value);
        void setInt(std::string name, int value);
        void setFloat(std::string name, float value);
		void setFloat2(std::string name, float x, float y);
        void setFloat4(std::string name, float x, float y, float z, float w);
};

#endif