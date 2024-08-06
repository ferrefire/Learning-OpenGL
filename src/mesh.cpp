#include "mesh.hpp"

Mesh::Mesh(Shape *shape, Shader *shader)
{
	this->shader = shader;
    this->shape = shape;

    elements = shape->IndiceCount();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	currentActiveVAO = VAO;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, shape->DataCount() * sizeof(float), shape->GetData(), GL_STATIC_COPY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    if (shape->useShortIndices)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->IndiceCount() * sizeof(unsigned short), shape->GetIndicesShort(), GL_STATIC_COPY);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->IndiceCount() * sizeof(unsigned int), shape->GetIndices(), GL_STATIC_COPY);
	}

    if (shape->vertexOnly)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }
    else
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

unsigned int Mesh::GetVAO()
{
    return (VAO);
}

unsigned int Mesh::GetVBO()
{
    return (VBO);
}

unsigned int Mesh::GetEBO()
{
    return (EBO);
}

Shader *Mesh::GetShader()
{
    return (shader);
}

Shape *Mesh::GetShape()
{
    return (shape);
}

void Mesh::UseMesh()
{
	if (currentActiveVAO != VAO)
	{
		glBindVertexArray(VAO);
		currentActiveVAO = VAO;
	} 
}