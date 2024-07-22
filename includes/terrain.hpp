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
		static float terrainShadowSize;

		static int terrainLod0Resolution;
		static int terrainLod1Resolution;
		static int terrainChunkResolution;
		static int terrainNormalResolution;
		static int terrainOcclusionResolution;
		static int terrainShadowResolution;
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
		//static unsigned int heightMapArrayTexture;
		//static unsigned int occlusionMapTexture;

		static Texture *heightMapLod0Texture;
		static Texture *heightMapLod0Normal;
		static Texture *heightMapLod1Texture;
		static Texture *heightMapLod1Normal;
		static Texture *heightMapArrayTexture;
		static Texture *heightMapArrayNormal;
		static Texture *shadowMapTexture;

		static float worldSampleDistance;

		static int computePartLod0;
		static int computePartLod1;

		static Shader *terrainShader;
		static Shader *terrainLodShader;

		static Shader *heightMapComputeShader;
		static Shader *heightMapNormalComputeShader;
		static Shader *heightMapArrayComputeShader;
		static Shader *heightMapArrayNormalComputeShader;
		static Shader *occlusionMapComputeShader;
		static Shader *shadowMapComputeShader;

		static Mesh *terrainMesh;
		static Mesh *terrainLod0Mesh;
		static Mesh *terrainLod1Mesh;

		static Object ***terrainChunks;

		static void CreateTerrain(float terrainSize = -1, float terrainChunkSize = -1, float terrainHeight = -1, 
			int terrainResolution = -1, int terrainChunkResolution = -1, int chunkRadius = -1, int terrainRadius = -1);

		static void CreateTextures();
		static void CreateShaders();
		static void CreateChunks();

		static void GenerateHeightMap(int lod);
		static void GenerateHeightMapParts(int lod, glm::vec2 part);
		static void GenerateHeightMapArray();
		static void GenerateOcclusionMap();
		static void GenerateShadowMap();

		static void RenderTerrain();
		static void CheckTerrainOffset();
		static void NewFrame();
};

#endif