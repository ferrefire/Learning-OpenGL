#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "glad.h"
#include "mesh.hpp"
#include "object.hpp"

void renderMesh(Mesh mesh);
void renderMeshInstanced(Mesh mesh, int count);
void renderObject(Object * object);

#endif