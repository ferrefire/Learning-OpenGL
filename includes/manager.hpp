#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <vector>
#include "shader.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "time.hpp"
#include "camera.hpp"
#include "rendering.hpp"

class Manager
{
    private:

    public:
        struct InstanceBatch
        {
            Mesh *mesh;
            int count;
        };

        static std::vector<Object *> objects;
        static std::vector<InstanceBatch> instanceBatches;
        static std::vector<Shader *> shaders;

		static Camera &camera;
		static GLFWwindow *window;
		static bool wireframeActive;
		static bool vSyncActive;
		static bool mouseLocked;

		static void NewFrame();

        static void AddObject(Object *object);
        static void AddInstanceBatch(InstanceBatch instanceBatch);
        static void AddInstanceBatch(Mesh *mesh, int count);
        static void AddShader(Shader *shader);
		static void SetShaderVariables(Shader *shader);
};

#endif