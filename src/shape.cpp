#include "shape.hpp"

Shape::Shape()
{

}

Shape::Shape(int preset)
{
    if (preset == TRIANGLE)
    {
        name = std::string("triangle");

        vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
        vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
        vertices.push_back(glm::vec3(0.0f, 0.5f, 0.0f));

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.5f, 1.0f));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        vertexCount = vertices.size();
        indiceCount = indices.size();
    }
    else if (preset == QUAD)
    {
        name = std::string("quad");

        vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
        vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
        vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
        vertices.push_back(glm::vec3(0.5f, 0.5f, 0.0f));

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 1.0f));
        uvs.push_back(glm::vec2(1.0f, 1.0f));

		normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
		normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
		normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
		normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

		indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(1);

        vertexCount = vertices.size();
        indiceCount = indices.size();
    }
    else if (preset == CUBE)
    {
        Shape front(QUAD);
        front.Translate(glm::vec3(0.0f, 0.0f, -0.5f));
        Shape right(QUAD);
        right.Rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        right.Translate(glm::vec3(0.5f, 0.0f, 0.0f));
        Shape left(QUAD);
        left.Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        left.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
        Shape back(QUAD);
		back.Rotate(-180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		back.Translate(glm::vec3(0.0f, 0.0f, 0.5f));
        Shape up(QUAD);
        up.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        up.Translate(glm::vec3(0.0f, 0.5f, 0.0f));
        Shape down(QUAD);
        down.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        down.Translate(glm::vec3(0.0f, -0.5f, 0.0f));

        Join(front);
        Join(right);
        Join(left);
        Join(back);
        Join(up);
        Join(down);
    }

    RecalculateData();
}

Shape::Shape(int preset, float scale)
{
    
}

float * Shape::GetData()
{
    return (data.data());
}

std::vector<glm::vec3> Shape::Vertices()
{
    return (vertices);
}

std::vector<glm::vec2> Shape::Uvs()
{
    return (uvs);
}

std::vector<glm::vec3> Shape::Normals()
{
	return (normals);
}

std::vector<unsigned int> Shape::Indices()
{
    return (indices);
}

unsigned int * Shape::GetIndices()
{
    return (indices.data());
}

unsigned int Shape::VertexCount()
{
    return (vertexCount);
}

unsigned int Shape::IndiceCount()
{
    return (indiceCount);
}

unsigned int Shape::DataCount()
{
    return (dataCount);
}

void Shape::Rotate(float degrees, glm::vec3 axis)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(degrees), axis);
    
    int size = vertices.size();
    for (int i = 0; i < size; i++)
    {
        glm::vec4 ri = glm::vec4(vertices[i], 1.0f);
        vertices[i] = rotation * ri;
    }

	size = normals.size();
	for (int i = 0; i < size; i++)
	{
		glm::vec4 ri = glm::vec4(normals[i], 0.0f); //maybe set 1.0f to 0.0f becaus it is a direction
		normals[i] = rotation * ri;
	}

	RecalculateData();
}

void Shape::Translate(glm::vec3 translation)
{
    glm::mat4 translationMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(translationMatrix, translation);

    int size = vertices.size();
    for (int i = 0; i < size; i++)
    {
        glm::vec4 ri = glm::vec4(vertices[i], 1.0f);
        vertices[i] = translationMatrix * ri;
    }

    RecalculateData();
}

void Shape::Scale(glm::vec3 scale)
{
    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::scale(scaleMatrix, scale);

    int size = vertices.size();
    for (int i = 0; i < size; i++)
    {
        glm::vec4 si = glm::vec4(vertices[i], 1.0f);
        vertices[i] = scaleMatrix * si;
    }

    RecalculateData();
}

void Shape::RecalculateData()
{
    vertexCount = vertices.size();
    indiceCount = indices.size();

    data.clear();

    for (int i = 0; i < vertexCount; i++)
    {
        data.push_back(vertices[i].x);
        data.push_back(vertices[i].y);
        data.push_back(vertices[i].z);

        data.push_back(uvs[i].x);
        data.push_back(uvs[i].y);

		data.push_back(normals[i].x);
		data.push_back(normals[i].y);
		data.push_back(normals[i].z);
	}

    dataCount = data.size();
}

void Shape::Join(Shape joinShape)
{
    std::vector<glm::vec3> addVertices = joinShape.Vertices();
	std::vector<glm::vec2> addUvs = joinShape.Uvs();
	std::vector<glm::vec3> addNormals = joinShape.Normals();
	std::vector<unsigned int> addIndices = joinShape.Indices();
    int vertOffset = vertices.size();
    int addSize = addVertices.size();

    for (int i = 0; i < addSize; i++)
    {
        vertices.push_back(addVertices[i]);
        uvs.push_back(addUvs[i]);
		normals.push_back(addNormals[i]);
	}
    
    addSize = addIndices.size();
    for (int i = 0; i < addSize; i++)
    {
        indices.push_back(addIndices[i] + vertOffset);
    }

    RecalculateData();
}