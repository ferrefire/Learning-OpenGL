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
		~Shader();

        void useShader();

        void setBool(const char *name, bool value);
        void setInt(const char *name, int value);
        void setFloat(const char *name, float value);
		void setFloat2(const char *name, float x, float y);
		void setFloat2(const char *name, const glm::vec2 &vec);
		void setFloat3(const char *name, float x, float y, float z);
        void setFloat3(const char *name, const glm::vec3 &vec);
        void setFloat4(const char *name, float x, float y, float z, float w);
        void setFloat4(const char *name, const glm::vec4 &vec);
        void setMatrix4(const char *name, const glm::mat4 &transformation);

		static void setBoolGlobal(const char *name, bool value);
		static void setIntGlobal(const char *name, int value);
		static void setFloatGlobal(const char *name, float value);
		static void setFloat2Global(const char *name, float x, float y);
		static void setFloat2Global(const char *name, const glm::vec2 &vec);
		static void setFloat3Global(const char *name, float x, float y, float z);
		static void setFloat3Global(const char *name, const glm::vec3 &vec);
		static void setFloat4Global(const char *name, float x, float y, float z, float w);
		static void setFloat4Global(const char *name, const glm::vec4 &vec);
		static void setMatrix4Global(const char *name, const glm::mat4 &transformation);
};

#endif