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
    }

    size = objects.size();
    for (int i = 0; i < size; i++)
    {
        objects[i]->GetMesh()->GetShader()->setMatrix4("model", objects[i]->Translation());
        objects[i]->GetMesh()->GetShader()->setFloat4("color", objects[i]->Color());
        renderObject(objects[i]);
    }

    size = instanceBatches.size();
    for (int i = 0; i < size; i++)
    {
        instanceBatches[i].mesh->GetShader()->setMatrix4("model", glm::mat4(1.0f));
        //instanceBatches[i].mesh->GetShader()->setInt("instanceCount", instanceBatches[i].count);
        //instanceBatches[i].mesh->GetShader()->setInt("instanceCountSqrt", sqrt(instanceBatches[i].count));
        renderMeshInstanced(*instanceBatches[i].mesh, instanceBatches[i].count);
    }
}