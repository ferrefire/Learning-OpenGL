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
	//Debug::DurationCheck();
	treeMesh = GenerateTrunk();
	//Debug::DurationCheck();
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

	if (Input::GetKey(GLFW_KEY_G).pressed)
	{
		//srand(Time::GetTime());
		treeMesh = GenerateTrunk();
		Manager::AddMesh(treeMesh);
	}

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

	if (scale.x < 0.1) splitTimes = 0;

	if (splitTimes <= 0) return (branch);

	float angleSpacing = 360.0 / splitTimes;
	float angleMax = angleSpacing * 0.5;
	float startAngle = Utilities::Random11() * 180.0;

	//int mainIndex = int(glm::ceil(Utilities::Random01() * splitTimes)) - 1;

	std::vector<std::promise<Shape>> promises = std::vector<std::promise<Shape>>(splitTimes);
	std::vector<std::future<Shape>> futures = std::vector<std::future<Shape>>(splitTimes);
	std::vector<std::thread> threads = std::vector<std::thread>(splitTimes);

	for (int i = 0; i < splitTimes; i++)
	{
		//int subResolution = glm::clamp(resolution - int(glm::ceil(float(resolution) / 3.0)), 4, resolution);
		int subResolution = glm::clamp(int(glm::ceil(resolution * 0.5)), 4, resolution);
		//if (subResolution % 2 == 1) subResolution += 1;

		float newSubAngle = startAngle + (i * angleSpacing) + (Utilities::Random11() * angleMax);
		//if (newSubAngle >= 360.0) newSubAngle -= 360.0;
		glm::vec3 subOffset = glm::vec3(0, 0, -1);
		Utilities::RotateVec3(subOffset, newSubAngle, glm::vec3(0, 1, 0));

		glm::vec3 subBase = branch.TopMergePointsCenter();

		subOffset.y = 1.0 + Utilities::Random01();
		subOffset *= (scale.x + scale.y) * (main ? 5 : 3);

		float scaleMult = glm::mix(0.9, 1.1, Utilities::Random01());
		//if (i != 0) scaleMult -= 0.1;
		glm::vec2 subScale = glm::vec2(scale.x * scaleMult * 0.6, scale.y * scaleMult * 0.75);
		//glm::vec2 subScale = glm::vec2(1);

		float subAngle = angles.x;
		if (i == 0) subAngle += glm::mix(0.0, 5.0, Utilities::Random01());
		else subAngle += glm::mix(5.0, 60.0, Utilities::Random01());
		//float subAngle = angle + 45.0;
		glm::vec3 subAngles = glm::vec3(subAngle, angles.x, sideAngle);

		Utilities::RotateVec3(subOffset, subAngle, glm::vec3(1, 0, 0));
		Utilities::RotateVec3(subOffset, sideAngle, glm::vec3(0, 1, 0));

		//Shape subBranch = GenerateBranch(subResolution, subBase, subOffset, subScale, subAngles, 2, false);

		//std::promise<Shape> branchSetter;
		//std::future<Shape> branchGetter = branchSetter.get_future();
		//promises[i] = std::promise<Shape>();
		futures[i] = promises[i].get_future();
		threads[i] = std::thread(GenerateBranchThreaded, subResolution, subBase, subOffset, subScale, subAngles, 2, false, &promises[i]);
		//Shape subBranch = branchGetter.get();
		//branchThread.join();
		//std::cout << subBranch.GetVertices().size() << std::endl;

		//branch.Join(subBranch, true);
	}

	for (int i = 0; i < splitTimes; i++)
	{
		Shape subBranch = futures[i].get();
		threads[i].join();
		//branchThread.join();
		branch.Join(subBranch, true);
	}

	branch.CloseUnusedPoints();
	return (branch);
}

Mesh *Trees::GenerateTrunk()
{
	Debug::DurationCheck();
	Shape *treeShape = new Shape();
	Manager::AddShape(treeShape);

	Shape subBranch = GenerateBranch(24, glm::vec3(0), glm::vec3(0), glm::vec2(1), glm::vec3(0), 4, true);
 	treeShape->Join(subBranch);

	std::cout << treeShape->VertexCount() << std::endl;

	Mesh *trunk = new Mesh(treeShape, treeShader);
	Debug::DurationCheck();
	return (trunk);
}