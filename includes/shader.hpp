#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "shaders.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "manager.hpp"

class Shader
{
    private:
        /* data */
    public:
		static unsigned int currentActiveShader;

        unsigned int ID;
        bool cullFaces = true;

        Shader(const char *vertexPath, const char *fragmentPath);
        Shader(const char *computePath);
        Shader(const char *vertexPath, const char *tesselationControlPath, const char *tesselationEvaluationPath, const char *fragmentPath);

        void useShader();

        void setBool(std::string name, bool value);
        void setInt(std::string name, int value);
        void setFloat(std::string name, float value);
        void setFloat2(std::string name, float x, float y);
        void setFloat3(std::string name, float x, float y, float z);
        void setFloat3(std::string name, glm::vec3 vec);
        void setFloat4(std::string name, float x, float y, float z, float w);
        void setFloat4(std::string name, glm::vec4 vec);
        void setMatrix4(std::string name, glm::mat4 transformation);

		static void setBoolGlobal(std::string name, bool value);
		static void setIntGlobal(std::string name, int value);
		static void setFloatGlobal(std::string name, float value);
		static void setFloat2Global(std::string name, float x, float y);
		static void setFloat3Global(std::string name, float x, float y, float z);
		static void setFloat3Global(std::string name, glm::vec3 vec);
		static void setFloat4Global(std::string name, float x, float y, float z, float w);
		static void setFloat4Global(std::string name, glm::vec4 vec);
		static void setMatrix4Global(std::string name, glm::mat4 transformation);
};

#endif