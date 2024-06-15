#include "mesh.hpp"

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, Shader *shader)
{
	this->shader = shader;

	elements = indices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	// glEnableVertexAttribArray(2);
}

Mesh::Mesh(Shape shape, Shader *shader)
{
	//Mesh(shape.vertices, shape.indices, shader);

	this->shader = shader;

	elements = shape.indices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices.size(), shape.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices.size(), shape.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

Mesh::Mesh(std::vector<float> vertices, std::vector<Triangle> triangles, Shader *shader)
{
	int i = 0;
	// std::vector<float> vertices;
	std::vector<unsigned int> indices;

	while (i < triangles.size())
	{
		for (int j = 0; j < 3; j++)
		{
			// vertices.push_back(triangles[i].GetVert(j).x);
			// vertices.push_back(triangles[i].GetVert(j).y);
			// vertices.push_back(triangles[i].GetVert(j).z);

			indices.push_back(triangles[i].GetInd(j));
		}
		i++;
	}

	Mesh(vertices, indices, shader);
}