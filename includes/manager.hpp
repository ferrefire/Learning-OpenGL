#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <vector>
#include "object.hpp"
#include "shader.hpp"
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
        static bool wireframeActive;

        static void NewFrame();

        static void AddObject(Object *object);
        static void AddInstanceBatch(InstanceBatch instanceBatch);
        static void AddInstanceBatch(Mesh *mesh, int count);
        static void AddShader(Shader *shader);
};

#endif