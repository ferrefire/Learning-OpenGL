#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "shader.hpp"
#include <glm/glm.hpp>
#include "object.hpp"
#include <vector>
#include "mesh.hpp"
#include "texture.hpp"

class Terrain
{
	private:
		
	public:
		static float terrainSize;
		static float terrainOccludeSize;
		static float terrainChunkSize;
		static float terrainHeight;
		static float terrainLod0Size;
		static float terrainLod1Size;

		static int terrainLod0Resolution;
		static int terrainLod1Resolution;
		static int terrainChunkResolution;
		static int terrainOcclusionResolution;
		static float terrainScale;
		static int terrainLayers;

		static int chunkRadius;
		static int chunksLength;
		static int chunkCount;

		static int terrainRadius;
		static int terrainLength;
		static int terrainCount;

		static glm::vec2 terrainOffset;
		static glm::vec2 offsetLod0;
		static glm::vec2 offsetLod1;
		static glm::vec2 seed;

		//static unsigned int heightMapLod0Texture;
		//static unsigned int heightMapLod1Texture;
		static unsigned int heightMapArrayTexture;
		static unsigned int occlusionMapTexture;

		static Texture *heightMapLod0Texture;
		static Texture *heightMapLod1Texture;

		static float worldSampleDistance;

		static Shader *terrainShader;

		static Shader *heightMapComputeShader;
		static Shader *heightMapArrayComputeShader;
		static Shader *occlusionMapComputeShader;

		static Mesh *terrainMesh;
		static Mesh *terrainLodMesh;

		static Object ***terrainChunks;

		static void CreateTerrain(float terrainSize = -1, float terrainChunkSize = -1, float terrainHeight = -1, 
			int terrainResolution = -1, int terrainChunkResolution = -1, int chunkRadius = -1, int terrainRadius = -1);

		static void CreateTextures();
		static void CreateShaders();
		static void CreateChunks();

		static void GenerateHeightMap(int lod);
		static void GenerateHeightMapArray();
		static void GenerateOcclusionMap();

		static void RenderTerrain();

		static void NewFrame();
};

#endif