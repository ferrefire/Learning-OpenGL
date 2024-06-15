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

    public:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int elements;
        Shader *shader;
		Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, Shader *shader);
		Mesh(Shape shape, Shader *shader);
		Mesh(std::vector<float> vertices, std::vector<Triangle> triangles, Shader *shader);
		//~Mesh();
};

#endif
