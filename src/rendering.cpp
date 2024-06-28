#include "rendering.hpp"

void renderMesh(Mesh mesh)
{
    glUseProgram(mesh.GetShader()->ID);
    glBindVertexArray(mesh.GetVAO());
    glDrawElements(GL_TRIANGLES, mesh.GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
}

void renderMeshInstanced(Mesh mesh, int count)
{
    glUseProgram(mesh.GetShader()->ID);
    glBindVertexArray(mesh.GetVAO());
    glDrawElementsInstanced(GL_TRIANGLES, mesh.GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0, count);
}

void renderObject(Object * object)
{
    glUseProgram(object->GetMesh()->GetShader()->ID);
    glBindVertexArray(object->GetMesh()->GetVAO());
    glDrawElements(GL_TRIANGLES, object->GetMesh()->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);
}