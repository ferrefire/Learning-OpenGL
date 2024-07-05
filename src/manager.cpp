#include "manager.hpp"

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
	shader->setFloat("near", camera.near);
	shader->setFloat("nearMult", 1.0 / camera.near);
	shader->setFloat("far", camera.far);
	shader->setFloat("farMult", 1.0 / camera.far);
	shader->setInt("noiseLayers", 8);
	shader->setFloat("noiseScale", 0.5f);
	shader->setFloat("noiseHeight", 2500.0f);
	shader->setFloat("sizeMultiplier", 1.0 / float(heightMapResolution));
	shader->setFloat("stepSizeMult", float(heightMapResolution) * 0.5);
}

void Manager::AddShader(Shader *shader)
{
    shaders.push_back(shader);
	SetShaderVariables(shader);
}

void Manager::NewFrame()
{
    int size = shaders.size();
    for (int i = 0; i < size; i++)
    {
		shaders[i]->setFloat("time", glfwGetTime());
		shaders[i]->setMatrix4("view", camera.View());
		shaders[i]->setFloat3("viewPosition", camera.Position());
		shaders[i]->setFloat3("viewDirection", camera.Front());
	}

    glEnable(GL_CULL_FACE);

    size = objects.size();
    for (int i = 0; i < size; i++)
    {      
        renderObject(objects[i]);
    }

    glDisable(GL_CULL_FACE);

    size = instanceBatches.size();
    for (int i = 0; i < size; i++)
    {
        renderMeshInstanced(*instanceBatches[i].mesh, instanceBatches[i].count);
    }
}