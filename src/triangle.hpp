#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glm/glm.hpp>
#include <tuple>
#include <vector>
#include <iostream>

class Triangle
{
	private:
		std::tuple<glm::vec3, glm::vec3, glm::vec3> vertices;
		std::tuple<int, int, int> indices;
		glm::vec3 center;
	public:
		Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int i1, int i2, int i3);
		glm::vec3 Center();
		glm::vec3 GetVert(int i);
		int GetInd(int i);
		std::tuple<glm::vec3, glm::vec3, glm::vec3> GetVertices();
		std::tuple<int, int, int> GetIndices();
		std::vector<glm::vec3> GetVerticesVec();
		std::vector<int> GetIndicesVec();
		void PrintVert(int i);
		void Print();
};

#endif