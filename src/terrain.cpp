#include "terrain.hpp"
#include "manager.hpp"
#include "shape.hpp"
#include "mesh.hpp"
#include "rendering.hpp"
#include "input.hpp"

void Terrain::CreateTerrain(float terrainSize, float terrainChunkSize, float terrainHeight,
	int terrainLod0Resolution, int terrainChunkResolution, int chunkRadius, int terrainRadius)
{
	//if (terrainSize != -1) Terrain::terrainSize = terrainSize;
	//if (terrainChunkSize != -1) Terrain::terrainChunkSize = terrainChunkSize;
	//if (terrainLod0Resolution != -1) Terrain::terrainLod0Resolution = terrainLod0Resolution;
	//if (terrainChunkResolution != -1) Terrain::terrainChunkResolution = terrainChunkResolution;
	//if (chunkRadius != -1)
	//{
	//	Terrain::chunkRadius = chunkRadius;
	//	Terrain::chunksLength = Terrain::chunkRadius * 2 + 1;
	//	Terrain::chunkCount = Terrain::chunksLength * Terrain::chunksLength;
	//}
	//if (terrainRadius != -1) 
	//{
	//	Terrain::terrainRadius = terrainRadius;
	//	Terrain::terrainLength = terrainRadius * 2 + 1;
	//	Terrain::terrainCount = Terrain::terrainLength * Terrain::terrainLength;
	//}

	//Terrain::worldSampleDistance *= Terrain::terrainScale;

	CreateTextures();
	CreateShaders();
	GenerateHeightMap(0);
	GenerateHeightMap(1);
	GenerateHeightMapArray();
	//GenerateOcclusionMap();
	CreateChunks();
}

void Terrain::CreateShaders()
{
	heightMapComputeShader = new Shader("heightmap_compute.glsl");
	Manager::AddShader(heightMapComputeShader);
	//heightMapComputeShader->setFloat("resolution", terrainLod0Resolution);
	//heightMapComputeShader->setFloat("resolutionMult", 1.0 / float(terrainLod0Resolution));
	//heightMapComputeShader->setFloat("mapScale", terrainLod0Size / terrainChunkSize);

	heightMapArrayComputeShader = new Shader("heightmapArray_compute.glsl");
	Manager::AddShader(heightMapArrayComputeShader);
	heightMapArrayComputeShader->setFloat("resolution", terrainChunkResolution);
	heightMapArrayComputeShader->setFloat("resolutionMult", 1.0 / float(terrainChunkResolution));
	heightMapArrayComputeShader->setInt("chunksRadius", chunkRadius);

	occlusionMapComputeShader = new Shader("occlusion_compute.glsl");
	Manager::AddShader(occlusionMapComputeShader);
	occlusionMapComputeShader->setInt(heightMapLod0Texture->Name().c_str(), 0);
	occlusionMapComputeShader->setInt(heightMapLod1Texture->Name().c_str(), 1);
	//occlusionMapComputeShader->setInt("heightMapLod1", 1);
	occlusionMapComputeShader->setInt("heightMapArray", 2);
	occlusionMapComputeShader->setFloat("resolution", terrainOcclusionResolution);
	occlusionMapComputeShader->setFloat("resolutionMult", 1.0 / float(terrainOcclusionResolution));

	terrainShader = new Shader("terrain_vertex.glsl", "tesselation_control.glsl", "tesselation_evaluation.glsl", 
		"terrain_fragment.glsl");
	Manager::AddShader(terrainShader);
	terrainShader->setInt(heightMapLod0Texture->Name().c_str(), heightMapLod0Texture->Index());
	terrainShader->setInt(heightMapLod1Texture->Name().c_str(), heightMapLod1Texture->Index());
	terrainShader->setInt("heightMapArray", 2);
	//terrainShader->setInt("occlusionMap", 3);
}

