#ifndef TREES_HPP
#define TREES_HPP

#include "shader.hpp"
#include "mesh.hpp"
#include "buffer.hpp"

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

        static void ComputeTrees();
        static void RenderTrees();
        
        static void NewFrame();
};


#endif