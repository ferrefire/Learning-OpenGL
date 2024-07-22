#ifndef GRASS_HPP
#define GRASS_HPP

#include "shader.hpp"
#include "mesh.hpp"
#include "buffer.hpp"

class Grass
{
	private:
		/* data */
	public:
		static unsigned int grassCount;
		static unsigned int grassCountSqrRoot;
		static unsigned int grassRenderCount;

		static Shader *grassShader;

		static Shader *grassComputeShader;

		static Buffer *grassBuffer;
		static Buffer *countBuffer;

		static Mesh *grassMesh;

		static void CreateGrass();
		static void CreateShaders();
		static void CreateBuffers();
		static void CreateMeshes(); 

		static void RenderGrass();
		static void ComputeGrass();

		static void NewFrame();
};

#endif