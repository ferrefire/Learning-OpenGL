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

glm::vec3 GetSubOffset(glm::vec3 base, glm::vec3 direction)
{
	glm::vec2 baseV2 = Utilities::Normalize(glm::vec2(base.x, base.z));
	glm::vec2 directionV2 = Utilities::Normalize(glm::vec2(direction.x, direction.z));

	glm::vec2 averageV2 = Utilities::Normalize(baseV2 + directionV2);

	glm::vec3 subOffset = glm::vec3(averageV2.x, 0, averageV2.y);

	return (subOffset);
}

Shape GenerateBranch(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, float angle, int splitTimes, bool main)
{
	Shape branch = Shape(CYLINDER, resolution);
	float sideAngle = 0;
	if (!main)
	{
		branch.Scale(glm::vec3(scale.x, scale.y, scale.x));
		branch.Rotate(angle, glm::vec3(1, 0, 0));
		glm::vec2 vec2Offset = glm::vec2(offset.x, offset.z);
		sideAngle = GetBranchAngle(Utilities::Normalize(vec2Offset));
		branch.Rotate(sideAngle, glm::vec3(0, 1, 0));
		branch.Translate(base + offset);
	}

	if (scale.x < 0.1) splitTimes = 0;

	if (splitTimes <= 0) return (branch);

	float angleSpacing = 360.0 / splitTimes;
	float angleMax = angleSpacing * 0.5;

	for (int i = 0; i < splitTimes; i++)
	{
		int subResolution = glm::clamp(int(glm::ceil(resolution * 0.5)), 3, resolution);

		float newSubAngle = (i * angleSpacing) + (Utilities::Random11() * angleMax);
		glm::vec3 subOffset = glm::vec3(0, 0, -1);
		Utilities::RotateVec3(subOffset, newSubAngle, glm::vec3(0, 1, 0));
		
		//if (!main)
		//{
		//	glm::vec2 normOffset = Utilities::Normalize(glm::vec2(offset.x, offset.z));
		//	glm::vec2 averageOffset = Utilities::Normalize(glm::vec2(normOffset.x + subOffset.x, normOffset.y + subOffset.z));
		//	subOffset.x = averageOffset.x;
		//	subOffset.z = averageOffset.y;
		//}

		glm::vec3 subBase = branch.TopMergePointsCenter();

		//if (!main) subOffset = GetSubOffset(offset, subOffset);

		subOffset.y = 0.5 + Utilities::Random01();
		//subOffset.y = 0;
		subOffset *= (scale.x + scale.y) * 3;

		float scaleMult = glm::mix(0.9, 1.1, Utilities::Random01());
		glm::vec2 subScale = glm::vec2(scale.x * scaleMult * 0.6, scale.y * scaleMult * 0.75);
		//glm::vec2 subScale = glm::vec2(1);

		float subAngle = angle;
		subAngle += glm::mix(5.0, 45.0, Utilities::Random01());
		//float subAngle = angle + 45.0;

		if (!main)
		{
			Utilities::RotateVec3(subOffset, subAngle, glm::vec3(1, 0, 0));
			Utilities::RotateVec3(subOffset, sideAngle, glm::vec3(0, 1, 0));
		}

		Shape subBranch = GenerateBranch(subResolution, subBase, subOffset, subScale, subAngle, 3, false);

		branch.Join(subBranch, true);
	}
	
	return (branch);
}

/*Shape *GenerateMainBranch(int resolution, glm::vec3 scale, float angle, int splitTimes)
{
	Shape *branch = new Shape(CYLINDER, resolution);
	branch->Scale(scale);
	branch->Rotate(angle, glm::vec3(1, 0, 0));

	for (int i = 0; i < splitTimes; i++)
	{

	}

	return (branch);
}*/

Mesh *Trees::GenerateTrunk()
{
	Shape *treeShape = new Shape();
	Manager::AddShape(treeShape);

	Shape subBranch = GenerateBranch(24, glm::vec3(0), glm::vec3(0), glm::vec2(1), 0, 3, true);
 	treeShape->Join(subBranch);

	std::cout << treeShape->VertexCount() << std::endl;

	/*glm::vec3 center = treeShape->TopMergePointsCenter();

	Shape branch = GenerateBranch(7, center, glm::vec3(5, 7.5, -7), glm::vec3(0.625, 0.5, 0.625), 40, 4);
	treeShape->Join(branch, true);

	branch = GenerateBranch(7, center, glm::vec3(-5, 10, 5), glm::vec3(0.75, 0.625, 0.75), 25, 4);
	treeShape->Join(branch, true);

	branch = GenerateBranch(7, center, glm::vec3(6, 8.25, 6), glm::vec3(0.75, 0.625, 0.75), 45, 4);
	treeShape->Join(branch, true);*/

	Mesh *trunk = new Mesh(treeShape, treeShader);
	return (trunk);
}