#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "shader.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "texture.hpp"
#include "cinematic.hpp"

class Manager
{
    private:
		static std::vector<Mesh *> meshes;
		static std::vector<Shape *> shapes;
		static std::vector<Texture *> textures;

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

		static Cinematic activeCinematic;

		static bool wireframeActive;
		static bool vSyncActive;
		static bool mouseLocked;
		static bool fullScreen;
		static bool cullingActive;
		static bool firstFrame;

		static glm::vec3 sunDirection;
		static glm::vec2 sunAngles;

		static unsigned int depthBuffer;

		//static int heightMapResolution;
		//static int heightMapChunkResolution;

		static void NewFrame();

        static void AddObject(Object *object);
        static void AddInstanceBatch(InstanceBatch instanceBatch);
        static void AddInstanceBatch(Mesh *mesh, int count);
		static void AddShader(Shader *shader);
		static void AddShape(Shape *shape);
		static void AddMesh(Mesh *mesh);
		static void AddTexture(Texture *texture);

		static void SetShaderVariables(Shader *shader);
		static void SetShaderFrameVariables();

		static void EnableCulling(bool mode);
		static void EnableVsync(bool mode);

		static void Close();
		static void Clean();
		static void Quit(int exitCode = EXIT_SUCCESS);

		static void SetCinematic(Cinematic &cinematic);
};

#endif