void Terrain::CreateTextures()
{
	heightMapLod0Texture = new Texture("heightMapLod0", 0, GL_TEXTURE0, terrainLod0Resolution, terrainLod0Resolution, GL_R16_SNORM);
	heightMapLod0Texture->CreateTexture();
	Manager::AddTexture(heightMapLod0Texture);

	heightMapLod1Texture = new Texture("heightMapLod1", 1, GL_TEXTURE1, terrainLod1Resolution, terrainLod1Resolution, GL_R16_SNORM);
	heightMapLod1Texture->CreateTexture();
	Manager::AddTexture(heightMapLod1Texture);

	//glGenTextures(1, &heightMapLod0Texture);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, heightMapLod0Texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R16_SNORM, terrainLod0Resolution, terrainLod0Resolution, 0, GL_RED, GL_FLOAT, NULL);
	//glActiveTexture(0);

	//glGenTextures(1, &heightMapLod1Texture);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, heightMapLod1Texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R16_SNORM, terrainLod1Resolution, terrainLod1Resolution, 0, GL_RED, GL_FLOAT, NULL);
	//glActiveTexture(0);

	//glBindImageTexture(0, heightMapLod0Texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16_SNORM);

	//glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &heightMapArrayTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D_ARRAY, heightMapArrayTexture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R16_SNORM, terrainChunkResolution, terrainChunkResolution,
		chunkCount, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(1, heightMapArrayTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R16_SNORM);

	glGenTextures(1, &occlusionMapTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, occlusionMapTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8_SNORM, terrainOcclusionResolution, terrainOcclusionResolution, 0, GL_RED, GL_FLOAT, NULL);
	
	glBindImageTexture(2, occlusionMapTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8_SNORM);

	//glActiveTexture(0);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Terrain::CreateChunks()
{
	Shape *plane = new Shape(PLANE, 100);
	Manager::AddShape(plane);
	Shape *lodPlane = new Shape(PLANE, 10);
	Manager::AddShape(lodPlane);
	plane->Scale(glm::vec3(terrainChunkSize));
	lodPlane->Scale(glm::vec3(terrainChunkSize));

	terrainMesh = new Mesh(plane, terrainShader);
	Manager::AddMesh(terrainMesh);
	terrainLodMesh = new Mesh(lodPlane, terrainShader);
	Manager::AddMesh(terrainLodMesh);

	terrainChunks = new Object **[terrainLength];
	for (int x = -terrainRadius; x <= terrainRadius; x++)
	{
		int xi = x + terrainRadius;
		terrainChunks[xi] = new Object *[terrainLength];
		for (int y = -terrainRadius; y <= terrainRadius; y++)
		{
			int yi = y + terrainRadius;
			terrainChunks[xi][yi] = new Object(terrainLodMesh);
			terrainChunks[xi][yi]->Paint(glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));
			terrainChunks[xi][yi]->Move(glm::vec3(x, 0, y) * terrainChunkSize);
			terrainChunks[xi][yi]->GetMesh()->GetShader()->setMatrix4("model", terrainChunks[xi][yi]->Translation());
			terrainChunks[xi][yi]->GetMesh()->GetShader()->setFloat4("color", terrainChunks[xi][yi]->Color());
			Manager::AddObject(terrainChunks[xi][yi]);
		}
	}
}

void Terrain::GenerateHeightMap(int lod)
{
	if (lod == 0)
	{
		//glActiveTexture(GL_TEXTURE0);
		Shader::setFloat2Global("terrainOffsetLod0", offsetLod0);
		heightMapComputeShader->useShader();
		heightMapLod0Texture->BindImage(0);
		//glBindImageTexture(0, heightMapLod0Texture->TextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16_SNORM);
		heightMapComputeShader->setFloat("resolution", terrainLod0Resolution);
		heightMapComputeShader->setFloat("resolutionMult", 1.0 / float(terrainLod0Resolution));
		heightMapComputeShader->setFloat("mapScale", terrainLod0Size / terrainChunkSize);
		heightMapComputeShader->setFloat2("offset", offsetLod0 / terrainLod0Size);
		heightMapComputeShader->setFloat2("worldOffset", terrainOffset / terrainChunkSize);
		glDispatchCompute(terrainLod0Resolution / 4, terrainLod0Resolution / 4, 1);
	}
	else if (lod == 1)
	{
		//glActiveTexture(GL_TEXTURE1);
		Shader::setFloat2Global("terrainOffsetLod1", offsetLod1);
		heightMapComputeShader->useShader();
		heightMapLod1Texture->BindImage(0);
		//glBindImageTexture(0, heightMapLod1Texture->TextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16_SNORM);
		heightMapComputeShader->setFloat("resolution", terrainLod1Resolution);
		heightMapComputeShader->setFloat("resolutionMult", 1.0 / float(terrainLod1Resolution));
		heightMapComputeShader->setFloat("mapScale", terrainLod1Size / terrainChunkSize);
		heightMapComputeShader->setFloat2("offset", offsetLod1 / terrainLod1Size);
		heightMapComputeShader->setFloat2("worldOffset", terrainOffset / terrainChunkSize);
		glDispatchCompute(terrainLod1Resolution / 4, terrainLod1Resolution / 4, 1);
	}
}

