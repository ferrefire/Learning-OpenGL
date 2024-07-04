#include "manager.hpp"

void Manager::AddObject(Object * object)
{
    objects.push_back(object);
}

void Manager::AddInstanceBatch(InstanceBatch instanceBatch)
{
    instanceBatches.push_back(instanceBatch);
}

void Manager::AddInstanceBatch(Mesh *mesh, int count)
{
    InstanceBatch newBatch;
    newBatch.mesh = mesh;
    newBatch.count = count;
    instanceBatches.push_back(newBatch);
}

void Manager::AddShader(Shader *shader)
{
    shaders.push_back(shader);
}

void Manager::NewFrame()
{
    int size = shaders.size();
    for (int i = 0; i < size; i++)
    {
        shaders[i]->setFloat("time", glfwGetTime());
        shaders[i]->setMatrix4("view", camera.View());
        shaders[i]->setFloat3("viewPosition", camera.Position());
        shaders[i]->setInt("noiseLayers", 8);
        shaders[i]->setFloat("noiseScale", 0.5f);
        shaders[i]->setFloat("noiseHeight", 1000.0f);
		shaders[i]->setFloat("sizeMultiplier", 1.0 / 2048.0);
	}

    glEnable(GL_CULL_FACE);

    size = objects.size();
    for (int i = 0; i < size; i++)
    {
        objects[i]->GetMesh()->GetShader()->setMatrix4("model", objects[i]->Translation());
        objects[i]->GetMesh()->GetShader()->setFloat4("color", objects[i]->Color());
        renderObject(objects[i]);
    }

    glDisable(GL_CULL_FACE);

    size = instanceBatches.size();
    for (int i = 0; i < size; i++)
    {
        instanceBatches[i].mesh->GetShader()->setMatrix4("model", glm::mat4(1.0f));
        renderMeshInstanced(*instanceBatches[i].mesh, instanceBatches[i].count);
    }
}