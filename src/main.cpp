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

const int width = 1600;
const int height = 900;

unsigned int Debug::totalFramesThisSecond = 0;
double Debug::start = -1;

unsigned int Shader::currentActiveShader = 0;
unsigned int Mesh::currentActiveVAO = 0;

float Time::deltaTime = 0.0f;
float Time::currentFrame = 0.0f;
float Time::lastFrame = 0.0f;
double Time::timeLastSecond = 0;
bool Time::newSecond = false;

Camera cam = Camera();

float Input::height = (float)height;
float Input::width = (float)width;
float Input::lastX = width * 0.5f;
float Input::lastY = height * 0.5f;
float Input::sensitivity = 0.1f;
Camera &Input::camera = cam;
std::map<int, Input::KeyStatus> Input::keys = std::map<int, Input::KeyStatus>();

std::vector<Object *> Manager::objects = std::vector<Object *>();
std::vector<Manager::InstanceBatch> Manager::instanceBatches = std::vector<Manager::InstanceBatch>();
std::vector<Shader *> Manager::shaders = std::vector<Shader *>();
bool Manager::wireframeActive = false;
bool Manager::vSyncActive = true;
bool Manager::mouseLocked = true;
Camera &Manager::camera = cam;
GLFWwindow *Manager::window = NULL;

float Terrain::terrainSize = 30000.0;
float Terrain::terrainChunkSize = 10000.0;
int Terrain::terrainResolution = 4096;
int Terrain::terrainChunkResolution = 1024;
int Terrain::chunkRadius = 1;
int Terrain::chunksLength = 3;
int Terrain::chunkCount = 9;
glm::vec2 Terrain::offset = glm::vec2(0.0, 0.0);
unsigned int Terrain::heightMapTexture = 0;
unsigned int Terrain::heightMapArrayTexture = 0;
Shader *Terrain::terrainShader = NULL;
Shader *Terrain::heightMapComputeShader = NULL;
Shader *Terrain::heightMapArrayComputeShader = NULL;
Object *Terrain::terrainObject = NULL;
Object ***Terrain::terrainChunks = NULL;
int Terrain::terrainRadius = 1;
int Terrain::terrainLength = 3;
int Terrain::terrainCount = 9;
float Terrain::sampleStepSize = 0.0001;
float Terrain::worldSampleStepSize = 1;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    //printf("Window resized: width-%d height-%d\n", width, height);
}

GLFWwindow *setupGLFW(bool fullScreen)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
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
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //if (argc > 1 && strlen(argv[1]) == 1 && isdigit(*argv[1]) && *argv[1] - '0' == 1)
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glfwSwapInterval(1);
    //glEnable(GL_PROGRAM_POINT_SIZE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cam.speed = 10.0f;
    glfwSetCursorPosCallback(window, Input::mouse_callback);
    glfwSetScrollCallback(window, Input::scroll_callback);
}

float GetRandom01()
{
    return (float(rand() % 1000) / 1000.0f);
}

float GetRandom11()
{
    return (((float(rand() % 1000) / 1000.0f) - 0.5f) * 2.0f);
}

void Print(int val)
{
    std::cout << val << std::endl;
}

int main(int argc, char **argv)
{
    int count = 16;

    if (argc > 1)
    {
        int i = 0;
        while (i >= 0 && argv[1][i])
        {
            if (!isdigit(argv[1][i]))
            {
                i = -1;
                break;
            }
            i++;
        }
        if (i > 0)
        {
            count = atoi(argv[1]);
        }
    }
	bool fullScreen = false;
	if (argc > 2)
	{
		std::string arg = argv[2];
		if (Utilities::Contains(arg, "FULL")) fullScreen = true;
	}

	std::string path = std::filesystem::current_path();

    GLFWwindow *window = setupGLFW(fullScreen);
    if (window == NULL) return (quit(EXIT_FAILURE));
	Manager::window = window;

	setupSettings(argc, argv, window);
	Terrain::CreateTerrain(30000, 10000, 4096, 1024, 1, 1); 
	//Terrain::CreateTerrain();
	Shader instanceShader("instanced_vertex.glsl", "instanced_fragment.glsl");

    Shader computeShader("compute_shader.glsl");
	computeShader.setInt("instanceCount", count);
	computeShader.setFloat("instanceMult", 1.0 / float(count));
	computeShader.setInt("instanceCountSqrt", sqrt(count));
	computeShader.setFloat("instanceCountSqrtMult", 1.0 / float(sqrt(count)));
	computeShader.setInt("heightMap", 0);
	computeShader.setInt("heightMapArray", 1);

    Shape instanceShape(BLADE);

    Mesh instanceMesh(&instanceShape, &instanceShader);

	Manager::AddShader(&instanceShader);
	Manager::AddShader(&computeShader);
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

    int inssqrt = sqrt(count);
    Print(count);
    Print(inssqrt);

	while (!glfwWindowShouldClose(window))
    {
        Time::NewFrame();
		Debug::NewFrame();
        Input::processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (Input::GetKey(GLFW_KEY_G).pressed)
        {
            //Terrain::offset += glm::vec2(1, 1);
			//Terrain::heightMapComputeShader->setFloat2("offset", Terrain::offset);
			//Terrain::heightMapArrayComputeShader->setFloat2("offset", Terrain::offset);
			//Terrain::GenerateHeightMap();
			//Terrain::GenerateHeightMapArray();

			Terrain::offset = glm::vec2(Manager::camera.Position().x, Manager::camera.Position().z) / Terrain::terrainChunkSize;
			Shader::setFloat2Global("terrainOffset", Terrain::offset * Terrain::terrainChunkSize);
			Terrain::heightMapComputeShader->setFloat2("offset", Terrain::offset);
			Terrain::GenerateHeightMap();
		}

        computeShader.useShader();
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

		//if (Time::newSecond) Debug::DurationCheck();

        Manager::NewFrame();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    quit(EXIT_SUCCESS);
}