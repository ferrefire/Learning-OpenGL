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
	grassShader = new Shader("grass_vertex.glsl", "grass_fragment.glsl");
	Manager::AddShader(grassShader);
	grassShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	grassShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	grassShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());
	grassShader->setInt(Terrain::shadowMapTexture->Name().c_str(), Terrain::shadowMapTexture->Index());

	grassComputeShader = new Shader("grass_compute_shader.glsl");
	Manager::AddShader(grassComputeShader);
	grassComputeShader->setInt("instanceCount", grassCount);
	grassComputeShader->setFloat("instanceMult", 1.0 / float(grassCount));
	grassComputeShader->setInt("instanceCountSqrt", grassCountSqrRoot);
	grassComputeShader->setFloat("instanceCountSqrtMult", 1.0 / float(grassCountSqrRoot));
	grassComputeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	grassComputeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	grassComputeShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());
	grassComputeShader->setInt(Terrain::shadowMapTexture->Name().c_str(), Terrain::shadowMapTexture->Index());
}

void Grass::CreateBuffers()
{
	grassBuffer = new Buffer(3, grassCount * sizeof(float) * 8);
	grassBuffer->CreateBuffer();
	Manager::AddBuffer(grassBuffer);

	countBuffer = new Buffer(4, sizeof(unsigned int));
	countBuffer->CreateBuffer();
	Manager::AddBuffer(countBuffer);
}

void Grass::CreateMeshes()
{
	Shape *bladeShape = new Shape(BLADE, 2);
	Manager::AddShape(bladeShape);

	grassMesh = new Mesh(bladeShape, grassShader);
	Manager::AddMesh(grassMesh);
}

void Grass::RenderGrass()
{
	Manager::EnableCulling(false);
	grassShader->useShader();
	grassMesh->UseMesh();
	glDrawElementsInstanced(GL_TRIANGLES, grassMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, grassRenderCount);
}

void Grass::ComputeGrass()
{
	grassComputeShader->useShader();
	glDispatchCompute(grassCountSqrRoot / 4, grassCountSqrRoot / 4, 1);
	void *countPointer = countBuffer->GetPointer();
	grassRenderCount = *(unsigned int *)countPointer;
	*(unsigned int *)countPointer = 0;
	countBuffer->UnMapBuffer();
}

void Grass::NewFrame()
{
	//maybe every tick
	if (Time::newFrameTick) ComputeGrass();
	RenderGrass();
}