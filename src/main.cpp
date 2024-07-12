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
int Manager::heightMapResolution = 1024;
int Manager::heightMapChunkResolution = 1024;
Camera &Manager::camera = cam;
GLFWwindow *Manager::window = NULL;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    //printf("Window resized: width-%d height-%d\n", width, height);
}

GLFWwindow *setupGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
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

    std::string path = std::filesystem::current_path();

    GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));
	Manager::window = window;

	setupSettings(argc, argv, window);

    //Texture brickTex((path + "/textures/brick.png").c_str());
    //Texture stoneTex((path + "/textures/stone.png").c_str());    

    //Shader shader("default_vertex.glsl", "default_fragment.glsl");
    Shader terrainShader("terrain_vertex.glsl", "tesselation_control.glsl", "tesselation_evaluation.glsl", "terrain_fragment.glsl");
    Shader instanceShader("instanced_vertex.glsl", "instanced_fragment.glsl");
    //shader.setInt("texture1", 0);
    //shader.setInt("texture2", 1);
    //brickTex.bindTexture(GL_TEXTURE0);
    //stoneTex.bindTexture(GL_TEXTURE1);

    Shader computeShader("compute_shader.glsl");
	computeShader.setInt("instanceCount", count);
	computeShader.setFloat("instanceMult", 1.0 / float(count));
	computeShader.setInt("instanceCountSqrt", sqrt(count));
	computeShader.setFloat("instanceCountSqrtMult", 1.0 / float(sqrt(count)));

	Shader heightmapComputeShader("heightmap_compute.glsl");
	heightmapComputeShader.setInt("heightMap", 0);
	heightmapComputeShader.setInt("heightMapArray", 1);

	Shader heightmapArrayComputeShader("heightmapArray_compute.glsl");
	heightmapArrayComputeShader.setInt("heightMap", 0);
	heightmapArrayComputeShader.setInt("heightMapArray", 1);

	terrainShader.setInt("heightMap", 0);
	terrainShader.setInt("heightMapArray", 1);

	Manager::heightMapResolution = 4096;
	Manager::heightMapChunkResolution = 1024;

	heightmapComputeShader.setFloat("resolution", Manager::heightMapResolution);
	heightmapComputeShader.setFloat("scale", 1);

	heightmapArrayComputeShader.setFloat("resolution", Manager::heightMapChunkResolution);
	heightmapArrayComputeShader.setFloat("scale", 1);
	heightmapArrayComputeShader.setInt("chunksRadius", 1);

	unsigned int texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16_SNORM, Manager::heightMapResolution, Manager::heightMapResolution, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16_SNORM);

	unsigned int textureArray;

	glGenTextures(1, &textureArray);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R16_SNORM, Manager::heightMapChunkResolution, Manager::heightMapChunkResolution, 
		9, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(1, textureArray, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R16_SNORM);

	//Print(cam.far);

    //Manager::AddShader(&shader);

    //Shape shape(CUBE);
    Shape instanceShape(BLADE);
    //instanceShape.Scale(glm::vec3(0.25f, 2.0f, 1.0f));
    Shape plane(PLANE);
    plane.Scale(glm::vec3(10000.0f));

    //Mesh mesh(&shape, &shader);
    Mesh instanceMesh(&instanceShape, &instanceShader);
    Mesh planeMesh(&plane, &terrainShader);

    //Object object(&mesh);
    //object.Paint(glm::vec4(glm::vec3(0.25f), 1.0f));
    Object floor(&planeMesh);
	Object floor2(&planeMesh);
	Object floor3(&planeMesh);
	//floor.Move(glm::vec3(0, 0, 0));
	floor2.Move(glm::vec3(10000.0, 0.0, -10000.0));
	floor3.Move(glm::vec3(0.0, 0.0, 10000.0));
	//floor.Paint(glm::vec4(60.0f, 160.0f, 20.0f, 255.0f) / 255.0f);
    floor.Paint(glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));
	floor2.Paint(glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));
	floor3.Paint(glm::vec4(0.2f, 0.5f, 0.05f, 1.0f));

	//Manager::AddObject(&object);
	Manager::AddShader(&terrainShader);
	Manager::AddShader(&instanceShader);
	Manager::AddShader(&computeShader);
	Manager::AddShader(&heightmapComputeShader);
	Manager::AddShader(&heightmapArrayComputeShader);
	Manager::AddObject(&floor);
	Manager::AddObject(&floor2);
	Manager::AddObject(&floor3);
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

	glm::vec2 offset = glm::vec2(0, 0);
    heightmapComputeShader.useShader();
    heightmapComputeShader.setFloat2("offset", offset);
	glDispatchCompute(Manager::heightMapResolution / 4, Manager::heightMapResolution / 4, 1);

	heightmapArrayComputeShader.useShader();
	glDispatchCompute(Manager::heightMapChunkResolution / 4, Manager::heightMapChunkResolution / 4, 1);

	while (!glfwWindowShouldClose(window))
    {
        Time::NewFrame();
		Debug::NewFrame();
        Input::processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//if (Time::newSecond)
		//{
		//	Debug::DurationCheck();
		//	heightmapComputeShader.useShader();
		//	offset += glm::vec2(0.1, 0.1);
		//	heightmapComputeShader.setFloat2("offset", offset);
		//	glDispatchCompute(Manager::heightMapResolution / 4, Manager::heightMapResolution / 4, 1);
		//	//glMemoryBarrier(GL_ALL_BARRIER_BITS);
		//	//Debug::DurationCheck();
		//}

        if (Input::GetKey(GLFW_KEY_G).pressed)
        {
            offset += glm::vec2(1, 1);
            heightmapComputeShader.useShader();
            heightmapComputeShader.setFloat2("offset", offset);
            glDispatchCompute(Manager::heightMapResolution / 4, Manager::heightMapResolution / 4, 1);
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