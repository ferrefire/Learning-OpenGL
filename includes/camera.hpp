#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    private:
        glm::vec3 position = glm::vec3(0.0f, 9.0f, 9.0f);
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 angles = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 side = glm::vec3(1.0f, 0.0f, 0.0f);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

    public:
        Camera();
		~Camera();

        float near = 0.1f;
        float far = 1000.0f;
        float FOV = 45.0f;
        float speed = 1.0f;

		void Move(const glm::vec3 &amount);
		void SetPosition(const glm::vec3 &newPosition);
		void Rotate(const glm::vec3 &degrees);
		void SetRotation(const glm::vec3 &newRotation);
		void UpdateProjection();

        const glm::vec3 &Position();
        const glm::vec3 &Front();
        const glm::vec3 &Up();
        const glm::vec3 &Side();
        const glm::vec3 &Angles();

        const glm::mat4 &View();
        const glm::mat4 &Projection();
};

#endif