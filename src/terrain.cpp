#include "terrain.hpp"
#include "manager.hpp"
#include "shape.hpp"
#include "mesh.hpp"
#include "rendering.hpp"
#include "input.hpp"
#include "time.hpp"
#include <iostream>

void Terrain::CreateTerrain()
{
	chunksLength = chunkRadius * 2 + 1;
	chunkCount = chunksLength * chunksLength;

	terrainLength = terrainRadius * 2 + 1;
	terrainCount = terrainLength * terrainLength;

	terrainSize = chunksLength * terrainChunkSize;

	std::cout << "terrain size: " << terrainSize / 1000.0 << "km2" << std::endl;

	//Manager::EnableVsync(false);

	CreateTextures();
	CreateShaders();
	GenerateHeightMap(0);
	GenerateHeightMap(1);
	xi = -chunkRadius;
	yi = -chunkRadius;
	index = 0;
	generating = true;
	//GenerateHeightMapArrayPart();
	//GenerateShadowMap();
	// GenerateOcclusionMap();
	CreateChunks();
}

void Terrain::CreateShaders()
{
	heightMapComputeShader = new Shader("heightmap_compute.glsl");
	Manager::AddShader(heightMapComputeShader);

	//shadowMapComputeShader = new Shader("terrain_shadows_compute.glsl");
	//Manager::AddShader(shadowMapComputeShader);
	//shadowMapComputeShader->setFloat("resolution", terrainShadowResolution);
	//shadowMapComputeShader->setFloat("resolutionMult", 1.0 / float(terrainShadowResolution));
	//shadowMapComputeShader->setInt(heightMapLod0Texture->Name().c_str(), heightMapLod0Texture->Index());
	//shadowMapComputeShader->setInt(heightMapLod1Texture->Name().c_str(), heightMapLod1Texture->Index());
	//shadowMapComputeShader->setInt(heightMapArrayTexture->Name().c_str(), heightMapArrayTexture->Index());

	//heightMapNormalComputeShader = new Shader("heightmap_normal_compute.glsl");
	//Manager::AddShader(heightMapNormalComputeShader);

	heightMapArrayComputeShader = new Shader("heightmapArray_compute.glsl");
	Manager::AddShader(heightMapArrayComputeShader);
	heightMapArrayComputeShader->setInt("resolution", heightMapArrayTexture->Resolution());
	heightMapArrayComputeShader->setFloat("resolutionMult", heightMapArrayTexture->ResolutionMultiplier());
	heightMapArrayComputeShader->setInt("chunksRadius", chunkRadius);
	heightMapArrayComputeShader->setInt(heightMapLod0Texture->Name().c_str(), heightMapLod0Texture->Index());
	heightMapArrayComputeShader->setInt(heightMapLod1Texture->Name().c_str(), heightMapLod1Texture->Index());

	//heightMapArrayNormalComputeShader = new Shader("heightmapArrayNormal_compute.glsl");
	//Manager::AddShader(heightMapArrayNormalComputeShader);
	//heightMapArrayNormalComputeShader->setFloat("resolution", terrainNormalResolution);
	//heightMapArrayNormalComputeShader->setFloat("resolutionMult", 1.0 / float(terrainNormalResolution));
	//heightMapArrayNormalComputeShader->setInt("chunksRadius", chunkRadius);

	//occlusionMapComputeShader = new Shader("occlusion_compute.glsl");
	//Manager::AddShader(occlusionMapComputeShader);
	//occlusionMapComputeShader->setInt(heightMapLod0Texture->Name().c_str(), 0);
	//occlusionMapComputeShader->setInt(heightMapLod1Texture->Name().c_str(), 1);
	//// occlusionMapComputeShader->setInt("heightMapLod1", 1);
	//occlusionMapComputeShader->setInt("heightMapArray", 2);
	//occlusionMapComputeShader->setFloat("resolution", terrainOcclusionResolution);
	//occlusionMapComputeShader->setFloat("resolutionMult", 1.0 / float(terrainOcclusionResolution));

	terrainShader = new Shader("terrain_vertex.glsl", "tesselation_control.glsl", "tesselation_evaluation.glsl",
							   "terrain_fragment.glsl");
	Manager::AddShader(terrainShader);
	terrainShader->setInt(heightMapLod0Texture->Name().c_str(), heightMapLod0Texture->Index());
	terrainShader->setInt(heightMapLod1Texture->Name().c_str(), heightMapLod1Texture->Index());
	terrainShader->setInt(heightMapArrayTexture->Name().c_str(), heightMapArrayTexture->Index());
	terrainShader->setFloat4("color", glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));

	terrainLodShader = new Shader("terrain_vertex_lod.glsl", "terrain_fragment_lod.glsl");
	Manager::AddShader(terrainLodShader);
	terrainLodShader->setInt(heightMapLod0Texture->Name().c_str(), heightMapLod0Texture->Index());
	terrainLodShader->setInt(heightMapLod1Texture->Name().c_str(), heightMapLod1Texture->Index());
	terrainLodShader->setInt(heightMapArrayTexture->Name().c_str(), heightMapArrayTexture->Index());
	terrainLodShader->setFloat4("color", glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));

	// terrainShader->setInt("occlusionMap", 3);
}

