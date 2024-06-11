#include "rendering.hpp"

void renderMesh(Mesh mesh)
{
    glUseProgram(mesh.shader->ID);
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.elements, GL_UNSIGNED_INT, 0);
}