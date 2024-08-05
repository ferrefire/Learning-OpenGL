#include "trees.hpp"
#include "manager.hpp"
#include "terrain.hpp"
#include "time.hpp"
#include <iostream>
#include "input.hpp"
#include "debug.hpp"
#include <thread>
#include <future>

void Trees::CreateTrees()
{
    CreateShaders();
    CreateBuffers();
    CreateMeshes();
}

void Trees::CreateShaders()
{
	unsigned int totalTreeCount = treeLod0Count + treeLod1Count + treeLod2Count;

    treeShader = new Shader("tree_vertex.glsl", "tree_fragment.glsl");
    Manager::AddShader(treeShader);
    treeShader->setInt("instanceCount", totalTreeCount * totalTreeCount);
	treeShader->setFloat("instanceMult", 1.0 / float(totalTreeCount * totalTreeCount));
	treeShader->setInt("instanceCountSqrt", totalTreeCount);
	treeShader->setFloat("instanceCountSqrtMult", 1.0 / float(totalTreeCount));
	treeShader->setFloat("lod0Range", float(treeLod0Count));
	treeShader->setFloat("lod1Range", float(treeLod0Count + treeLod1Count));
	//treeShader->setFloat("lod2Range", float(treeLod2Count));
	treeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	treeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	treeShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());

    treeComputeShader = new Shader("trees_compute_shader.glsl");
    Manager::AddShader(treeComputeShader);
    treeComputeShader->setInt("instanceCount", totalTreeCount * totalTreeCount);
	treeComputeShader->setFloat("instanceMult", 1.0 / float(totalTreeCount * totalTreeCount));
	treeComputeShader->setInt("instanceCountSqrt", totalTreeCount);
	treeComputeShader->setFloat("instanceCountSqrtMult", 1.0 / float(totalTreeCount));
	treeComputeShader->setFloat("lod0Range", float(treeLod0Count));
	treeComputeShader->setFloat("lod1Range", float(treeLod0Count + treeLod1Count));
	//treeComputeShader->setFloat("lod2Range", float(treeLod2Count));
	treeComputeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	treeComputeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	treeComputeShader->setInt(Terrain::heightMapArrayTexture->Name().c_str(), Terrain::heightMapArrayTexture->Index());
	Terrain::depthMapTexture->BindImage(1);
	//treeComputeShader->setInt(Terrain::depthMapTexture->Name().c_str(), Terrain::depthMapTexture->Index());
}

void Trees::CreateBuffers()
{
    treeLod0RenderBuffer = new Buffer(7, (treeLod0Count * treeLod0Count) * (sizeof(float) * 2));
    treeLod0RenderBuffer->CreateBuffer();
    Manager::AddBuffer(treeLod0RenderBuffer);

    treeLod0CountBuffer = new Buffer(8, sizeof(unsigned int));
    treeLod0CountBuffer->CreateBuffer();
    Manager::AddBuffer(treeLod0CountBuffer);

	treeLod1RenderBuffer = new Buffer(9, (treeLod1Count * treeLod1Count) * (sizeof(float) * 2));
	treeLod1RenderBuffer->CreateBuffer();
	Manager::AddBuffer(treeLod1RenderBuffer);

	treeLod1CountBuffer = new Buffer(10, sizeof(unsigned int));
	treeLod1CountBuffer->CreateBuffer();
	Manager::AddBuffer(treeLod1CountBuffer);

	treeLod2RenderBuffer = new Buffer(11, (treeLod2Count * treeLod2Count) * (sizeof(float) * 2));
	treeLod2RenderBuffer->CreateBuffer();
	Manager::AddBuffer(treeLod2RenderBuffer);

	treeLod2CountBuffer = new Buffer(12, sizeof(unsigned int));
	treeLod2CountBuffer->CreateBuffer();
	Manager::AddBuffer(treeLod2CountBuffer);
}

void Trees::CreateMeshes()
{
	treeLod0Mesh = GenerateTrunk(1.0);
	Manager::AddMesh(treeLod0Mesh);

	treeLod1Mesh = GenerateTrunk(0.75);
	Manager::AddMesh(treeLod1Mesh);

	treeLod2Mesh = GenerateTrunk(0.5);
	Manager::AddMesh(treeLod2Mesh);
}

