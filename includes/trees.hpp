#ifndef TREES_HPP
#define TREES_HPP

#include "shader.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include <future>

class Trees
{
    private:
		static float trunkQuality;
		static float trunkSeed;

		static int minTrunkResolution;
		static int maxTrunkResolution;
		static int minTrunkMainBranchCount;
		static int maxTrunkMainBranchCount;
		static int minTrunkSubBranchCount;
		static int maxTrunkSubBranchCount;
		static float minTrunkMinScaleSize;
		static float maxTrunkMinScaleSize;

		static int tr;
		static int tmbc;
		static int tsbc;
		static float tmss;

	public:
		static unsigned int treeLod0Count;
		static unsigned int treeLod1Count;
		static unsigned int treeLod2Count;

		static unsigned int treeLod0RenderCount;
		static unsigned int treeLod1RenderCount;
		static unsigned int treeLod2RenderCount;

		static bool computeOnTick;
		static bool debugComputeTime;

		static Shader *treeShader;

        static Shader *treeComputeShader;

		static Mesh *treeCombinedMesh;
		static Mesh *treeLod0Mesh;
		static Mesh *treeLod1Mesh;
		static Mesh *treeLod2Mesh;

		static Buffer *treeLod0RenderBuffer;
        static Buffer *treeLod0CountBuffer;
		static Buffer *treeLod1RenderBuffer;
		static Buffer *treeLod1CountBuffer;
		static Buffer *treeLod2RenderBuffer;
		static Buffer *treeLod2CountBuffer;

		//static Buffer *treeCurrentLodBuffer;

		static int trunkResolution;
		static int trunkMainBranchCount;
		static int trunkSubBranchCount;
		static float trunkMinScaleSize;

		static void CreateTrees();
        static void CreateShaders();
        static void CreateBuffers();
        static void CreateMeshes();
		static Mesh *GenerateTrunk(float quality);
		static Shape GenerateBranch(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, glm::vec3 angles, int splitTimes, bool main);
		static void GenerateBranchThreaded(int resolution, glm::vec3 base, glm::vec3 offset, glm::vec2 scale, glm::vec3 angles, 
			int splitTimes, bool main, std::promise<Shape> *promise);

		static void ComputeTrees(bool lod0, bool lod1, bool lod2);
        static void RenderTrees(bool lod0, bool lod1, bool lod2);
        
        static void NewFrame();
};


#endif