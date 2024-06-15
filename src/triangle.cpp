#include "triangle.hpp"

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int i1, int i2, int i3)
{
	vertices = std::make_tuple(v1, v2, v3);
	center = (v1 + v2 + v3) / 3.0f;
	indices = std::make_tuple(i1, i2, i3);
}

glm::vec3 Triangle::Center()
{
	return (center);
}

glm::vec3 Triangle::GetVert(int i)
{
	if (i == 0)
		return (std::get<0>(vertices));
	else if (i == 1)
		return (std::get<1>(vertices));
	else if (i == 2)
		return (std::get<2>(vertices));
	else
		return (glm::vec3());
}

int Triangle::GetInd(int i)
{
	if (i == 0)
		return (std::get<0>(indices));
	else if (i == 1)
		return (std::get<1>(indices));
	else if (i == 2)
		return (std::get<2>(indices));
	else
		return (-1);
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> Triangle::GetVertices()
{
	return (vertices);
}

std::tuple<int, int, int> Triangle::GetIndices()
{
	return (indices);
}

std::vector<glm::vec3> Triangle::GetVerticesVec()
{
	std::vector<glm::vec3> vec;
	vec.push_back(std::get<0>(vertices));
	vec.push_back(std::get<1>(vertices));
	vec.push_back(std::get<2>(vertices));
	return (vec);
}

std::vector<int> Triangle::GetIndicesVec()
{
	std::vector<int> vec;
	vec.push_back(std::get<0>(indices));
	vec.push_back(std::get<1>(indices));
	vec.push_back(std::get<2>(indices));
	return (vec);
}

void Triangle::PrintVert(int i)
{
	std::cout << " (" << GetVert(i).x << ", " << GetVert(i).y << ", " << GetVert(i).z << ") ";
}

void Triangle::Print()
{
	std::cout << "vertices:";
	PrintVert(0);
	PrintVert(1);
	PrintVert(2);
	std::cout << std::endl;
	std::cout << "indices: " << std::get<0>(indices) << ", " << std::get<1>(indices) << ", " << std::get<2>(indices) << std::endl;
}