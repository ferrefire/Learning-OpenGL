#ifndef GRASS_HPP
#define GRASS_HPP

#include "shader.hpp"
#include "mesh.hpp"
#include "buffer.hpp"

class Grass
{
	private:
		static float windFrequency;
		static float windFrequencyTarget;
		static float windStrength;
		static float windStrengthTarget;
		static bool windEnabled;

	public:
		static unsigned int grassCount;
		static unsigned int grassLodCount;
		static unsigned int grassRenderCount;
		static unsigned int grassLodRenderCount;

		static bool computeOnTick;
		static bool debugComputeTime;

		static Shader *grassShader;

		static Shader *grassComputeShader;

		static Buffer *grassBuffer;
		static Buffer *grassLodBuffer;
		static Buffer *countBuffer;
		static Buffer *countLodBuffer;

		static Mesh *grassMesh;
		static Mesh *grassLodMesh;

		static void CreateGrass();
		static void CreateShaders();
		static void CreateBuffers();
		static void CreateMeshes(); 

		static void RenderGrass();
		static void ComputeGrass(int lod = -1);

		static void NewFrame();
};

#endif