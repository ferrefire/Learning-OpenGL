#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <string.h>
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
#include <map>
#include "camera.hpp"
#include "input.hpp"
#include "object.hpp"
#include "terrain.hpp"

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
bool Time::newSecond = false;
bool Time::newTick = false;
bool Time::newSubTick = false;

float Input::height = 900.0;
float Input::width = 1600.0;
float Input::lastX = Input::width * 0.5f;
float Input::lastY = Input::height * 0.5f;
float Input::sensitivity = 0.1f;
Camera cam = Camera();
Camera &Input::camera = cam;
std::map<int, Input::KeyStatus> Input::keys = std::map<int, Input::KeyStatus>();

std::vector<Object *> Manager::objects = std::vector<Object *>();
std::vector<Manager::InstanceBatch> Manager::instanceBatches = std::vector<Manager::InstanceBatch>();
std::vector<Shader *> Manager::shaders = std::vector<Shader *>();
std::vector<Shape *> Manager::shapes = std::vector<Shape *>();
std::vector<Texture *> Manager::textures = std::vector<Texture *>();
std::vector<Mesh *> Manager::meshes = std::vector<Mesh *>();
bool Manager::wireframeActive = false;
bool Manager::vSyncActive = true;
bool Manager::mouseLocked = true;
bool Manager::fullScreen = false;
bool Manager::cullingActive = true;
unsigned int Manager::depthBuffer = 0;
glm::vec3 Manager::sunDirection = glm::normalize(glm::vec3(1, 1, 1));
glm::vec2 Manager::sunAngles = glm::vec2(0, 0);
Camera &Manager::camera = cam;
GLFWwindow *Manager::window = NULL;

float Terrain::terrainSize = 90000.0;
float Terrain::terrainLod0Size = 2500.0;
float Terrain::terrainLod1Size = 5000.0;
float Terrain::terrainOccludeSize = 1000.0;
float Terrain::terrainHeight = 10000.0;
float Terrain::terrainScale = 1;
int Terrain::terrainLayers = 8;
float Terrain::terrainChunkSize = 10000.0;
int Terrain::terrainLod0Resolution = 1024;
int Terrain::terrainLod1Resolution = 1024;
int Terrain::terrainOcclusionResolution = 1024;
int Terrain::terrainChunkResolution = 1024;
int Terrain::chunkRadius = 4;
int Terrain::chunksLength = 9;
int Terrain::chunkCount = 81;
glm::vec2 Terrain::terrainOffset = glm::vec2(0.0, 0.0);
glm::vec2 Terrain::offsetLod0 = glm::vec2(0.0, 0.0);
glm::vec2 Terrain::offsetLod1 = glm::vec2(0.0, 0.0);
glm::vec2 Terrain::seed = glm::vec2(0.0, 0.0);
Texture *Terrain::heightMapLod0Texture = NULL;
Texture *Terrain::heightMapLod1Texture = NULL;
unsigned int Terrain::occlusionMapTexture = 0;
unsigned int Terrain::heightMapArrayTexture = 0;
Shader *Terrain::terrainShader = NULL;
Shader *Terrain::heightMapComputeShader = NULL;
Shader *Terrain::occlusionMapComputeShader = NULL;
Shader *Terrain::heightMapArrayComputeShader = NULL;
Mesh *Terrain::terrainMesh = NULL;
Mesh *Terrain::terrainLodMesh = NULL;
Object ***Terrain::terrainChunks = NULL;
int Terrain::terrainRadius = 2;
int Terrain::terrainLength = 5;
int Terrain::terrainCount = 25;
float Terrain::worldSampleDistance = 1;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
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
    glfwSwapInterval(1);
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
	std::string path = std::filesystem::current_path();

	GetArguments(argc, argv);
	GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));
	Manager::window = window;
	setupSettings(argc, argv, window);

	const GLubyte *renderer = glGetString(GL_RENDERER);
	printf("%s\n", (char *)renderer);

	//Terrain::CreateTerrain(90000, 10000, 2500, 1024, 1024, 4, 1);
	Terrain::CreateTerrain();
	Shader *instanceShader = new Shader("instanced_vertex.glsl", "instanced_fragment.glsl");

    //int count = 1048576;
    int count = 4194304;

    Shader *computeShader = new Shader("compute_shader.glsl");
	computeShader->setInt("instanceCount", count);
	computeShader->setFloat("instanceMult", 1.0 / float(count));
	computeShader->setInt("instanceCountSqrt", sqrt(count));
	computeShader->setFloat("instanceCountSqrtMult", 1.0 / float(sqrt(count)));
	computeShader->setInt(Terrain::heightMapLod0Texture->Name().c_str(), Terrain::heightMapLod0Texture->Index());
	computeShader->setInt(Terrain::heightMapLod1Texture->Name().c_str(), Terrain::heightMapLod1Texture->Index());
	computeShader->setInt("heightMapArray", 2);
	computeShader->setInt("occlusionMap", 3);
	//computeShader.setInt("frameBuffer", 2);

	Shape instanceShape(BLADE);
    Mesh instanceMesh(&instanceShape, instanceShader);

	Manager::AddShader(instanceShader);
	Manager::AddShader(computeShader);
	Manager::AddInstanceBatch(&instanceMesh, count);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, count * sizeof(float) * 8, NULL, GL_STATIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    unsigned int computeCount;
    glGenBuffers(1, &computeCount);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeCount);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int), NULL, GL_STATIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, computeCount);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//glGenFramebuffers(1, &Manager::depthBuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, Manager::depthBuffer);
//
	//unsigned int depthTexture;
	//glGenTextures(1, &depthTexture);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, depthTexture);
	////Change color type
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Input::width, Input::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthTexture, 0);
//
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int inssqrt = sqrt(count);
    Print(count);
    Print(inssqrt);

	while (!glfwWindowShouldClose(window))
    {
		Time::NewFrame();
		Debug::NewFrame();
        Input::processInput(window);
		Manager::SetShaderFrameVariables();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bool sunMoved = false;
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
		}
		
		Terrain::NewFrame();
		
		computeShader->useShader();
        glDispatchCompute(inssqrt / 4, inssqrt / 4, 1);
        //glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeCount);
        void *ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
        unsigned int cCount = *(unsigned int *)ptr;
        //Print(cCount);
        Manager::instanceBatches[0].count = cCount;
        *(unsigned int *)ptr = 0;
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        Manager::NewFrame();

		glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Manager::Quit();
}