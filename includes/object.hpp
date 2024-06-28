#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.hpp"

class Object
{
    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::vec4 color;

        //glm::mat4 translation;

        Mesh * mesh;

    public:
        Object(Mesh * mesh);

        glm::vec3 Position();
        glm::vec3 Rotation();
        glm::vec3 Scale();
        glm::vec4 Color();

        glm::mat4 Translation();
        Mesh * GetMesh();

        void Move(glm::vec3 amount);
        void Rotate(glm::vec3 amount);
        void Resize(glm::vec3 amount);
        void Paint(glm::vec4 amount);
};

#endif