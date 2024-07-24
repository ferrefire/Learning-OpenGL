#include "shape.hpp"
#include <iostream>
#include "utilities.hpp"

Shape::Shape()
{

}

Shape::~Shape()
{
	
}

Shape::Shape(int preset, int resolution)
{
    if (preset == TRIANGLE)
    {
        name = std::string("triangle");

        vertices.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
        vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
        vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));

        uvs.push_back(glm::vec2(0.5f, 1.0f));
        uvs.push_back(glm::vec2(1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));

        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

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
	else if (preset == SCREEN_QUAD)
	{
		vertexOnly = true;

		name = std::string("screen quad");

		vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
		vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, 0.0f));

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
    else if (preset == CYLINDER)
    {
        Shape plane(PLANE, resolution);
		plane.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		vertexOnly = true;
		Join(plane);
		vertexOnly = false;

		RecalculateUV();

		std::vector<int> startVertices;
		std::vector<int> endVertices;

		int index = 0;

		for (glm::vec3 &vert : vertices)
		{
			float x = round((vert.x + 0.5) * (resolution - 1));
			float y = round((vert.y + 0.5) * (resolution - 1));

			if (x == 0) startVertices.push_back(index);
			else if (x == (resolution - 1)) endVertices.push_back(index);

			glm::vec3 newVert = glm::normalize(Utilities::RotateNewVec3(glm::vec3(0, 0, -1), (360.0 / resolution) * -x, glm::vec3(0, 1, 0)));

			float trunkWidth = 2.0 - (y / (resolution - 1));

			vert.x = newVert.x * trunkWidth;
			vert.z = newVert.z * trunkWidth;

			vert += (glm::vec3(Utilities::Random11(), 0, Utilities::Random11()) * 0.2f);

			index++;
		}

		for (int i = 0; i < startVertices.size() - 1; i++)
		{
			indices.push_back(endVertices[i + 1]);
			indices.push_back(startVertices[i]);
			indices.push_back(endVertices[i]);

			indices.push_back(endVertices[i + 1]);
			indices.push_back(startVertices[i + 1]);
			indices.push_back(startVertices[i]);
		}

		RecalculateNormals();

        Translate(glm::vec3(0, 0.5, 0));
        Scale(glm::vec3(2, 50, 2));
    }
    else if (preset == PLANE)
    {
        name = std::string("plane");
        vertexOnly = true;

        int sideVertCount = resolution;
        float halfLength = 0.5f;

        for (int i = 0, x = 0; x <= sideVertCount; ++x)
        {
            for (int z = 0; z <= sideVertCount; ++z, ++i)
            {
                vertices.push_back(glm::vec3(((float)x / sideVertCount) - halfLength, 0, ((float)z / sideVertCount) - halfLength));
            }
        }

        for (int ti = 0, vi = 0, x = 0; x < sideVertCount; ++vi, ++x)
        {
            for (int z = 0; z < sideVertCount; ti += 6, ++vi, ++z)
            {
                indices.push_back(vi);
                indices.push_back(vi + 1);
                indices.push_back(vi + sideVertCount + 2);
                indices.push_back(vi);
                indices.push_back(vi + sideVertCount + 2);
                indices.push_back(vi + sideVertCount + 1);
            }
        }

        vertexCount = vertices.size();
        indiceCount = indices.size();

		//std::cout << vertexCount << std::endl;
    }
    else if (preset == BLADE)
    {
        name = std::string("blade");
        vertexOnly = true;

		int layer = 1;
		int subLayers = resolution;
		const float BLADE_WIDTH = 0.05f;
		const float BLADE_HEIGHT = 1.0 / (subLayers + 2);

		vertices.push_back(glm::vec3(-BLADE_WIDTH, layer * BLADE_HEIGHT, 0.0f));
        vertices.push_back(glm::vec3(BLADE_WIDTH, 0, 0.0f));
        vertices.push_back(glm::vec3(-BLADE_WIDTH, 0, 0.0f));
        vertices.push_back(glm::vec3(BLADE_WIDTH, layer * BLADE_HEIGHT, 0.0f));

		for (int i = 0; i < subLayers; i++)
		{
			layer++;
			vertices.push_back(glm::vec3(-BLADE_WIDTH, layer * BLADE_HEIGHT, 0.0f));
			vertices.push_back(glm::vec3(BLADE_WIDTH, layer * BLADE_HEIGHT, 0.0f));
		}

		layer++;
        vertices.push_back(glm::vec3(0.0f, layer * BLADE_HEIGHT, 0.0f));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(1);

		int max = 3;
		int top = 0;
		for (int i = 0; i < subLayers; i++)
		{
			indices.push_back(max + 1);
			indices.push_back(max);
			indices.push_back(top);
			indices.push_back(max + 1);
			indices.push_back(max + 2);
			indices.push_back(max);
			top = max + 1;
			max = max + 2;
		}

        indices.push_back(max + 1);
        indices.push_back(max);
        indices.push_back(top);

        vertexCount = vertices.size();
        indiceCount = indices.size();
    }

    RecalculateData();
}

float *Shape::GetData()
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

unsigned int *Shape::GetIndices()
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

void Shape::Rotate(float degrees, const glm::vec3 &axis)
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

void Shape::Translate(const glm::vec3 &translation)
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

void Shape::Scale(const glm::vec3 &scale)
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

        if (!vertexOnly)
        {
            data.push_back(uvs[i].x);
            data.push_back(uvs[i].y);

            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
	}

    dataCount = data.size();
}

void Shape::Join(Shape &joinShape)
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
        if (!joinShape.vertexOnly)
		{
			uvs.push_back(addUvs[i]);
			normals.push_back(addNormals[i]);
		}
	}
    
    addSize = addIndices.size();
    for (int i = 0; i < addSize; i++)
    {
        indices.push_back(addIndices[i] + vertOffset);
    }

    RecalculateData();
}

glm::vec4 Shape::GetBounds()
{
	glm::vec4 bounds;

	for (const glm::vec3 &vert : vertices)
	{
		if (vert.x < bounds.x) bounds.x = vert.x;
		if (vert.x > bounds.y) bounds.y = vert.x;
		if (vert.y < bounds.z) bounds.z = vert.y;
		if (vert.y > bounds.w) bounds.w = vert.y;
	}

	return bounds;
}

void Shape::RecalculateUV()
{
	uvs.clear();

	glm::vec4 bounds = GetBounds();

	for (const glm::vec3 &vert : vertices)
	{
		float x = (vert.x - bounds.x) / (bounds.y - bounds.x);
		float y = (vert.y - bounds.z) / (bounds.w - bounds.z);
		uvs.push_back(glm::vec2(x, y));
	}
}

void Shape::RecalculateNormals()
{
	normals.clear();

	for (const glm::vec3 &vert : vertices)
	{
		normals.push_back(glm::normalize(glm::vec3(vert.x, 0, vert.z)));
	}
}