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
	createResolution = resolution;

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
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);

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

			if (y == 0) mergeBottomPoints.push_back(index);
			else if (y == (resolution - 1)) mergeTopPoints.push_back(index);

			glm::vec3 newVert = glm::normalize(Utilities::RotateNewVec3(glm::vec3(0, 0, -1), (360.0 / resolution) * -x, glm::vec3(0, 1, 0)));

			vert.x = newVert.x;
			vert.z = newVert.z;

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

		for (int i = 0; i < mergeTopPoints.size(); i++)
		{
			pointMerged.push_back(glm::ivec2(0));
		}

		RecalculateNormals();

		if (resolution > 4)
		{
			centerMergePoint = vertices.size();
			vertices.push_back(TopMergePointsCenter() + glm::vec3(0, 0.05, 0));
			normals.push_back(glm::vec3(0, 1, 0));
			uvs.push_back(glm::vec2(0, 1));
		}

		Translate(glm::vec3(0, 0.5, 0));
        Scale(glm::vec3(3, 50, 3));
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

		//std::cout << resolution << " - " << vertexCount << std::endl;
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

std::vector<glm::vec3> &Shape::GetVertices()
{
	return (vertices);
}

std::vector<glm::vec2> Shape::Uvs()
{
    return (uvs);
}

std::vector<glm::vec2> &Shape::GetUVs()
{
	return (uvs);
}

std::vector<glm::vec3> Shape::Normals()
{
	return (normals);
}

std::vector<glm::vec3> &Shape::GetNormals()
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

unsigned short *Shape::GetIndicesShort()
{
	indicesShort.clear();

	for (const unsigned int &i : indices)
	{
		indicesShort.push_back((unsigned short)i);
	}

	return (indicesShort.data());
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

void Shape::RotateVert(int index, float degrees, const glm::vec3 &axis)
{
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, glm::radians(degrees), axis);

	glm::vec4 ri = glm::vec4(vertices[index], 1.0f);
	vertices[index] = rotation * ri;

	ri = glm::vec4(normals[index], 0.0f); // maybe set 1.0f to 0.0f becaus it is a direction
	normals[index] = rotation * ri;
	//RecalculateData();
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
        indices.push_back(addIndices[i] + (joinOffset ? vertOffset : 0));
    }

    RecalculateData();
}

int Shape::ClosestMergeIndex(glm::vec3 position, bool closest, bool top)
{
	int index = 0;
	int closestIndex = 0;
	float closestDistance = 0;

	if (top) closestDistance = glm::distance(position, vertices[mergeTopPoints[0]]);
	else closestDistance = glm::distance(position, vertices[mergeBottomPoints[0]]);

	for (const int &i : (top ? mergeTopPoints : mergeBottomPoints))
	{
		float currentDistance = glm::distance(position, vertices[i]);
		if (closest)
		{
			if (currentDistance < closestDistance)
			{
				closestDistance = currentDistance;
				closestIndex = index;
			}
		}
		else
		{
			if (currentDistance > closestDistance)
			{
				closestDistance = currentDistance;
				closestIndex = index;
			}
		}
		index++;
	}

	return (closestIndex);
}

int CalculateIndex(int i)
{
	int index = 0;

	if (i % 2 == 0)
	{
		index += i / 2;
	}
	else
	{
		index -= i / 2 + 1;
	}

	return (index);
}

