#include "grass.hpp"
#include "manager.hpp"
#include "terrain.hpp"
#include "time.hpp"

void Grass::CreateGrass()
{
	CreateShaders();
	CreateBuffers();
	CreateMeshes();
}

void Grass::CreateShaders()
{
	int totalGrassCount = grassCount + grassLodCount;

	grassShader = new Shader("grass_vertex.glsl", "grass_fragment.glsl");
	Manager::AddShader(grassShader);
	grassShader->setInt("instanceCount", totalGrassCount * totalGrassCount);
	grassShader->setFloat("instanceMult", 1.0 / float(totalGrassCount * totalGrassCount));
	grassShader->setInt("instanceCountSqrt", totalGrassCount);
	grassShader->setFloat("instanceCountSqrtMult", 1.0 / float(totalGrassCount));
	grassShader->setFloat("lodRange", float(grassCount) * 0.5);
	grassShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	grassShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	grassShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());
	//grassShader->setInt(Terrain::shadowMapTexture->Name().c_str(), Terrain::shadowMapTexture->Index());

	grassComputeShader = new Shader("grass_compute_shader.glsl");
	Manager::AddShader(grassComputeShader);
	grassComputeShader->setInt("instanceCount", totalGrassCount * totalGrassCount);
	grassComputeShader->setFloat("instanceMult", 1.0 / float(totalGrassCount * totalGrassCount));
	grassComputeShader->setInt("instanceCountSqrt", totalGrassCount);
	grassComputeShader->setFloat("instanceCountSqrtMult", 1.0 / float(totalGrassCount));
	grassComputeShader->setFloat("lodRange", float(grassCount) * 0.5);
	grassComputeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	grassComputeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	grassComputeShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());
	grassComputeShader->setInt(Terrain::depthMapTexture->Name().c_str(), Terrain::depthMapTexture->Index());
	//grassComputeShader->setInt(Terrain::shadowMapTexture->Name().c_str(), Terrain::shadowMapTexture->Index());
}

void Grass::CreateBuffers()
{
	grassBuffer = new Buffer(3, (grassCount * grassCount) * sizeof(float) * 4);
	grassBuffer->CreateBuffer();
	Manager::AddBuffer(grassBuffer);

	grassLodBuffer = new Buffer(4, (grassLodCount * grassLodCount) * sizeof(float) * 4);
	grassLodBuffer->CreateBuffer();
	Manager::AddBuffer(grassLodBuffer);

	countBuffer = new Buffer(5, sizeof(unsigned int));
	countBuffer->CreateBuffer();
	Manager::AddBuffer(countBuffer);

	countLodBuffer = new Buffer(6, sizeof(unsigned int));
	countLodBuffer->CreateBuffer();
	Manager::AddBuffer(countLodBuffer);
}

void Grass::CreateMeshes()
{
	Shape *bladeShape = new Shape(BLADE, 2);
	Manager::AddShape(bladeShape);

	Shape *bladeLodShape = new Shape(BLADE, 0);
	Manager::AddShape(bladeLodShape);

	grassMesh = new Mesh(bladeShape, grassShader);
	Manager::AddMesh(grassMesh);

	grassLodMesh = new Mesh(bladeLodShape, grassShader);
	Manager::AddMesh(grassLodMesh);
}

void Grass::RenderGrass()
{
	Manager::EnableCulling(false);
	grassShader->useShader();

	grassShader->setInt("lod", 0);
	grassMesh->UseMesh();
	glDrawElementsInstanced(GL_TRIANGLES, grassMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, grassRenderCount);

	grassShader->setInt("lod", 1);
	grassLodMesh->UseMesh();
	glDrawElementsInstanced(GL_TRIANGLES, grassLodMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, grassLodRenderCount);
}

void Grass::ComputeGrass(int lod)
{
	int totalGrassCount = grassCount + grassLodCount;

	grassComputeShader->useShader();
	grassComputeShader->setInt("specificLod", lod);
	glDispatchCompute(totalGrassCount / 4, totalGrassCount / 4, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	if (lod == 0 || lod == -1)
	{
		void *countPointer = countBuffer->GetPointer();
		grassRenderCount = *(unsigned int *)countPointer;
		*(unsigned int *)countPointer = 0;
		countBuffer->UnMapBuffer();

		grassShader->setFloat3("computeViewPosition", Manager::camera.Position());
	}
	if (lod == 1 || lod == -1)
	{
		void *countLodPointer = countLodBuffer->GetPointer();
		grassLodRenderCount = *(unsigned int *)countLodPointer;
		*(unsigned int *)countLodPointer = 0;
		countLodBuffer->UnMapBuffer();

		grassShader->setFloat3("computeViewPositionLod", Manager::camera.Position());
	}
}

void Grass::NewFrame()
{
	//maybe every tick
	if (Time::newFrameTick) ComputeGrass(-1);
	//if (Time::newTick) ComputeGrass(1);
	RenderGrass();
}