void Terrain::CreateTextures()
{
	heightMapLod0Texture = new Texture("heightMapLod0", 0, GL_TEXTURE0, terrainLod0Resolution, terrainLod0Resolution, GL_R16_SNORM);
	heightMapLod0Texture->CreateTexture();
	Manager::AddTexture(heightMapLod0Texture);

	// heightMapLod0Normal = new Texture("heightMapLod0Normal", 4, GL_TEXTURE4, terrainLod0Resolution, terrainLod0Resolution, GL_RGBA16_SNORM);
	// heightMapLod0Normal->SetColorChannels(GL_RGBA);
	// heightMapLod0Normal->CreateTexture();
	// Manager::AddTexture(heightMapLod0Normal);

	heightMapLod1Texture = new Texture("heightMapLod1", 1, GL_TEXTURE1, terrainLod1Resolution, terrainLod1Resolution, GL_R16_SNORM);
	heightMapLod1Texture->CreateTexture();
	Manager::AddTexture(heightMapLod1Texture);

	// heightMapLod1Normal = new Texture("heightMapLod1Normal", 5, GL_TEXTURE5, terrainLod1Resolution, terrainLod1Resolution, GL_RGBA16_SNORM);
	// heightMapLod1Normal->SetColorChannels(GL_RGBA);
	// heightMapLod1Normal->CreateTexture();
	// Manager::AddTexture(heightMapLod1Normal);

	heightMapArrayTexture = new Texture("heightMapArray", 2, GL_TEXTURE2, terrainChunkResolution, terrainChunkResolution, GL_R16_SNORM);
	heightMapArrayTexture->SetDimensions(terrainChunkResolution, terrainChunkResolution, chunkCount);
	heightMapArrayTexture->SetTextureType(GL_TEXTURE_2D_ARRAY);
	heightMapArrayTexture->CreateTexture();
	Manager::AddTexture(heightMapArrayTexture);

	// heightMapArrayNormal = new Texture("heightMapArrayNormal", 3, GL_TEXTURE3, terrainNormalResolution, terrainNormalResolution, GL_RGBA16_SNORM);
	// heightMapArrayNormal->SetDimensions(terrainNormalResolution, terrainNormalResolution, chunkCount);
	// heightMapArrayNormal->SetTextureType(GL_TEXTURE_2D_ARRAY);
	// heightMapArrayNormal->SetColorChannels(GL_RGBA);
	// heightMapArrayNormal->CreateTexture();
	// Manager::AddTexture(heightMapArrayNormal);

	shadowMapTexture = new Texture("shadowMap", 3, GL_TEXTURE3, terrainShadowResolution, terrainShadowResolution, GL_R16_SNORM);
	shadowMapTexture->CreateTexture();
	Manager::AddTexture(shadowMapTexture);
}

