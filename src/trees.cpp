#include "trees.hpp"
#include "manager.hpp"
#include "terrain.hpp"
#include "time.hpp"
#include <iostream>

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
	Terrain::depthMapTexture->BindImage(1);
	//treeComputeShader->setInt(Terrain::depthMapTexture->Name().c_str(), Terrain::depthMapTexture->Index());
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
    //Shape *treeShape = new Shape(CYLINDER, 12);
    //Manager::AddShape(treeShape);

	//treeMesh = new Mesh(treeShape, treeShader);
	treeMesh = GenerateTrunk();
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

	//if (Time::newSecond) std::cout << treeRenderCount << std::endl;
}

float GetBranchAngle(glm::vec2 dir)
{
	float angle = 0;

	int signX = glm::sign(dir.x);
	int signZ = glm::sign(dir.y);

	float angleX = signX * dir.x;
	float angleZ = signZ * dir.y;

	if (signX == -1 && signZ != -1)
	{
		angle = (1.0 - angleX) * 90.0 + 270.0;
	}
	else if (signX == -1 && signZ == -1)
	{
		angle = angleX * 90.0 + 180.0;
	}
	else if (signZ == -1)
	{
		angle = (1.0 - angleX) * 90.0 + 90.0;
	}
	else
	{
		angle = angleX * 90.0;
	}

	return (angle);
}

Shape GenerateBranch(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec3 scale, float angle, int splitTimes)
{
	Shape branch = Shape(CYLINDER, resolution);
	branch.Scale(scale);
	branch.Rotate(angle, glm::vec3(1, 0, 0));
	glm::vec2 vec2Offset = glm::vec2(offset.x, offset.z);
	float sideAngle = GetBranchAngle(Utilities::Normalize(vec2Offset));
	branch.Rotate(sideAngle, glm::vec3(0, 1, 0));
	branch.Translate(base + offset);

	if (splitTimes > 0)
	{
		splitTimes--;

		glm::vec3 center = branch.TopMergePointsCenter();
		int branchRes = int(glm::ceil(resolution * 0.5));

		Shape subBranch = GenerateBranch(branchRes, center, 
			glm::vec3(5, 7.5, -5), glm::vec3(0.45, 0.35, 0.45), angle * 1.5, splitTimes);
		branch.Join(subBranch, true);
		subBranch = GenerateBranch(branchRes, center, 
			glm::vec3(-5, 7.5, 5), glm::vec3(0.45, 0.35, 0.45), angle * 1.5, splitTimes);
		branch.Join(subBranch, true);
	}
	
	return (branch);
}

Mesh *Trees::GenerateTrunk()
{
	Shape *treeShape = new Shape(CYLINDER, 12);
	Manager::AddShape(treeShape);

	glm::vec3 center = treeShape->TopMergePointsCenter();

	Shape branch = GenerateBranch(7, center, glm::vec3(5, 7.5, -7), glm::vec3(0.625, 0.5, 0.625), 40, 1);
	treeShape->Join(branch, true);

	branch = GenerateBranch(7, center, glm::vec3(-5, 10, 5), glm::vec3(0.75, 0.625, 0.75), 25, 1);
	treeShape->Join(branch, true);

	Mesh *trunk = new Mesh(treeShape, treeShader);
	return (trunk);
}