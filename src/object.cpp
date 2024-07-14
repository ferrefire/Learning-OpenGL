#include "object.hpp"

Object::Object(Mesh *mesh)
{
    this->mesh = mesh;

    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    color = glm::vec4(1.0f);
}

glm::vec3 Object::Position()
{
    return (position);
}

glm::vec3 Object::Rotation()
{
    return (rotation);
}

glm::vec3 Object::Scale()
{
    return (scale);
}

glm::vec4 Object::Color()
{
    return (color);
}

glm::vec3 &Object::GetPosition()
{
	return (position);
}

glm::mat4 Object::Translation()
{
    glm::mat4 translation = glm::mat4(1.0f);

    translation = glm::translate(translation, position);
    translation = glm::scale(translation, scale);

    translation = glm::rotate(translation, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    translation = glm::rotate(translation, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    translation = glm::rotate(translation, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return (translation);
}

Mesh *Object::GetMesh()
{
    return (mesh);
}

void Object::Move(glm::vec3 amount)
{
    position += amount;
}

void Object::Rotate(glm::vec3 amount)
{
    rotation += amount;
}

void Object::Resize(glm::vec3 amount)
{
    scale = amount;
}

void Object::Paint(glm::vec4 amount)
{
    color = amount;
}