void Terrain::CreateChunks()
{
	Shape *plane = new Shape(PLANE, 100);
	Manager::AddShape(plane);
	Shape *lod0Plane = new Shape(PLANE, 10);
	Manager::AddShape(lod0Plane);
	//Shape *lod1Plane = new Shape(PLANE, 25);
	//Manager::AddShape(lod1Plane);
	plane->Scale(glm::vec3(terrainChunkSize));
	lod0Plane->Scale(glm::vec3(terrainChunkSize));
	//lod1Plane->Scale(glm::vec3(terrainChunkSize));

	terrainMesh = new Mesh(plane, terrainShader);
	Manager::AddMesh(terrainMesh);
	terrainLod0Mesh = new Mesh(lod0Plane, terrainShader);
	Manager::AddMesh(terrainLod0Mesh);
	//terrainLod1Mesh = new Mesh(lod1Plane, terrainLodShader);
	//Manager::AddMesh(terrainLod1Mesh);

	terrainChunks = new Object **[terrainLength];
	for (int x = -terrainRadius; x <= terrainRadius; x++)
	{
		int xi = x + terrainRadius;
		terrainChunks[xi] = new Object *[terrainLength];
		for (int y = -terrainRadius; y <= terrainRadius; y++)
		{
			int yi = y + terrainRadius;
			terrainChunks[xi][yi] = new Object(terrainLod0Mesh);
			terrainChunks[xi][yi]->Paint(glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));
			terrainChunks[xi][yi]->Move(glm::vec3(x, 0, y) * terrainChunkSize);
			terrainChunks[xi][yi]->GetMesh()->GetShader()->setMatrix4("model", terrainChunks[xi][yi]->Translation());
			// terrainChunks[xi][yi]->GetMesh()->GetShader()->setFloat4("color", terrainChunks[xi][yi]->Color());
			Manager::AddObject(terrainChunks[xi][yi]);
		}
	}
}

void Terrain::GenerateHeightMapParts(int lod, glm::vec2 part)
{
	//if (lod == 0)
	//{
	//	heightMapComputeShader->useShader();
	//	heightMapLod0Texture->BindImage(0);
	//	heightMapComputeShader->setInt("resolution", heightMapLod0Texture->Resolution());
	//	heightMapComputeShader->setFloat("resolutionMult", heightMapLod0Texture->ResolutionMultiplier());
	//	heightMapComputeShader->setFloat("mapScale", terrainLod0Size / terrainChunkSize);
	//	heightMapComputeShader->setFloat2("offset", offsetLod0 / terrainLod0Size);
	//	heightMapComputeShader->setFloat2("worldOffset", terrainOffset / terrainChunkSize);
	//	//heightMapComputeShader->setFloat2("part", part);
	//	glDispatchCompute(heightMapLod0Texture->Resolution() / 8, heightMapLod0Texture->Resolution() / 8, 1);
	//}
	//else if (lod == 1)
	//{
	//	heightMapComputeShader->useShader();
	//	heightMapLod1Texture->BindImage(0);
	//	heightMapComputeShader->setInt("resolution", heightMapLod1Texture->Resolution());
	//	heightMapComputeShader->setFloat("resolutionMult", heightMapLod1Texture->ResolutionMultiplier());
	//	heightMapComputeShader->setFloat("mapScale", terrainLod1Size / terrainChunkSize);
	//	heightMapComputeShader->setFloat2("offset", offsetLod1 / terrainLod1Size);
	//	heightMapComputeShader->setFloat2("worldOffset", terrainOffset / terrainChunkSize);
	//	//heightMapComputeShader->setFloat2("part", 0, 0);
	//	glDispatchCompute(heightMapLod1Texture->Resolution() / 4, heightMapLod1Texture->Resolution() / 32, 1);
	//}
}

