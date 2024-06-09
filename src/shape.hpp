#include <vector>
#include <string>

class Shape
{
    public:
        std::string name;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Shape(std::string name, int vertices_size, float vertices[], int indices_size, unsigned int indices[])
        {
            this->name = name;
            for (int i = 0; i < vertices_size; i++) this->vertices.push_back(vertices[i]);
            for (int i = 0; i < indices_size; i++) this->indices.push_back(indices[i]);
        }
};