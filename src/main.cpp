#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
//#include <string.h>
#include <stdio.h>
#include "manager.hpp"
#include "shape.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "learning.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cmath>
#include "stb_image.h"
#include "texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include "camera.hpp"
#include "input.hpp"
#include "object.hpp"
#include "terrain.hpp"
#include "cinematic.hpp"
#include "grass.hpp"
#include "buffer.hpp"
#include "trees.hpp"

unsigned int Utilities::seed = 1;
bool Utilities::useSeed = false;
std::hash<float> Utilities::floatHash;
std::hash<size_t> Utilities::sizetHash;

unsigned int Debug::totalFramesThisSecond = 0;
unsigned int Debug::totalFramesLastSecond = 0;
unsigned int Debug::totalFramesThisTick = 0;
unsigned int Debug::totalFramesLastTick = 0;
float Debug::totalFrameTimeThisTick = 0;
float Debug::totalFrameTimeLastTick = 0;
float Debug::start = -1;

unsigned int Shader::currentActiveShader = 0;
unsigned int Mesh::currentActiveVAO = 0;

float Time::deltaTime = 0.0f;
float Time::currentFrame = 0.0f;
float Time::lastFrame = 0.0f;
double Time::timeLastSecond = 0;
double Time::timeLastTick = 0;
double Time::timeLastSubTick = 0;
double Time::timeLastFrameTick = 0;
bool Time::newSecond = false;
bool Time::newTick = false;
bool Time::newSubTick = false;
bool Time::newFrameTick = false;
int Time::framesSinceLastFrameTick = 0;

float Input::height = 900.0;
float Input::width = 1600.0;
float Input::lastX = Input::width * 0.5f;
float Input::lastY = Input::height * 0.5f;
float Input::sensitivity = 0.1f;
bool Input::canMove = true;
bool Input::canLook = true;
std::map<int, Input::KeyStatus> Input::keys = std::map<int, Input::KeyStatus>();

Camera cam = Camera();

std::vector<Object *> Manager::objects = std::vector<Object *>();
std::vector<Manager::InstanceBatch> Manager::instanceBatches = std::vector<Manager::InstanceBatch>();
std::vector<Shader *> Manager::shaders = std::vector<Shader *>();
std::vector<Shape *> Manager::shapes = std::vector<Shape *>();
std::vector<Texture *> Manager::textures = std::vector<Texture *>();
std::vector<Mesh *> Manager::meshes = std::vector<Mesh *>();
std::vector<Buffer *> Manager::buffers = std::vector<Buffer *>();
bool Manager::wireframeActive = false;
bool Manager::vSyncActive = true;
bool Manager::mouseLocked = true;
bool Manager::fullScreen = false;
bool Manager::cullingActive = true;
bool Manager::firstFrame = true;
unsigned int Manager::depthBuffer = 0;
glm::vec3 Manager::sunDirection = glm::normalize(glm::vec3(1, 1, 1));
glm::vec2 Manager::sunAngles = glm::vec2(0, 0);
Camera &Manager::camera = cam;
GLFWwindow *Manager::window = NULL;
Cinematic Manager::activeCinematic;

float Terrain::terrainSize = 0;
float Terrain::terrainShadowSize = 25000.0;
float Terrain::terrainLod0Size = 2500.0;
float Terrain::terrainLod1Size = 5000.0;
float Terrain::terrainHeight = 5000.0;
float Terrain::terrainScale = 0.75;
int Terrain::terrainLayers = 8;
float Terrain::terrainChunkSize = 10000.0;
int Terrain::terrainLod0Resolution = 1024;
int Terrain::terrainLod1Resolution = 1024;
int Terrain::terrainShadowResolution = 512;
int Terrain::terrainChunkResolution = 1024;
int Terrain::chunkRadius = 4;
int Terrain::chunksLength = 0;
int Terrain::chunkCount = 0;
int Terrain::computePartLod0 = 0;
int Terrain::computePartLod1 = 0;
int Terrain::xi = 0;
int Terrain::yi = 0;
int Terrain::index = 0;
bool Terrain::generating = false;
glm::vec2 Terrain::terrainOffset = glm::vec2(0.0, 0.0);
glm::vec2 Terrain::offsetLod0 = glm::vec2(0.0, 0.0);
glm::vec2 Terrain::offsetLod1 = glm::vec2(0.0, 0.0);
glm::vec2 Terrain::seed = glm::vec2(0.0, 0.0);
Texture *Terrain::heightMapLod0Texture = NULL;
Texture *Terrain::heightMapLod1Texture = NULL;
Texture *Terrain::heightMapArrayTexture = NULL;
Texture *Terrain::shadowMapTexture = NULL;
Texture *Terrain::depthMapTexture = NULL;
Shader *Terrain::terrainShader = NULL;
Shader *Terrain::terrainLodShader = NULL;
Shader *Terrain::heightMapComputeShader = NULL;
Shader *Terrain::heightMapArrayComputeShader = NULL;
Shader *Terrain::shadowMapComputeShader = NULL;
Mesh *Terrain::terrainMesh = NULL;
Mesh *Terrain::terrainLod0Mesh = NULL;
Mesh *Terrain::terrainLod1Mesh = NULL;
Object ***Terrain::terrainChunks = NULL;
int Terrain::terrainRadius = 3;
int Terrain::terrainLength = 0;
int Terrain::terrainCount = 0;
float Terrain::worldSampleDistance = 1;

