#include "terrain.hpp"
#include "manager.hpp"
#include "shape.hpp"
#include "mesh.hpp"

void Terrain::CreateTerrain(float terrainSize, float terrainChunkSize, float terrainHeight,
	int terrainResolution, int terrainChunkResolution, int chunkRadius, int terrainRadius)
{
	if (terrainSize != -1) Terrain::terrainSize = terrainSize;
	if (terrainChunkSize != -1) Terrain::terrainChunkSize = terrainChunkSize;
	if (terrainResolution != -1) Terrain::terrainResolution = terrainResolution;
	if (terrainChunkResolution != -1) Terrain::terrainChunkResolution = terrainChunkResolution;
	if (chunkRadius != -1)
	{
		Terrain::chunkRadius = chunkRadius;
		Terrain::chunksLength = Terrain::chunkRadius * 2 + 1;
		Terrain::chunkCount = Terrain::chunksLength * Terrain::chunksLength;
	}
	if (terrainRadius != -1) 
	{
		Terrain::terrainRadius = terrainRadius;
		Terrain::terrainLength = terrainRadius * 2 + 1;
		Terrain::terrainCount = Terrain::terrainLength * Terrain::terrainLength;
	}

	//Terrain::worldSampleDistance *= Terrain::terrainScale;

	CreateHeightMaps();
	CreateChunks();
}

void Terrain::CreateHeightMaps()
{
	Terrain::heightMapComputeShader = new Shader("heightmap_compute.glsl");
	Manager::AddShader(Terrain::heightMapComputeShader);

	Terrain::heightMapArrayComputeShader = new Shader("heightmapArray_compute.glsl");
	Manager::AddShader(Terrain::heightMapArrayComputeShader);

	Terrain::heightMapComputeShader->setInt("heightMap", 0);
	Terrain::heightMapComputeShader->setFloat("resolution", Terrain::terrainResolution);
	Terrain::heightMapComputeShader->setFloat("resolutionMult", 1.0 / float(Terrain::terrainResolution));

	Terrain::heightMapArrayComputeShader->setInt("heightMapArray", 1);
	Terrain::heightMapArrayComputeShader->setFloat("resolution", Terrain::terrainChunkResolution);
	Terrain::heightMapArrayComputeShader->setFloat("resolutionMult", 1.0 / float(Terrain::terrainChunkResolution));
	Terrain::heightMapArrayComputeShader->setInt("chunksRadius", Terrain::chunkRadius);

	glGenTextures(1, &Terrain::heightMapTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Terrain::heightMapTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16_SNORM, Terrain::terrainResolution, Terrain::terrainResolution, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(0, Terrain::heightMapTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16_SNORM);

	//glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &Terrain::heightMapArrayTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, Terrain::heightMapArrayTexture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//Check if there is a difference when using GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R16_SNORM, Terrain::terrainChunkResolution, Terrain::terrainChunkResolution,
		Terrain::chunkCount, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(1, Terrain::heightMapArrayTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R16_SNORM);

	//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	GenerateHeightMap();
	GenerateHeightMapArray();
}

void Terrain::CreateChunks()
{
	Terrain::terrainShader = new Shader("terrain_vertex.glsl", "tesselation_control.glsl", "tesselation_evaluation.glsl", "terrain_fragment.glsl");
	Terrain::terrainShader->setInt("heightMap", 0);
	Terrain::terrainShader->setInt("heightMapArray", 1);

	Shape *plane = new Shape(PLANE);
	plane->Scale(glm::vec3(Terrain::terrainChunkSize));

	Mesh *planeMesh = new Mesh(plane, Terrain::terrainShader);

	Terrain::terrainChunks = new Object **[Terrain::terrainLength];
	for (int x = -Terrain::terrainRadius; x <= Terrain::terrainRadius; x++)
	{
		int xi = x + Terrain::terrainRadius;
		Terrain::terrainChunks[xi] = new Object *[Terrain::terrainLength];
		for (int y = -Terrain::terrainRadius; y <= Terrain::terrainRadius; y++)
		{
			int yi = y + Terrain::terrainRadius;
			Terrain::terrainChunks[xi][yi] = new Object(planeMesh);
			Terrain::terrainChunks[xi][yi]->Paint(glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));
			Terrain::terrainChunks[xi][yi]->Move(glm::vec3(x, 0, y) * Terrain::terrainChunkSize);
			Manager::AddObject(Terrain::terrainChunks[xi][yi]);
		}
	}

	Manager::AddShader(Terrain::terrainShader);
}

void Terrain::GenerateHeightMap()
{
	Terrain::heightMapComputeShader->useShader();
	glDispatchCompute(Terrain::terrainResolution / 4, Terrain::terrainResolution / 4, 1);
}

void Terrain::GenerateHeightMapArray()
{
	Terrain::heightMapArrayComputeShader->useShader();
	glDispatchCompute(Terrain::terrainChunkResolution / 4, Terrain::terrainChunkResolution / 4, 1);
}