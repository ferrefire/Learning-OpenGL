#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>
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
#include "camera.hpp"
#include "input.hpp"
#include "object.hpp"
#include "manager.hpp"

const int width = 1600;
const int height = 900;

double Debug::timeLastSecond = 0;
unsigned int Debug::totalFramesThisSecond = 0;
unsigned int Shader::currentActiveShader = INT_MAX;

float Time::deltaTime = 0.0f;
float Time::currentFrame = 0.0f;
float Time::lastFrame = 0.0f;

Camera cam = Camera();

float Input::height = (float)height;
float Input::width = (float)width;
float Input::lastX = width * 0.5f;
float Input::lastY = height * 0.5f;
float Input::sensitivity = 0.1f;
Camera &Input::camera = cam;

std::vector<Object *> Manager::objects = std::vector<Object *>();
std::vector<Manager::InstanceBatch> Manager::instanceBatches = std::vector<Manager::InstanceBatch>();
std::vector<Shader *> Manager::shaders = std::vector<Shader *>();
Camera &Manager::camera = cam;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    //printf("Window resized: width-%d height-%d\n", width, height);
}

GLFWwindow *setupGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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
    if (argc > 1 && strlen(argv[1]) == 1 && isdigit(*argv[1]) && *argv[1] - '0' == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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

int main(int argc, char **argv)
{
    int count = 10000;

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

    setupSettings(argc, argv, window);

    //Texture brickTex((path + "/textures/brick.png").c_str());
    //Texture stoneTex((path + "/textures/stone.png").c_str());

    Shader shader((path + "/shaders/default_vertex.glsl").c_str(), (path + "/shaders/default_fragment.glsl").c_str());
    Shader instanceShader((path + "/shaders/instanced_vertex.glsl").c_str(), (path + "/shaders/instanced_fragment.glsl").c_str());
    //shader.setInt("texture1", 0);
    //shader.setInt("texture2", 1);
    //brickTex.bindTexture(GL_TEXTURE0);
    //stoneTex.bindTexture(GL_TEXTURE1);
    shader.setFloat3("lightPosition", 25000.0f, 25000.0f, 50000.0f);
    shader.setMatrix4("projection", cam.Projection());
    shader.setFloat("near", cam.near);
    shader.setFloat("far", cam.far);
    instanceShader.setFloat3("lightPosition", 25000.0f, 25000.0f, 50000.0f);
    instanceShader.setMatrix4("projection", cam.Projection());
    instanceShader.setFloat("near", cam.near);
    instanceShader.setFloat("far", cam.far);
    //instanceShader.setInt("instanceCount", 10000);
    //instanceShader.setInt("instanceCountSqrt", 100);

    Manager::AddShader(&shader);
    Manager::AddShader(&instanceShader);

    Shape shape(CUBE);
    Shape plane(QUAD);
    plane.Scale(glm::vec3(1000.0f));
    plane.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    Mesh mesh(&shape, &shader);
    Mesh instanceMesh(&shape, &instanceShader);
    Mesh planeMesh(&plane, &shader);

    //mesh.GetShader()->setInt("countRoot", count * count);
    //mesh.GetShader()->setInt("countRootRoot", count);

    Object object(&mesh);
    object.Paint(glm::vec4(glm::vec3(0.25f), 1.0f));
    Object floor(&planeMesh);
    floor.Paint(glm::vec4(0.1f, 0.75f, 0.1f, 1.0f));

    Manager::AddObject(&object);
    Manager::AddObject(&floor);

    Manager::AddInstanceBatch(&instanceMesh, count);

    while (!glfwWindowShouldClose(window))
    {
        Time::NewFrame();
		Debug::NewFrame();

        Input::processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        object.Move(glm::vec3(0.0f, Time::deltaTime * 0.1f, Time::deltaTime));
        object.Rotate(glm::vec3(Time::deltaTime * 100));

        //object.GetMesh()->GetShader()->setMatrix4("model", model);
        //object.GetMesh()->GetShader()->setFloat("time", glfwGetTime());
		//object.GetMesh()->GetShader()->setMatrix4("view", cam.View());
        ////object.GetMesh()->GetShader()->setMatrix4("projection", projection);
        //object.GetMesh()->GetShader()->setFloat3("viewPos", cam.Position());

        //renderMeshInstanced(mesh, count * count * count);

        Manager::NewFrame();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    quit(EXIT_SUCCESS);
}