void Terrain::GenerateHeightMapArray()
{
	heightMapArrayComputeShader->useShader();
	glDispatchCompute(terrainChunkResolution / 4, terrainChunkResolution / 4, 1);
}

void Terrain::GenerateOcclusionMap()
{
	occlusionMapComputeShader->useShader();
	glDispatchCompute(terrainOcclusionResolution / 4, terrainOcclusionResolution / 4, 1);
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
			clipSpace.y < -tolerance || clipSpace.y > 1.0f + tolerance ||
			clipSpace.z <= -(tolerance * 0.5f));
}

bool ChunkInView(const glm::vec3 &position, float tolerance, const glm::mat4 &projection, const glm::mat4 &view, bool main = true)
{
	//position.y += Terrain::terrainHeight * 0.5;
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
			float distance = glm::clamp(glm::distance(glm::vec2(x, z), glm::vec2(sx, sz)), 0.0f, float(terrainLength));
			
			if (distance < 0.75)
			{
				terrainChunks[ix][iz]->GetPosition().y = 0;
				terrainMesh->UseMesh();
				terrainShader->setMatrix4("model", terrainChunks[ix][iz]->Translation());
				glDrawElements(GL_PATCHES, terrainMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
			}
			else if (ChunkInView(terrainChunks[ix][iz]->Position(), 0, projection, view))
			{
				terrainChunks[ix][iz]->GetPosition().y = -10;
				terrainLodMesh->UseMesh();
				terrainShader->setMatrix4("model", terrainChunks[ix][iz]->Translation());
				glDrawElements(GL_PATCHES, terrainLodMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
				amount++;
			}
		}
	}
}

void Terrain::NewFrame()
{
	RenderTerrain();

	if (Input::GetKey(GLFW_KEY_G).pressed)
	{
		seed = glm::vec2(int(Utilities::Random11() * 100), int(Utilities::Random11() * 100));
		heightMapComputeShader->setFloat2("seed", seed);
		heightMapArrayComputeShader->setFloat2("seed", seed);
		GenerateHeightMap(0);
		GenerateHeightMap(1);
		GenerateHeightMapArray();
	}

	
	float xw = floor(Manager::camera.Position().x);
	float zw = floor(Manager::camera.Position().z);
	float x0 = xw - offsetLod0.x;
	float z0 = zw - offsetLod0.y;
	float x1 = xw - offsetLod1.x;
	float z1 = zw - offsetLod1.y;
	

	if (abs(xw) > terrainChunkSize * 0.5)
	{
		terrainOffset += glm::vec2(xw, 0);
		Shader::setFloat2Global("terrainWorldOffset", terrainOffset);
		offsetLod0 = glm::vec2(0);
		offsetLod1 = glm::vec2(0);
		GenerateHeightMap(0);
		GenerateHeightMap(1);
		Manager::camera.Move(-glm::vec3(xw, 0, 0));
	}
	else if (abs(zw) > terrainChunkSize * 0.5)
	{
		terrainOffset += glm::vec2(0, zw);
		Shader::setFloat2Global("terrainWorldOffset", terrainOffset);
		offsetLod0 = glm::vec2(0);
		offsetLod1 = glm::vec2(0);
		GenerateHeightMap(0);
		GenerateHeightMap(1);
		Manager::camera.Move(-glm::vec3(0, 0, zw));
	}
	else if (abs(x0) > terrainLod0Size * 0.25)
	{
		offsetLod0 += glm::vec2(x0, 0);
		GenerateHeightMap(0);
	}
	else if (abs(x1) > terrainLod1Size * 0.25)
	{
		offsetLod1 += glm::vec2(x1, 0);
		GenerateHeightMap(1);
	}
	else if (abs(z0) > terrainLod0Size * 0.25)
	{
		offsetLod0 += glm::vec2(0, z0);
		GenerateHeightMap(0);
	}
	else if (abs(z1) > terrainLod1Size * 0.25)
	{
		offsetLod1 += glm::vec2(0, z1);
		GenerateHeightMap(1);
	}

	
}