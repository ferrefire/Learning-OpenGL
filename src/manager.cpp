#include "manager.hpp"
#include "rendering.hpp"
#include "time.hpp"
#include "terrain.hpp"
#include "input.hpp"

void Manager::AddObject(Object *object)
{
    objects.push_back(object);
	object->GetMesh()->GetShader()->setMatrix4("model", object->Translation());
	object->GetMesh()->GetShader()->setFloat4("color", object->Color());
}

void Manager::AddInstanceBatch(InstanceBatch instanceBatch)
{
    instanceBatches.push_back(instanceBatch);
}

void Manager::AddInstanceBatch(Mesh *mesh, int count)
{
    InstanceBatch newBatch;
	mesh->GetShader()->setMatrix4("model", glm::mat4(1.0f));
	newBatch.mesh = mesh;
    newBatch.count = count;
    instanceBatches.push_back(newBatch);
}

void Manager::SetShaderVariables(Shader *shader)
{
	shader->setFloat("time", glfwGetTime());
	shader->setMatrix4("view", camera.View());
	shader->setFloat3("viewPosition", camera.Position());
	shader->setFloat3("viewDirection", camera.Front());
	shader->setMatrix4("projection", camera.Projection());
    shader->setFloat3("lightPosition", 25000.0f, 25000.0f, 50000.0f);
	shader->setFloat3("lightDirection", glm::normalize(Manager::sunDirection));
	shader->setFloat3("lightColor", glm::vec3(1, 1, 1));
	shader->setFloat("lightStrength", 1);
	shader->setFloat("near", camera.near);
	shader->setFloat("nearMult", 1.0 / camera.near);
	shader->setFloat("far", camera.far);
	shader->setFloat("farMult", 1.0 / camera.far);
	shader->setFloat("width", Input::width);
	shader->setFloat("height", Input::height);
	shader->setInt("noiseLayers", Terrain::terrainLayers);
	shader->setFloat("noiseScale", Terrain::terrainScale);
    shader->setFloat("heightMapHeight", Terrain::terrainHeight);
	shader->setFloat("terrainSize", Terrain::terrainSize);
	shader->setFloat("terrainSizeMult", 1.0 / Terrain::terrainSize);
	shader->setFloat("terrainChunkSize", Terrain::terrainChunkSize);
	shader->setFloat("terrainChunkSizeMult", 1.0 / Terrain::terrainChunkSize);
	shader->setFloat("terrainHeight", Terrain::terrainHeight);
	shader->setFloat("terrainHeightMult", 1.0 / Terrain::terrainHeight);
	shader->setFloat("terrainOccludeSize", Terrain::terrainOccludeSize);
	shader->setFloat("terrainOccludeSizeMult", 1.0 / Terrain::terrainOccludeSize);
	shader->setInt("chunksLength", Terrain::chunksLength);
	shader->setFloat("chunksLengthMult", 1.0 / Terrain::chunksLength);
	shader->setFloat("worldSampleDistance", Terrain::worldSampleDistance);
	shader->setFloat("worldSampleDistanceMult", (Terrain::worldSampleDistance * 2) / (Terrain::terrainChunkSize * Terrain::worldSampleDistance));
	shader->setFloat("noiseSampleDistance", 0.0003);
	shader->setFloat("noiseSampleDistanceMult", 1.0 / (0.0003));
	shader->setFloat2("offset", 0.0, 0.0);
	shader->setFloat2("seed", 0.0, 0.0);
}

void Manager::AddShader(Shader *shader)
{
    shaders.push_back(shader);
	SetShaderVariables(shader);
}

void Manager::SetShaderFrameVariables()
{
	int size = shaders.size();
	for (int i = 0; i < size; i++)
	{
		shaders[i]->setFloat("time", glfwGetTime());
		shaders[i]->setMatrix4("view", camera.View());
		shaders[i]->setFloat3("viewPosition", camera.Position());
		shaders[i]->setFloat3("viewDirection", camera.Front());
	}
}

void Manager::NewFrame()
{
    int size = objects.size();
    for (int i = 0; i < size; i++)
    {
        objects[i]->GetMesh()->GetShader()->setMatrix4("model", objects[i]->Translation());
        renderObject(objects[i]);
    }

	EnableCulling(false);

    size = instanceBatches.size();
    for (int i = 0; i < size; i++)
    {
        renderMeshInstanced(*instanceBatches[i].mesh, instanceBatches[i].count);
    }
}

void Manager::EnableCulling(bool mode)
{
	if (Manager::cullingActive == mode) return ;

	Manager::cullingActive = mode;
	if (mode) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void Manager::Close()
{
	glfwSetWindowShouldClose(Manager::window, true);
}

void Manager::Quit(int exitCode)
{
	glfwTerminate();
	exit(exitCode);
}