void Terrain::GenerateHeightMap(int lod)
{
	if (lod == 0)
	{
		Shader::setFloat2Global("terrainOffsetLod0", offsetLod0);
		heightMapComputeShader->useShader();
		heightMapLod0Texture->BindImage(0);
		heightMapComputeShader->setInt("resolution", heightMapLod0Texture->Resolution());
		heightMapComputeShader->setFloat("resolutionMult", heightMapLod0Texture->ResolutionMultiplier());
		heightMapComputeShader->setFloat("mapScale", terrainLod0Size / terrainChunkSize);
		heightMapComputeShader->setFloat2("offset", offsetLod0 / terrainLod0Size);
		heightMapComputeShader->setFloat2("worldOffset", terrainOffset / terrainChunkSize);
		heightMapComputeShader->setFloat2("part", 0, 0);
		glDispatchCompute(heightMapLod0Texture->Resolution() / 4, heightMapLod0Texture->Resolution() / 4, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
	else if (lod == 1)
	{
		Shader::setFloat2Global("terrainOffsetLod1", offsetLod1);
		heightMapComputeShader->useShader();
		heightMapLod1Texture->BindImage(0);
		heightMapComputeShader->setInt("resolution", heightMapLod1Texture->Resolution());
		heightMapComputeShader->setFloat("resolutionMult", heightMapLod1Texture->ResolutionMultiplier());
		heightMapComputeShader->setFloat("mapScale", terrainLod1Size / terrainChunkSize);
		heightMapComputeShader->setFloat2("offset", offsetLod1 / terrainLod1Size);
		heightMapComputeShader->setFloat2("worldOffset", terrainOffset / terrainChunkSize);
		heightMapComputeShader->setFloat2("part", 0, 0);
		glDispatchCompute(heightMapLod1Texture->Resolution() / 4, heightMapLod1Texture->Resolution() / 4, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}

void Terrain::GenerateHeightMapArray()
{
	heightMapArrayComputeShader->useShader();
	heightMapArrayTexture->BindImage(0);
	//glDispatchCompute(heightMapArrayTexture->Resolution() / 4, heightMapArrayTexture->Resolution() / 4, 1);

	int index = 0;
	for (int xi = -chunkRadius; xi <= chunkRadius; xi++)
	{
		for (int yi = -chunkRadius; yi <= chunkRadius; yi++)
		{
			heightMapArrayComputeShader->setInt("chunkIndex", index);
			heightMapArrayComputeShader->setFloat2("chunkCoordinates", xi, yi);
			glDispatchCompute(heightMapArrayTexture->Resolution() / 4, heightMapArrayTexture->Resolution() / 4, 1);
			index++;
		}
	}

	//if (heightMapArrayNormal)
	//{
	//	heightMapArrayNormalComputeShader->useShader();
	//	heightMapArrayNormal->BindImage(0);
	//	glDispatchCompute(heightMapArrayNormal->Resolution() / 4, heightMapArrayNormal->Resolution() / 4, 1);
	//}
}

void Terrain::GenerateHeightMapArrayPart()
{
	heightMapArrayComputeShader->useShader();
	heightMapArrayTexture->BindImage(0);

	heightMapArrayComputeShader->setInt("chunkIndex", index);
	heightMapArrayComputeShader->setFloat2("chunkCoordinates", xi, yi);
	glDispatchCompute(heightMapArrayTexture->Resolution() / 4, heightMapArrayTexture->Resolution() / 4, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	index++;
	if (index >= chunkCount) 
	{
		generating = false;
	}

	if (yi < chunkRadius)
	{
		yi++;
	}
	else 
	{
		yi = -chunkRadius;
		xi++;
	}
}

void Terrain::GenerateOcclusionMap()
{
	occlusionMapComputeShader->useShader();
	glDispatchCompute(terrainOcclusionResolution / 4, terrainOcclusionResolution / 4, 1);
}

void Terrain::GenerateShadowMap()
{
	shadowMapComputeShader->useShader();
	shadowMapTexture->BindImage(0);
	glDispatchCompute(shadowMapTexture->Resolution() / 4, shadowMapTexture->Resolution() / 4, 1);
}

bool InView(const glm::vec3 &position, float tolerance, const glm::mat4 &projection, const glm::mat4 &view)
{
	glm::vec4 viewSpace = projection * view * glm::vec4(position, 1.0);

	glm::vec3 clipSpace = viewSpace;
	clipSpace /= -viewSpace.w;

	clipSpace.x = clipSpace.x * 0.5f + 0.5f;
	clipSpace.y = clipSpace.y * 0.5f + 0.5f;
	clipSpace.z = viewSpace.w;

	if (clipSpace.z <= 0.0 || clipSpace.z >= Manager::camera.far) return false;

	return !(clipSpace.x < -tolerance || clipSpace.x > 1.0f + tolerance ||
			 clipSpace.y < -tolerance || clipSpace.y > 1.0f + tolerance);
}

bool ChunkInView(const glm::vec3 &position, float tolerance, const glm::mat4 &projection, const glm::mat4 &view, bool main = true)
{
	// position.y += Terrain::terrainHeight * 0.5;
	if (InView(position, tolerance, projection, view))
		return true;
	else if (InView(position + glm::vec3(Terrain::terrainChunkSize * 0.5, 0, Terrain::terrainChunkSize * 0.5),
					tolerance, projection, view))
		return true;
	else if (InView(position + glm::vec3(Terrain::terrainChunkSize * -0.5, 0, Terrain::terrainChunkSize * -0.5),
					tolerance, projection, view))
		return true;
	else if (InView(position + glm::vec3(Terrain::terrainChunkSize * -0.5, 0, Terrain::terrainChunkSize * 0.5),
					tolerance, projection, view))
		return true;
	else if (InView(position + glm::vec3(Terrain::terrainChunkSize * 0.5, 0, Terrain::terrainChunkSize * -0.5),
					tolerance, projection, view))
		return true;
	else if (main)
		return ChunkInView(position + glm::vec3(0, Terrain::terrainHeight * 0.5, 0), tolerance, projection, view, false);
	return false;
}

void Terrain::RenderTerrain()
{
	Manager::EnableCulling(true);
	terrainShader->useShader();

	float sx = Manager::camera.Position().x / float(terrainChunkSize);
	float sz = Manager::camera.Position().z / float(terrainChunkSize);

	glm::mat4 projection = Manager::camera.Projection();
	glm::mat4 view = Manager::camera.View();

	int amount = 0;

	for (int x = -terrainRadius; x <= terrainRadius; x++)
	{
		for (int z = -terrainRadius; z <= terrainRadius; z++)
		{
			int ix = x + terrainRadius;
			int iz = z + terrainRadius;
			float fx = float(x);
			float fz = float(z);
			float distance = glm::clamp(glm::distance(glm::vec2(fx, fz), glm::vec2(sx, sz)), 0.0f, float(terrainLength));
			bool inView = ChunkInView(terrainChunks[ix][iz]->Position(), 0, projection, view);

			if ((inView && distance < 1.0) || distance <= 0.75)
			{
				terrainChunks[ix][iz]->GetPosition().y = 0;
				terrainMesh->UseMesh();
				terrainShader->setMatrix4("model", terrainChunks[ix][iz]->Translation());
				//terrainShader->setFloat("tesselationFactor", 10);
				glDrawElements(GL_PATCHES, terrainMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
			}
			else if (inView)
			{
				float factor = 1.0 - (distance - 1) / float(terrainLength - 1);

				terrainChunks[ix][iz]->GetPosition().y = pow(factor, 0.5) * -10;
				terrainLod0Mesh->UseMesh();
				terrainShader->setMatrix4("model", terrainChunks[ix][iz]->Translation());
				//terrainShader->setFloat("tesselationFactor", 10.0 + pow(factor, 2) * 10);

				glDrawElements(GL_PATCHES, terrainLod0Mesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
				amount++;
			}
			// else if (inView)
			//{
			//	//terrainLodShader->useShader();
			//	terrainChunks[ix][iz]->GetPosition().y = -20;
			//	terrainLodShader->useShader();
			//	terrainLod1Mesh->UseMesh();
			//	terrainLodShader->setMatrix4("model", terrainChunks[ix][iz]->Translation());
			//	glDrawElements(GL_TRIANGLES, terrainLod1Mesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
			//	amount++;
			// }
		}
	}
}

void Terrain::NewFrame()
{
	//if (Input::GetKey(GLFW_KEY_G).pressed)
	//{
	//	seed = glm::vec2(int(Utilities::Random11() * 100), int(Utilities::Random11() * 100));
	//	heightMapComputeShader->setFloat2("seed", seed);
	//	heightMapArrayComputeShader->setFloat2("seed", seed);
	//	GenerateHeightMap(0);
	//	GenerateHeightMap(1);
	//	GenerateHeightMapArray();
	//}

	if (generating) GenerateHeightMapArrayPart();

	if (Time::newSubTick)
		CheckTerrainOffset();
	else
		RenderTerrain();
}

void Terrain::CheckTerrainOffset()
{
	float xw = Manager::camera.Position().x;
	float zw = Manager::camera.Position().z;

	bool moveTerrain = false;
	bool moveLod0 = false;
	bool moveLod1 = false;
	float terrainStep = 1;

	if (abs(xw) >= terrainChunkSize * terrainStep || abs(zw) >= terrainChunkSize * terrainStep)
		moveTerrain = true;

	if (moveTerrain)
	{
		// RenderTerrain();
		glm::vec2 newOffset = glm::vec2(Utilities::Fits(terrainChunkSize * terrainStep, xw), 
			Utilities::Fits(terrainChunkSize * terrainStep, zw)) * terrainChunkSize * terrainStep;
		terrainOffset += newOffset;
		offsetLod0 = glm::vec2(0);
		offsetLod1 = glm::vec2(0);
		Manager::camera.Move(-glm::vec3(newOffset.x, 0, newOffset.y));
		xw = Manager::camera.Position().x;
		zw = Manager::camera.Position().z;

		// std::cout << "terrain regenerated" << std::endl;
	}

	float x0 = xw - offsetLod0.x;
	float z0 = zw - offsetLod0.y;
	float x1 = xw - offsetLod1.x;
	float z1 = zw - offsetLod1.y;
	float lod0Step = 0.125;
	float lod1Step = 0.25;

	if (moveTerrain || abs(x0) >= terrainLod0Size * lod0Step || abs(z0) >= terrainLod0Size * lod0Step)
		moveLod0 = true;
	if (moveTerrain || abs(x1) >= terrainLod1Size * lod1Step || abs(z1) >= terrainLod1Size * lod1Step)
		moveLod1 = true;

	if (moveLod0 && computePartLod0 == 0)
	{
		glm::vec2 newOffset = glm::vec2(Utilities::Fits(terrainLod0Size * lod0Step, x0), Utilities::Fits(terrainLod0Size * lod0Step, z0)) * terrainLod0Size * lod0Step;
		offsetLod0 += newOffset;
		GenerateHeightMap(0);
	}
	if (moveLod1 && computePartLod1 == 0)
	{
		glm::vec2 newOffset = glm::vec2(Utilities::Fits(terrainLod1Size * lod1Step, x1), Utilities::Fits(terrainLod1Size * lod1Step, z1)) * terrainLod1Size * lod1Step;
		offsetLod1 += newOffset;
		GenerateHeightMap(1);
	}

	RenderTerrain();
	if (moveTerrain)
	{
		Shader::setFloat2Global("terrainWorldOffset", terrainOffset);
	}
}

bool Terrain::Generating()
{
	return generating;
}