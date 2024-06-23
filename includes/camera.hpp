#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "time.hpp"
#include "utilities.hpp"

class Camera
{
    private:
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 9.0f);
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 angles = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 side = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::mat4 view = glm::mat4(1.0f);

    public:
        Camera();

        float speed = 1.0f;

        void Move(glm::vec3 amount);
        void Rotate(glm::vec3 angles);

        glm::vec3 Position();
        glm::vec3 Front();
        glm::vec3 Up();
        glm::vec3 Side();
        glm::vec3 Angles();
        glm::mat4 View();
};

#endif