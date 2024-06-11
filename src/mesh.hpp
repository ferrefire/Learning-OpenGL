#ifndef MESH_HPP
#define MESH_HPP

#include "shape.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
    private:

    public:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int elements;
        unsigned int shader;
        Mesh(Shape shape, unsigned int shader);
        //~Mesh();
};

#endif