void Trees::ComputeTrees(bool lod0, bool lod1, bool lod2)
{
	unsigned int totalTreeCount = treeLod0Count + treeLod1Count + treeLod2Count;

	treeComputeShader->useShader();
	treeComputeShader->setInt("renderLod0", lod0 ? 1 : 0);
	treeComputeShader->setInt("renderLod1", lod1 ? 1 : 0);
	treeComputeShader->setInt("renderLod2", lod2 ? 1 : 0);
	glDispatchCompute(totalTreeCount / 4, totalTreeCount / 4, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	if (lod0)
	{
		void *countPointer = treeLod0CountBuffer->GetPointer();
		treeLod0RenderCount = *(unsigned int *)countPointer;
		*(unsigned int *)countPointer = 0;
		treeLod0CountBuffer->UnMapBuffer();
	}
	if (lod1)
	{
		void *countPointer = treeLod1CountBuffer->GetPointer();
		treeLod1RenderCount = *(unsigned int *)countPointer;
		*(unsigned int *)countPointer = 0;
		treeLod1CountBuffer->UnMapBuffer();
	}
	if (lod2)
	{
		void *countPointer = treeLod2CountBuffer->GetPointer();
		treeLod2RenderCount = *(unsigned int *)countPointer;
		*(unsigned int *)countPointer = 0;
		treeLod2CountBuffer->UnMapBuffer();
	}
}

void Trees::RenderTrees(bool lod0, bool lod1, bool lod2)
{
    Manager::EnableCulling(true);
	treeShader->useShader();

	if (lod0)
	{
		treeShader->setInt("lod", 0);
		treeLod0Mesh->UseMesh();
		glDrawElementsInstanced(GL_TRIANGLES, treeLod0Mesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, treeLod0RenderCount);
	}
	if (lod1)
	{
		treeShader->setInt("lod", 1);
		treeLod1Mesh->UseMesh();
		glDrawElementsInstanced(GL_TRIANGLES, treeLod1Mesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, treeLod1RenderCount);
	}
	if (lod2)
	{
		treeShader->setInt("lod", 2);
		treeLod2Mesh->UseMesh();
		glDrawElementsInstanced(GL_TRIANGLES, treeLod2Mesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, treeLod2RenderCount);
	}
}

void Trees::NewFrame()
{
	if (Time::newFrameTick)
	{
		treeShader->setFloat3("computeViewPosition", Manager::camera.Position());

		//ComputeTrees(true);
		//RenderTrees(true);
		//ComputeTrees(false);
		//RenderTrees(false);

		ComputeTrees(true, true, true);
	}
	RenderTrees(true, true, true);
	//if (Manager::firstFrame) return ;

	/*if (Input::GetKey(GLFW_KEY_DOWN).pressed)
	{
		trunkQuality = glm::clamp(trunkQuality - 0.5, 0.0, 1.0);
		treeMesh = GenerateTrunk(trunkQuality);
		Manager::AddMesh(treeMesh);
	}
	else if (Input::GetKey(GLFW_KEY_UP).pressed)
	{
		trunkQuality = glm::clamp(trunkQuality + 0.5, 0.0, 1.0);
		treeMesh = GenerateTrunk(trunkQuality);
		Manager::AddMesh(treeMesh);
	}
	else if (Input::GetKey(GLFW_KEY_RIGHT).pressed)
	{
		trunkSeed += 1;
		treeMesh = GenerateTrunk(trunkQuality);
		Manager::AddMesh(treeMesh);
	}
	else if (Input::GetKey(GLFW_KEY_LEFT).pressed)
	{
		trunkSeed -= 1;
		treeMesh = GenerateTrunk(trunkQuality);
		Manager::AddMesh(treeMesh);
	}*/

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

void Trees::GenerateBranchThreaded(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, glm::vec3 angles, int splitTimes, bool main,
	std::promise<Shape> *promise)
{
	promise->set_value(GenerateBranch(resolution, base, offset, scale, angles, splitTimes, main));
}

Shape Trees::GenerateBranch(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, glm::vec3 angles, int splitTimes, bool main)
{
	float branchSeed = angles.length() + base.length() + offset.y + trunkSeed;

	Shape branch = Shape(CYLINDER, resolution);
	float sideAngle = 0;
	float sideAngleStart = 0;
	branch.Scale(glm::vec3(scale.x, scale.y, scale.x));

	int i = 0;
	float angleDiff = angles.x - angles.y;

	glm::vec2 vec2Offset = glm::vec2(offset.x, offset.z);
	sideAngle = GetBranchAngle(Utilities::Normalize(vec2Offset));
	sideAngleStart = angles.z;
	if (sideAngle > 180.0)
	{
		sideAngle = -(360.0 - sideAngle);
		//sideAngleStart = -(360.0 - sideAngleStart);
	}

	for (int y = 0; y <= resolution; y++)
	{
		float gradient = float(y) / float(resolution);

		float xAngle = angles.y + angleDiff * 0.5 + angleDiff * 0.5 * gradient;
		glm::mat4 xRotationMatrix = Utilities::GetRotationMatrix(xAngle, glm::vec3(1, 0, 0));

		float yAngle = glm::mix(sideAngleStart, sideAngle, gradient);
		glm::mat4 yRotationMatrix = Utilities::GetRotationMatrix(yAngle, glm::vec3(0, 1, 0));

		//glm::mat4 rotationMatrix = Utilities::GetRotationMatrix(glm::vec3(xAngle, yAngle, 0));

		for (int x = 0; x <= resolution; x++)
		{
			branch.GetVertices()[branch.GetVertexIndex(y, x)] = xRotationMatrix * 
				glm::vec4(branch.GetVertices()[branch.GetVertexIndex(y, x)], 1);
			branch.GetVertices()[branch.GetVertexIndex(y, x)] = yRotationMatrix * 
				glm::vec4(branch.GetVertices()[branch.GetVertexIndex(y, x)], 1);

			branch.GetNormals()[branch.GetVertexIndex(y, x)] = xRotationMatrix * 
				glm::vec4(branch.GetNormals()[branch.GetVertexIndex(y, x)], 0);
			branch.GetNormals()[branch.GetVertexIndex(y, x)] = yRotationMatrix * 
				glm::vec4(branch.GetNormals()[branch.GetVertexIndex(y, x)], 0);

			if (y == resolution && x == resolution && branch.centerMergePoint != -1)
			{
				branch.GetVertices()[branch.centerMergePoint] = xRotationMatrix * 
					glm::vec4(branch.GetVertices()[branch.centerMergePoint], 1);
				branch.GetVertices()[branch.centerMergePoint] = yRotationMatrix * 
					glm::vec4(branch.GetVertices()[branch.centerMergePoint], 1);

				branch.GetNormals()[branch.centerMergePoint] = xRotationMatrix * 
					glm::vec4(branch.GetNormals()[branch.centerMergePoint], 0);
				branch.GetNormals()[branch.centerMergePoint] = yRotationMatrix * 
					glm::vec4(branch.GetNormals()[branch.centerMergePoint], 0);
			}
		}
	}

	branch.RecalculateData();

	// branch.Rotate(sideAngle, glm::vec3(0, 1, 0));
	branch.Translate(base + offset);

	if (scale.x <= tmss) splitTimes = 0;

	if (splitTimes <= 0) return (branch);

	float angleSpacing = 360.0 / splitTimes;
	float angleMax = angleSpacing * 0.5;
	branchSeed = Utilities::Random11(branchSeed);
	float startAngle = branchSeed * 180.0;

	//int mainIndex = int(glm::ceil(Utilities::Random01() * splitTimes)) - 1;

	std::vector<std::promise<Shape>> promises = std::vector<std::promise<Shape>>(splitTimes);
	std::vector<std::future<Shape>> futures = std::vector<std::future<Shape>>(splitTimes);
	std::vector<std::thread> threads = std::vector<std::thread>(splitTimes);

	for (int i = 0; i < splitTimes; i++)
	{
		int subResolution = glm::clamp(int(glm::ceil(resolution * 0.5)), 4, resolution);

		branchSeed = Utilities::Random11(branchSeed);
		float newSubAngle = startAngle + (i * angleSpacing) + (branchSeed * angleMax);
		glm::vec3 subOffset = glm::vec3(0, 0, -1);
		Utilities::RotateVec3(subOffset, newSubAngle, glm::vec3(0, 1, 0));

		glm::vec3 subBase = branch.TopMergePointsCenter();

		branchSeed = Utilities::Random01(branchSeed);
		subOffset.y = 1.0 + branchSeed;
		subOffset *= (scale.x + scale.y) * (main ? 5 : 3);

		branchSeed = Utilities::Random01(branchSeed);
		float scaleMult = glm::mix(0.9, 1.1, branchSeed);
		glm::vec2 subScale = glm::vec2(scale.x * scaleMult * 0.6, scale.y * scaleMult * 0.75);

		float subAngle = angles.x;
		branchSeed = Utilities::Random01(branchSeed);
		if (i == 0) subAngle += glm::mix(0.0, 5.0, branchSeed);
		else subAngle += glm::mix(5.0, 60.0, branchSeed);
		glm::vec3 subAngles = glm::vec3(subAngle, angles.x, sideAngle);

		Utilities::RotateVec3(subOffset, subAngle, glm::vec3(1, 0, 0));
		Utilities::RotateVec3(subOffset, sideAngle, glm::vec3(0, 1, 0));

		// Shape subBranch = GenerateBranch(subResolution, subBase, subOffset, subScale, subAngles, 2, false);
		// branch.Join(subBranch, true);

		futures[i] = promises[i].get_future();
		threads[i] = std::thread(GenerateBranchThreaded, subResolution, subBase, subOffset, subScale, subAngles, tsbc, false, &promises[i]);
	}

	for (int i = 0; i < splitTimes; i++)
	{
		Shape subBranch = futures[i].get();
		threads[i].join();
		branch.Join(subBranch, true);
	}

	branch.CloseUnusedPoints();
	return (branch);
}

Mesh *Trees::GenerateTrunk(float quality)
{
	//Debug::DurationCheck();
	Shape *treeShape = new Shape();
	Manager::AddShape(treeShape);

	tr = glm::mix(minTrunkResolution, maxTrunkResolution, quality == 1 ? 1 : quality - 0.5);
	//tr = int(glm::round(tr * 0.5) * 2.0);
	tmbc = maxTrunkMainBranchCount;
	tsbc = maxTrunkSubBranchCount;
	tmss = glm::mix(minTrunkMinScaleSize, maxTrunkMinScaleSize, quality);

	Shape subBranch = GenerateBranch(tr, glm::vec3(0), glm::vec3(0), glm::vec2(1), glm::vec3(0), tmbc, true);
 	treeShape->Join(subBranch);

	//std::cout << treeShape->VertexCount() << std::endl;

	Mesh *trunk = new Mesh(treeShape, treeShader);
	//Debug::DurationCheck();
	//std::cout << trunkSeed << std::endl;
	return (trunk);
}