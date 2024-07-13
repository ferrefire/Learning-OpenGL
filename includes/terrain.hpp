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
		static float terrainHeight;

		static int terrainResolution;
		static int terrainChunkResolution;
		static float terrainScale;
		static int terrainLayers;

		static int chunkRadius;
		static int chunksLength;
		static int chunkCount;

		static int terrainRadius;
		static int terrainLength;
		static int terrainCount;

		static glm::vec2 offset;
		static glm::vec2 seed;

		static unsigned int heightMapTexture;
		static unsigned int heightMapArrayTexture;

		static float worldSampleDistance;

		static Shader *terrainShader;

		static Shader *heightMapComputeShader;
		static Shader *heightMapArrayComputeShader;

		static Object ***terrainChunks;

		static void CreateTerrain(float terrainSize = -1, float terrainChunkSize = -1, float terrainHeight = -1, 
			int terrainResolution = -1, int terrainChunkResolution = -1, int chunkRadius = -1, int terrainRadius = -1);

		static void CreateHeightMaps();
		static void CreateChunks();

		static void GenerateHeightMap();
		static void GenerateHeightMapArray();
};

#endif