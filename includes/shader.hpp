#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "shaders.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
    private:
        /* data */
    public:
		static unsigned int currentActiveShader;

        unsigned int ID;

        Shader(const char *vertexPath, const char *fragmentPath);

		void useShader();

        void setBool(std::string name, bool value);
        void setInt(std::string name, int value);
        void setFloat(std::string name, float value);
        void setFloat2(std::string name, float x, float y);
        void setFloat3(std::string name, float x, float y, float z);
        void setFloat3(std::string name, glm::vec3 vec);
        void setFloat4(std::string name, float x, float y, float z, float w);
        void setMatrix4(std::string name, glm::mat4 transformation);
};

#endif