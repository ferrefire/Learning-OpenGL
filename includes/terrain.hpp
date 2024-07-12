#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "shader.hpp"
#include <glm/glm.hpp>
#include "object.hpp"
#include <vector>

class Terrain
{
	private:
		
	public:
		static float terrainSize;
		static float terrainChunkSize;

		static int terrainResolution;
		static int terrainChunkResolution;

		static int chunkRadius;
		static int chunksLength;
		static int chunkCount;

		static int terrainRadius;
		static int terrainLength;
		static int terrainCount;

		static glm::vec2 offset;

		static unsigned int heightMapTexture;
		static unsigned int heightMapArrayTexture;

		static float sampleStepSize;

		static Shader *terrainShader;

		static Shader *heightMapComputeShader;
		static Shader *heightMapArrayComputeShader;

		static Object *terrainObject;
		static Object ***terrainChunks;

		static void CreateTerrain(float terrainSize, float terrainChunkSize, int terrainResolution, int terrainChunkResolution, 
			int chunkRadius, int terrainRadius);
		static void CreateTerrain();

		static void CreateHeightMaps();
		static void CreateChunks();

		static void GenerateHeightMap();
		static void GenerateHeightMapArray();
};

#endif