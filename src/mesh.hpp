#ifndef MESH_HPP
#define MESH_HPP

#include "shape.hpp"
#include "shader.hpp"
#include "glad.h"
#include <glm/glm.hpp>
#include <tuple>
#include <vector>
#include "triangle.hpp"

class Mesh
{
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int elements;

        Shader *shader;
        Shape *shape;
    public:
        Mesh(Shape *shape, Shader *shader);

        unsigned int GetVAO();
        unsigned int GetVBO();
        unsigned int GetEBO();

        Shader * GetShader();
        Shape * GetShape();
};

#endif