unsigned int Grass::grassCount = 384;
unsigned int Grass::grassLodCount = 2048 + 1024;
unsigned int Grass::grassRenderCount = 0;
unsigned int Grass::grassLodRenderCount = 0;
Shader *Grass::grassShader = NULL;
Shader *Grass::grassComputeShader = NULL;
Buffer *Grass::grassBuffer = NULL;
Buffer *Grass::grassLodBuffer = NULL;
Buffer *Grass::countBuffer = NULL;
Buffer *Grass::countLodBuffer = NULL;
Mesh *Grass::grassMesh = NULL;
Mesh *Grass::grassLodMesh = NULL;

unsigned int Trees::treeLod0Count = 16;
unsigned int Trees::treeLod1Count = 32;
unsigned int Trees::treeLod2Count = 128;
unsigned int Trees::treeLod0RenderCount = 0;
unsigned int Trees::treeLod1RenderCount = 0;
unsigned int Trees::treeLod2RenderCount = 0;
Shader *Trees::treeShader = NULL;
Shader *Trees::treeComputeShader = NULL;
Mesh *Trees::treeLod0Mesh = NULL;
Mesh *Trees::treeLod1Mesh = NULL;
Mesh *Trees::treeLod2Mesh = NULL;
Buffer *Trees::treeLod0RenderBuffer = NULL;
Buffer *Trees::treeLod0CountBuffer = NULL;
Buffer *Trees::treeLod1RenderBuffer = NULL;
Buffer *Trees::treeLod1CountBuffer = NULL;
Buffer *Trees::treeLod2RenderBuffer = NULL;
Buffer *Trees::treeLod2CountBuffer = NULL;

int Trees::minTrunkResolution = 8;
int Trees::maxTrunkResolution = 24;
int Trees::minTrunkMainBranchCount = 0;
int Trees::maxTrunkMainBranchCount = 4;
int Trees::minTrunkSubBranchCount = 0;
int Trees::maxTrunkSubBranchCount = 2;
float Trees::minTrunkMinScaleSize = 1.0;
float Trees::maxTrunkMinScaleSize = 0.1;

int Trees::tr = 0;
int Trees::tmbc = 0;
int Trees::tsbc = 0;
float Trees::tmss = 0;

float Trees::trunkQuality = 1.0;
float Trees::trunkSeed = 32.0;

bool makeCinematic = false;
std::string makeCinName;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	std::cout << width << "-" << height << std::endl;
	Input::width = width;
	Input::height = height;
	cam.UpdateProjection();
    glViewport(0, 0, width, height);
}

GLFWwindow *setupGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (Manager::fullScreen)
	{
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    	Input::width = mode->width;
    	Input::height = mode->height;
	}
    GLFWwindow *window = glfwCreateWindow(Input::width, Input::height, "LearnOpenGL", Manager::fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return (NULL);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return (NULL);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return (window);
}

void setupSettings(int argc, char **argv, GLFWwindow *window)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    Manager::EnableVsync(true);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Input::mouse_callback);
    glfwSetScrollCallback(window, Input::scroll_callback);
	cam.speed = 100.0f;
}

void GetArguments(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (Utilities::Contains(arg, "FULL")) Manager::fullScreen = true;
		else if (Utilities::Contains(arg, "MAKECIN="))
		{
			makeCinematic = true;
			makeCinName = (Utilities::GetPath() + "/cinematics/" + (argv[i] + arg.find('=') + 1) + ".txt");
		}
		else if (Utilities::Contains(arg, "CIN=")) 
		{
			Manager::activeCinematic.Load((Utilities::GetPath() + "/cinematics/" + (argv[i] + arg.find('=') + 1) + ".txt").c_str());
			Manager::activeCinematic.speed = 1;
		}
		else if (Utilities::Contains(arg, "TERRAIN_RES=")) Terrain::terrainLod0Resolution = std::stof(argv[i] + arg.find('=') + 1);
		else if (Utilities::Contains(arg, "TERRAIN_CHUNK_RES=")) Terrain::terrainChunkResolution = std::stof(argv[i] + arg.find('=') + 1);
		else if (Utilities::Contains(arg, "TERRAIN_HEIGHT=")) Terrain::terrainHeight = std::stof(argv[i] + arg.find('=') + 1);
		else if (Utilities::Contains(arg, "TERRAIN_SCALE=")) Terrain::terrainScale = std::stof(argv[i] + arg.find('=') + 1);
		else if (Utilities::Contains(arg, "TERRAIN_LAYERS=")) Terrain::terrainLayers = std::stof(argv[i] + arg.find('=') + 1);
	}
}

