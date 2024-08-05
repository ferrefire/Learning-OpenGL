#ifndef TREES_HPP
#define TREES_HPP

#include "shader.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include <future>

class Trees
{
    private:
        

    public:
        static unsigned int treeCount;

        static unsigned int treeRenderCount;
        
        static Shader *treeShader;

        static Shader *treeComputeShader;

        static Mesh *treeMesh;

        static Buffer *treeRenderBuffer;
        static Buffer *treeCountBuffer;

        static void CreateTrees();
        static void CreateShaders();
        static void CreateBuffers();
        static void CreateMeshes();
		static Mesh *GenerateTrunk();
		static Shape GenerateBranch(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, glm::vec3 angles, int splitTimes, bool main);
		static void GenerateBranchThreaded(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, glm::vec3 angles, 
			int splitTimes, bool main, std::promise<Shape> *promise);

		static void ComputeTrees();
        static void RenderTrees();
        
        static void NewFrame();
};


#endif