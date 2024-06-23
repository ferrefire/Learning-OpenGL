#include "camera.hpp"

Camera::Camera()
{
    Move(glm::vec3(0.0f));
    Rotate(glm::vec3(0.0f, -90.0f, 0.0f));
}

void Camera::Move(glm::vec3 amount)
{
    position += amount * speed * Time::deltaTime;

    view = glm::lookAt(position, position + front, up);
}

void Camera::Rotate(glm::vec3 angles)
{
    this->angles = angles;

    if (angles.x > 89.0f)
        angles.x = 89.0f;
    if (angles.x < -89.0f)
        angles.x = -89.0f;

    direction.x = cos(glm::radians(angles.y)) * cos(glm::radians(angles.x));
    direction.y = sin(glm::radians(angles.x));
    direction.z = sin(glm::radians(angles.y)) * cos(glm::radians(angles.x));

    front = glm::normalize(direction);
    //up = glm::ve
    side = glm::normalize(glm::cross(front, up));

    view = glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::Front()
{
    return (front);
}

glm::vec3 Camera::Up()
{
    return (up);
}

glm::vec3 Camera::Side()
{
    return (side);
}

glm::vec3 Camera::Angles()
{
    return (angles);
}

glm::mat4 Camera::View()
{
    return (view);
}