void Shape::Join(Shape &joinShape, bool merge)
{
	int vertOffset = vertices.size();
	int joinPoints = joinShape.mergeBottomPoints.size();
	int mainPoints = mergeTopPoints.size();
	int minPoints = glm::min(joinPoints, mainPoints);

	int closestMainPoint = ClosestMergeIndex(joinShape.BottomMergePointsCenter(), true, true);
	int furthestJoinPoint = joinShape.ClosestMergeIndex(TopMergePointsCenter(), false, false);
	//mergeIndex = ClosestMergeIndex(joinShape.GetVertices()[joinShape.mergeBottomPoints[0]]);
	//mergeIndex = minPoints;

	for (int i = 0; i < minPoints; i++)
	{
		//int targeti = ClosestMergeIndex(joinShape.GetVertices()[joinShape.mergeBottomPoints[i]]);

		int index = CalculateIndex(i);

		int i1 = index + furthestJoinPoint;
		int i2 = index + 1 + furthestJoinPoint;

		//int i1 = i + furthestJoinPoint;
		//int i2 = i + 1 + furthestJoinPoint;

		//if (i + 1 >= minPoints) i2 = furthestJoinPoint;
		//if (i + 1 >= minPoints) i2 = CalculateIndex(i - 1) + furthestJoinPoint;

		if (i1 >= joinPoints) i1 -= joinPoints;
		if (i2 >= joinPoints) i2 -= joinPoints;
		if (i1 < 0) i1 += joinPoints;
		if (i2 < 0) i2 += joinPoints;

		//int mi1 = i + closestMainPoint;
		//int mi2 = i + 1 + closestMainPoint;

		int mi1 = index + closestMainPoint;
		int mi2 = index + 1 + closestMainPoint;

		//if (i + 1 >= minPoints) mi2 = CalculateIndex(i - 1) + closestMainPoint;

		if (mi1 >= mainPoints) mi1 -= mainPoints;
		if (mi1 < 0) mi1 += mainPoints;
		if (mi2 >= mainPoints) mi2 -= mainPoints;
		if (mi2 < 0) mi2 += mainPoints;

		indices.push_back(joinShape.mergeBottomPoints[i1] + vertOffset);
		indices.push_back(mergeTopPoints[mi2]);
		indices.push_back(mergeTopPoints[mi1]);

		indices.push_back(joinShape.mergeBottomPoints[i1] + vertOffset);
		indices.push_back(joinShape.mergeBottomPoints[i2] + vertOffset);
		indices.push_back(mergeTopPoints[mi2]);

		glm::vec3 direction = joinShape.GetVertices()[joinShape.mergeBottomPoints[i1]] - vertices[mergeTopPoints[mi1]];
		joinShape.GetVertices()[joinShape.mergeBottomPoints[i1]] -= direction * 0.25f;
		direction = joinShape.GetVertices()[joinShape.mergeBottomPoints[i1]] - vertices[mergeTopPoints[mi1]];
		vertices[mergeTopPoints[mi1]] += direction * 0.1f;

		glm::ivec2 coords = GetVertexCoords(mergeTopPoints[mi1]);
		int range = int(glm::ceil(createResolution * 0.25));
		//int range = 2;
		for (int j = 1; j < range; j++)
		{
			//float gradient = 1.0 - (float(j) / float(range));
			int prev = GetVertexIndex(coords.x - j + 1, coords.y);
			int below = GetVertexIndex(coords.x - j, coords.y);

			direction = vertices[prev] - vertices[below];
			//std::cout << mergeTopPoints[mi1] << " " << below << " " << createResolution << std::endl;
			vertices[below] += direction * 0.25f;
		}

		pointMerged[mi1].x = 1;
		pointMerged[mi2].y = 1;
		//pointMerged[mi2] = true;

		//unUsedPoints.push_back(mi1);
	}

	if (centerMergePoint != -1)
	{
		int index = CalculateIndex(minPoints - 1);
		int i1 = index + 1 + furthestJoinPoint;
		if (i1 >= joinPoints) i1 -= joinPoints;
		if (i1 < 0) i1 += joinPoints;

		int mi1 = index + 1 + closestMainPoint;
		if (mi1 >= mainPoints) mi1 -= mainPoints;
		if (mi1 < 0) mi1 += mainPoints;

		indices.push_back(joinShape.mergeBottomPoints[i1] + vertOffset);
		indices.push_back(centerMergePoint);
		indices.push_back(mergeTopPoints[mi1]);

		index = CalculateIndex(minPoints - 2);

		mi1 = index + closestMainPoint;
		if (mi1 >= mainPoints) mi1 -= mainPoints;
		if (mi1 < 0) mi1 += mainPoints;

		indices.push_back(joinShape.mergeBottomPoints[i1] + vertOffset);
		indices.push_back(mergeTopPoints[mi1]);
		indices.push_back(centerMergePoint);
	}

	Join(joinShape);
}

void Shape::AddIndice(unsigned int indice)
{
	indices.push_back(indice);
}

glm::vec4 Shape::GetBounds()
{
	glm::vec4 bounds = glm::vec4(0);

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

glm::vec3 Shape::BottomMergePointsCenter()
{
	glm::vec3 center = glm::vec3(0);

	for (const unsigned int &i : mergeBottomPoints)
	{
		center += vertices[i];
	}

	center /= mergeBottomPoints.size();

	return (center);
}

glm::vec3 Shape::TopMergePointsCenter()
{
	glm::vec3 center = glm::vec3(0);

	for (const unsigned int &i : mergeTopPoints)
	{
		center += vertices[i];
	}

	center /= mergeTopPoints.size();

	return (center);
}

int Shape::GetVertexIndex(int x, int y)
{
	int root = createResolution + 1;

	return (x + y * root);
}

glm::ivec2 Shape::GetVertexCoords(int i)
{
	int root = createResolution + 1;

	glm::ivec2 coords = glm::ivec2(0);
	coords.y = i / root;
	coords.x = i % root;

	return (coords);
}

int Shape::GetClosestVertex(glm::vec3 position, bool up)
{
	int index = 0;
	int closestIndex = 0;
	float closestDistance = glm::distance(position, vertices[0]);

	for (const glm::vec3 &vertex : vertices)
	{
		//bool inMergePoints = false;
		//for (const unsigned &mergePoint : mergeTopPoints)
		//{
		//	if (index == mergePoint)
		//	{
		//		inMergePoints = true;
		//		break ;
		//	}
		//}

		if ((up && vertex.y <= position.y))
		{
			index++;
			continue ;
		}

		float currentDistance = glm::distance(position, vertex);
		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			closestIndex = index;
		}

		index++;
	}

	return closestIndex;
}

void Shape::CloseUnusedPoints()
{
	if (centerMergePoint == -1) return;

	glm::vec3 center = TopMergePointsCenter();

	int i = 0;
	for (const unsigned int &point : mergeTopPoints)
	{
		if (pointMerged[i].x == 0)
		{
			int mi1 = i;
			int mi2 = i + 1;
			if (mi2 >= mergeTopPoints.size()) mi2 -= mergeTopPoints.size();
			indices.push_back(centerMergePoint);
			indices.push_back(mergeTopPoints[mi2]);
			indices.push_back(mergeTopPoints[mi1]);
		}
		i++;
	}
}

Shape Shape::Copy()
{
	return *this;
}

//std::vector<int> Shape::GetEndVertices()
//{
//
//}
//
//std::vector<int> Shape::GetStartVertices()
//{
//
//}