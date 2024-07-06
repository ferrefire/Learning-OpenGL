#include "shape.hpp"

Shape::Shape()
{

}

Shape::Shape(int preset)
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
    else if (preset == PLANE)
    {
        name = std::string("plane");
        vertexOnly = true;

        int sideVertCount = 100;
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

        /*for (int i = 2; i < 4; i++)
        {
            for (int j = 0; j < i; j++)
            {
                vertices.push_back(glm::vec3(j, 0.0f, i));
            }
            for (int j = 0; j < i; j++)
            {
                vertices.push_back(glm::vec3(i, 0.0f, j));
            }
            vertices.push_back(glm::vec3(i, 0.0f, i));
        }

        //----------------------------------- 1
        vertices.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // 0 0 i
        vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // i 0 0
        vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // 0 0 0
        vertices.push_back(glm::vec3(1.0f, 0.0f, 1.0f)); // i 0 i
        //-----------------------------------

        //----------------------------------- 2
        vertices.push_back(glm::vec3(0.0f, 0.0f, 2.0f)); // 0 0 i
        vertices.push_back(glm::vec3(1.0f, 0.0f, 2.0f)); // 1 0 i

        vertices.push_back(glm::vec3(2.0f, 0.0f, 0.0f)); // i 0 0
        vertices.push_back(glm::vec3(2.0f, 0.0f, 1.0f)); // i 0 1

        vertices.push_back(glm::vec3(2.0f, 0.0f, 2.0f)); // i 0 i
        //-----------------------------------

        //----------------------------------- 3
        vertices.push_back(glm::vec3(0.0f, 0.0f, 3.0f)); // 0 0 i
        vertices.push_back(glm::vec3(1.0f, 0.0f, 3.0f)); // 1 0 i

        vertices.push_back(glm::vec3(2.0f, 0.0f, 3.0f)); // 2 0 i

        vertices.push_back(glm::vec3(3.0f, 0.0f, 0.0f)); // i 0 0
        vertices.push_back(glm::vec3(3.0f, 0.0f, 1.0f)); // i 0 1

        vertices.push_back(glm::vec3(3.0f, 0.0f, 2.0f)); // i 0 2

        vertices.push_back(glm::vec3(3.0f, 0.0f, 3.0f)); // i 0 i
        //-----------------------------------

        //----------------------------------- 1
        indices.push_back(0); // 1
        indices.push_back(1); // 2
        indices.push_back(2); // 3
        indices.push_back(0); // 1
        indices.push_back(3); // 4
        indices.push_back(1); // 2
        //-----------------------------------

        //----------------------------------- 2
        indices.push_back(4); // 1 m+1
        indices.push_back(3); // 2 
        indices.push_back(0); // 3 pc-4
        indices.push_back(4); // 1 pc-1
        indices.push_back(5); // 4 pc-1
        indices.push_back(3); // 2 pc-3

        indices.push_back(3); // 1 p4
        indices.push_back(6); // 2 m+1
        indices.push_back(1); // 3 p2
        indices.push_back(3); // 1 
        indices.push_back(7); // 4 m+2
        indices.push_back(6); // 2

        indices.push_back(5); // 1 
        indices.push_back(7); // 2
        indices.push_back(3); // 3
        indices.push_back(5); // 1
        indices.push_back(8); // 4
        indices.push_back(7); // 2
        //-----------------------------------

        //----------------------------------- 3
        indices.push_back(9); // 1 m+1
        indices.push_back(5); // 2 pn-4
        indices.push_back(4); // 3 1-1
        indices.push_back(9); // 1
        indices.push_back(10);// 4 m+2
        indices.push_back(5); // 2

        indices.push_back(10);// 1 p4
        indices.push_back(8); // 2 pm
        indices.push_back(5); // 3 
        indices.push_back(10);// 1
        indices.push_back(11);// 4
        indices.push_back(8); // 2

        indices.push_back(7); // 1 p4
        indices.push_back(12);// 2 m+1
        indices.push_back(6); // 3 p2
        indices.push_back(7); // 1
        indices.push_back(13);// 4 m+2
        indices.push_back(12);// 2

        indices.push_back(8); // 1 p4
        indices.push_back(13);// 2 m
        indices.push_back(7); // 3 
        indices.push_back(8); // 1
        indices.push_back(14);// 4
        indices.push_back(13);// 2

        indices.push_back(11);// 1
        indices.push_back(14);// 2
        indices.push_back(8); // 3
        indices.push_back(11);// 1
        indices.push_back(15);// 4
        indices.push_back(14);// 2
        //-----------------------------------

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 1.0f));
        uvs.push_back(glm::vec2(1.0f, 1.0f));

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 0.0f));

        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));*/

        vertexCount = vertices.size();
        indiceCount = indices.size();
    }
    else if (preset == BLADE)
    {
        name = std::string("blade");
        vertexOnly = true;

        const float BLADE_WIDTH = 0.05f;
        const float BLADE_HEIGHT = 0.25f;

        vertices.push_back(glm::vec3(-BLADE_WIDTH, BLADE_HEIGHT, 0.0f));
        vertices.push_back(glm::vec3(BLADE_WIDTH, 0, 0.0f));
        vertices.push_back(glm::vec3(-BLADE_WIDTH, 0, 0.0f));
        vertices.push_back(glm::vec3(BLADE_WIDTH, BLADE_HEIGHT, 0.0f));
        vertices.push_back(glm::vec3(-BLADE_WIDTH, BLADE_HEIGHT * 2, 0.0f));
        vertices.push_back(glm::vec3(BLADE_WIDTH, BLADE_HEIGHT * 2, 0.0f));
        vertices.push_back(glm::vec3(-BLADE_WIDTH, BLADE_HEIGHT * 3, 0.0f));
        vertices.push_back(glm::vec3(BLADE_WIDTH, BLADE_HEIGHT * 3, 0.0f));
        vertices.push_back(glm::vec3(0.0f, BLADE_HEIGHT * 4, 0.0f));

        //uvs.push_back(glm::vec2(0.0f, 0.0f));
        //uvs.push_back(glm::vec2(1.0f, 0.0f));
        //uvs.push_back(glm::vec2(0.0f, 1.0f));
        //uvs.push_back(glm::vec2(1.0f, 1.0f));

        //normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        //normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        //normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        //normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(1);

        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(0);

        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(3);

        indices.push_back(6);
        indices.push_back(5);
        indices.push_back(4);

        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(5);

        indices.push_back(8);
        indices.push_back(7);
        indices.push_back(6);

        vertexCount = vertices.size();
        indiceCount = indices.size();
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