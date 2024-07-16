#ifndef MESH_HPP
#define MESH_HPP

#include "glad.h"
#include <glm/glm.hpp>
#include <tuple>
#include <vector>
#include <iostream>
#include "shape.hpp"
#include "shader.hpp"

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
		static unsigned int currentActiveVAO;

        Mesh(Shape *shape, Shader *shader);
		~Mesh();

        unsigned int GetVAO();
        unsigned int GetVBO();
        unsigned int GetEBO();

        Shader *GetShader();
        Shape *GetShape();

		void UseMesh();
};

#endif
