#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <string>

class Shape
{
    public:
        std::string name;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Shape(std::string name, int vertices_size, float vertices[], int indices_size, unsigned int indices[]);
};

#endif