void Print(int val)
{
    std::cout << val << std::endl;
}

int main(int argc, char **argv)
{
	// std::cout << glm::mix(25.0f, 45.0f, 0.5f) << std::endl;
	// std::cout << glm::sqrt(25) << std::endl;
	//return 0;

	// Utilities::seed = Time::GetTime();
	Utilities::seed = rand();
	GetArguments(argc, argv);
	GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));
	Manager::window = window;
	setupSettings(argc, argv, window);

	const GLubyte *renderer = glGetString(GL_RENDERER);
	printf("%s\n", (char *)renderer);

	GLint max_layers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
	std::cout << "max texture array layers: " << max_layers << std::endl;

	std::cout << "max compute groups: ";
	GLint max_compute_groups;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &max_compute_groups);
	std::cout << max_compute_groups;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &max_compute_groups);
	std::cout << " " << max_compute_groups;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &max_compute_groups);
	std::cout << " " << max_compute_groups << std::endl;

	Terrain::CreateTerrain();
	Grass::CreateGrass();
	Trees::CreateTrees();

	Shader *quadShader = new Shader("screen_quad_vertex.glsl", "screen_quad_fragment.glsl");
	Manager::AddShader(quadShader);

	Shape *screenQuadShape = new Shape(SCREEN_QUAD);
	Manager::AddShape(screenQuadShape);

	Mesh *screenQuadMesh = new Mesh(screenQuadShape, quadShader);
	Manager::AddMesh(screenQuadMesh);

	quadShader->setInt("quadTexture", Terrain::depthMapTexture->Index());

	double lastTime = 0;

	while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Time::NewFrame();
		Debug::NewFrame();
        Input::ProcessInput();
		Manager::SetShaderFrameVariables();

		if (Manager::firstFrame) Manager::activeCinematic.Start();

		if (makeCinematic && Input::GetKey(GLFW_MOUSE_BUTTON_RIGHT, true).pressed)
		{
			glm::vec3 newPosKey;
			float newPosDur;
			newPosDur = glfwGetTime() - lastTime;
			newPosKey.x = Manager::camera.Position().x + Terrain::terrainOffset.x;
			newPosKey.y = Manager::camera.Position().y;
			newPosKey.z = Manager::camera.Position().z + Terrain::terrainOffset.y;
			Manager::activeCinematic.AddKeyPosition(newPosKey, newPosDur);
			//pos.push_back(newPosKey);
			glm::vec3 newRotKey;
			float newRotDur;
			newRotDur = glfwGetTime() - lastTime;
			newRotKey.x = Manager::camera.Angles().x;
			newRotKey.y = Manager::camera.Angles().y;
			newRotKey.z = Manager::camera.Angles().z;
			Manager::activeCinematic.AddKeyRotation(newRotKey, newRotDur);
			lastTime = glfwGetTime();
		}

		/*bool sunMoved = false;
		if (Input::GetKey(GLFW_KEY_DOWN).down)
		{
			Manager::sunAngles.y += Time::deltaTime * 0.25;
			sunMoved = true;
		}
		if (Input::GetKey(GLFW_KEY_UP).down)
		{
			Manager::sunAngles.y -= Time::deltaTime * 0.25;
			sunMoved = true;
		}
		if (Input::GetKey(GLFW_KEY_RIGHT).down) 
		{
			Manager::sunAngles.x -= Time::deltaTime * 0.25;
			sunMoved = true;
		}
		if (Input::GetKey(GLFW_KEY_LEFT).down)
		{
			Manager::sunAngles.x += Time::deltaTime * 0.25;
			sunMoved = true;
		}
		if (sunMoved)
		{
			Manager::sunDirection = glm::normalize(glm::vec3(1, 1, 1));
			Manager::sunDirection = Utilities::RotateVec3(Manager::sunDirection, Manager::sunAngles.y * 360.0, glm::vec3(1, 0, 0));
			Manager::sunDirection = Utilities::RotateVec3(Manager::sunDirection, Manager::sunAngles.x * 360.0, glm::vec3(0, 1, 0));
			Manager::sunDirection = glm::normalize(Manager::sunDirection);
			Shader::setFloat3Global("lightDirection", Manager::sunDirection);
		}*/
		
		Terrain::NewFrame();
		Trees::NewFrame();
		Grass::NewFrame();

		//quadShader->useShader();
		//screenQuadMesh->UseMesh();
		//glDrawElements(GL_TRIANGLES, screenQuadMesh->GetShape()->IndiceCount(), GL_UNSIGNED_INT, 0);

		Manager::NewFrame();
		if (Manager::firstFrame) Manager::firstFrame = false;

		//glBindVertexArray(0);
		//Mesh::currentActiveVAO = 0;
		//glUseProgram(0);
		//Shader::currentActiveShader = 0;

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	if (makeCinematic) Manager::activeCinematic.Create(makeCinName.c_str());

    Manager::Quit();
}