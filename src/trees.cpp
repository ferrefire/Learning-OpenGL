#include "trees.hpp"
#include "manager.hpp"
#include "terrain.hpp"
#include "time.hpp"

void Trees::CreateTrees()
{
    CreateShaders();
    CreateBuffers();
    CreateMeshes();
}

void Trees::CreateShaders()
{
    treeShader = new Shader("tree_vertex.glsl", "tree_fragment.glsl");
    Manager::AddShader(treeShader);
    treeShader->setInt("instanceCount", treeCount * treeCount);
	treeShader->setFloat("instanceMult", 1.0 / float(treeCount * treeCount));
	treeShader->setInt("instanceCountSqrt", treeCount);
	treeShader->setFloat("instanceCountSqrtMult", 1.0 / float(treeCount));
	treeShader->setFloat("lodRange", float(treeCount) * 0.5);
	treeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	treeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	treeShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());

    treeComputeShader = new Shader("trees_compute_shader.glsl");
    Manager::AddShader(treeComputeShader);
    treeComputeShader->setInt("instanceCount", treeCount * treeCount);
	treeComputeShader->setFloat("instanceMult", 1.0 / float(treeCount * treeCount));
	treeComputeShader->setInt("instanceCountSqrt", treeCount);
	treeComputeShader->setFloat("instanceCountSqrtMult", 1.0 / float(treeCount));
	treeComputeShader->setFloat("lodRange", float(treeCount) * 0.5);
	treeComputeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	treeComputeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	treeComputeShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());
}

void Trees::CreateBuffers()
{
    treeRenderBuffer = new Buffer(7, (treeCount * treeCount) * (sizeof(float) * 2));
    treeRenderBuffer->CreateBuffer();
    Manager::AddBuffer(treeRenderBuffer);

    treeCountBuffer = new Buffer(8, sizeof(unsigned int));
    treeCountBuffer->CreateBuffer();
    Manager::AddBuffer(treeCountBuffer);
}

void Trees::CreateMeshes()
{
    Shape *treeShape = new Shape(TRUNK, 12);
    Manager::AddShape(treeShape);

    treeMesh = new Mesh(treeShape, treeShader);
    Manager::AddMesh(treeMesh);
}

void Trees::ComputeTrees()
{
    treeComputeShader->useShader();
	glDispatchCompute(treeCount / 4, treeCount / 4, 1);

    void *countPointer = treeCountBuffer->GetPointer();
	treeRenderCount = *(unsigned int *)countPointer;
	*(unsigned int *)countPointer = 0;
	treeCountBuffer->UnMapBuffer();

	treeShader->setFloat3("computeViewPosition", Manager::camera.Position());
}

void Trees::RenderTrees()
{
    Manager::EnableCulling(true);
	treeShader->useShader();

	treeMesh->UseMesh();
	glDrawElementsInstanced(GL_TRIANGLES, treeMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, treeRenderCount);
}

void Trees::NewFrame()
{
    if (Time::newFrameTick) ComputeTrees();
    RenderTrees();
}