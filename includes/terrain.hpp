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
		static int xi;
		static int yi;
		static int index;
		static bool generating;

	public:
		static float terrainSize;
		static float terrainChunkSize;
		static float terrainHeight;
		static float terrainLod0Size;
		static float terrainLod1Size;
		static float terrainShadowSize;

		static int terrainLod0Resolution;
		static int terrainLod1Resolution;
		static int terrainChunkResolution;
		static int terrainShadowResolution;
		static float terrainScale;
		static int terrainLayers;

		static int chunkRadius;
		static int chunksLength;
		static int chunkCount;

		static int terrainRadius;
		static int terrainLength;
		static int terrainCount;

		static bool computeOnTick;

		static glm::vec2 terrainOffset;
		static glm::vec2 offsetLod0;
		static glm::vec2 offsetLod1;
		static glm::vec2 seed;

		static Texture *heightMapLod0Texture;
		static Texture *heightMapLod1Texture;
		static Texture *heightMapArrayTexture;
		static Texture *shadowMapTexture;
		static Texture *depthMapTexture;

		static float worldSampleDistance;

		static int computePartLod0;
		static int computePartLod1;

		static Shader *terrainShader;
		static Shader *terrainLodShader;

		static Shader *heightMapComputeShader;
		static Shader *heightMapArrayComputeShader;
		static Shader *shadowMapComputeShader;

		static Mesh *terrainMesh;
		static Mesh *terrainLod0Mesh;
		static Mesh *terrainLod1Mesh;

		static Object ***terrainChunks;

		static void CreateTerrain();

		static void CreateTextures();
		static void CreateShaders();
		static void CreateChunks();

		static void GenerateHeightMap(int lod);
		static void GenerateHeightMapParts(int lod, glm::vec2 part);
		static void GenerateHeightMapArray();
		static void GenerateHeightMapArrayPart();
		static void GenerateShadowMap();

		static void RenderTerrain();
		static void CheckTerrainOffset();

		static void NewFrame();

		static bool Generating();
};

#endif