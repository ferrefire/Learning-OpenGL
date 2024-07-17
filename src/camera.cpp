#include "camera.hpp"
#include "time.hpp"
#include "utilities.hpp"
#include "input.hpp"
#include "shader.hpp"

Camera::Camera()
{
    far = 25000;
    UpdateProjection();
	position.x = 0;
	position.z = 0;
	position.y = 1000;
	Move(glm::vec3(0.0f));
    Rotate(glm::vec3(25.0f, -90.0f, 0.0f));
}

Camera::~Camera()
{
	
}

void Camera::UpdateProjection()
{
	projection = glm::perspective(glm::radians(FOV), Input::width / Input::height, near, far);
	Shader::setFloatGlobal("width", Input::width);
	Shader::setFloatGlobal("height", Input::height);
	Shader::setMatrix4Global("projection", projection);
}

void Camera::Move(const glm::vec3 &amount)
{
    position += amount;

    view = glm::lookAt(position, position + front, up);
}

void Camera::Rotate(const glm::vec3 &degrees)
{
    this->angles = degrees;

    if (angles.x > 89.0f)
        angles.x = 89.0f;
    if (angles.x < -89.0f)
        angles.x = -89.0f;

    direction.x = cos(glm::radians(angles.y)) * cos(glm::radians(angles.x));
    direction.y = sin(glm::radians(angles.x));
    direction.z = sin(glm::radians(angles.y)) * cos(glm::radians(angles.x));

    front = glm::normalize(direction);
    side = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(side, front));

    view = glm::lookAt(position, position + front, up);
}

const glm::vec3 &Camera::Position()
{
    return (position);
}

const glm::vec3 &Camera::Front()
{
    return (front);
}

const glm::vec3 &Camera::Up()
{
    return (up);
}

const glm::vec3 &Camera::Side()
{
    return (side);
}

const glm::vec3 &Camera::Angles()
{
    return (angles);
}

const glm::mat4 &Camera::View()
{
    return (view);
}

const glm::mat4 &Camera::Projection()
{
    return (projection);
}