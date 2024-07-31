#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TRIANGLE 0
#define QUAD 1
#define CUBE 2
#define PLANE 3
#define BLADE 4
#define SCREEN_QUAD 5
#define CYLINDER 6

class Shape
{
    private:
        std::string name;

        unsigned int vertexCount;
        unsigned int indiceCount;
        unsigned int dataCount;

        std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

        std::vector<float> data;

		int mergeIndex = 0;

	public:
        Shape();
        Shape(int preset, int resolution = 5);
		~Shape();

        float *GetData();
        unsigned int *GetIndices();

        std::vector<glm::vec3> Vertices();
		std::vector<glm::vec2> Uvs();
		std::vector<glm::vec3> Normals();
		std::vector<unsigned int> Indices();

		std::vector<glm::vec3> &GetVertices();
		std::vector<glm::vec2> &GetUVs();

		unsigned int VertexCount();
        unsigned int IndiceCount();
        unsigned int DataCount();

		std::vector<unsigned int> mergeTopPoints;
		std::vector<unsigned int> mergeBottomPoints;

		bool vertexOnly = false;

		void Rotate(float degrees, const glm::vec3 &axis);
		void RotateVert(int index, float degrees, const glm::vec3 &axis);
		void Translate(const glm::vec3 &translation);
        void Scale(const glm::vec3 &scale);

        void RecalculateData();
		void RecalculateUV();
		void RecalculateNormals();
		void Join(Shape &joinShape);
		void Join(Shape &joinShape, bool merge);
		void AddIndice(unsigned int indice);

		int ClosestMergeIndex(glm::vec3 position);

		glm::vec4 GetBounds();
		glm::vec3 BottomMergePointsCenter();
		glm::vec3 TopMergePointsCenter();
		//std::vector<int> GetEndVertices();
		//std::vector<int> GetStartVertices();
};

#endif