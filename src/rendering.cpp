#include "rendering.hpp"

void renderMesh(Mesh mesh)
{
    //glUseProgram(mesh.GetShader()->ID);
	mesh.GetShader()->useShader();
	//glBindVertexArray(mesh.GetVAO());
	mesh.UseMesh();
	glDrawElements(GL_TRIANGLES, mesh.GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
}

void renderMeshInstanced(Mesh mesh, int count)
{
    //glUseProgram(mesh.GetShader()->ID);
	mesh.GetShader()->useShader();
	//glBindVertexArray(mesh.GetVAO());
	mesh.UseMesh();
	glDrawElementsInstanced(GL_TRIANGLES, mesh.GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, count);
}

void renderObject(Object * object)
{
    //glUseProgram(object->GetMesh()->GetShader()->ID);
	object->GetMesh()->GetShader()->useShader();
	//glBindVertexArray(object->GetMesh()->GetVAO());
	object->GetMesh()->UseMesh();
	glDrawElements(GL_PATCHES, object->GetMesh()->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
}