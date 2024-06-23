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
#include "camera.hpp"
#include "input.hpp"

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

float updown = 1, leftright = 1;
bool updownPressed, leftrightPressed;


float triangle_vertices[] = {
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,};

unsigned int triangle_indices[] = {
    0, 1, 2};

float row_vertices[] = {
    -1.0f, -0.5f, 0.0f, 
    0.0f, -0.5f, 0.0f, 
    -0.5f, 0.5f, 0.0f,
    1.0f, -0.5f, 0.0f, 
    0.5f, 0.5f, 0.0f
};

unsigned int row_indices[] = {
    0, 1, 2,
    1, 3, 4
};

float square_vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	
	//-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //0.5f, 0.5f, 0.5f, 1.0f, 1.0f
	};

unsigned int square_indices[] = {
	0, 1, 2,
	2, 1, 3,

	//4, 5, 6,
	//6, 5, 7
	};

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (argc > 1 && strlen(argv[1]) == 1 && isdigit(*argv[1]) && *argv[1] - '0' == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cam.speed = 10.0f;
    glfwSetCursorPosCallback(window, Input::mouse_callback);
    glfwSetScrollCallback(window, Input::scroll_callback);
    
    //Input::cam = &cam;
}

int main(int argc, char **argv)
{
    //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
    //return (0);

	std::string path = std::filesystem::current_path();

    GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));

    setupSettings(argc, argv, window);

    Texture brickTex((path + "/textures/brick.png").c_str());
    Texture stoneTex((path + "/textures/stone.png").c_str());

    Shader shader((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment.glsl").c_str());
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    brickTex.bindTexture(GL_TEXTURE0);
    stoneTex.bindTexture(GL_TEXTURE1);
    //shader.setFloat3("lightPos", 0.25f, 0.25f, 0.5f);
    shader.setFloat3("lightPos", 2500.0f, 2500.0f, 5000.0f);

    Shape shape(CUBE);

    Mesh mesh(&shape, &shader);

    int count = 100;

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

    int dis = 250;

    if (argc > 2)
    {
        int i = 0;
        while (i >= 0 && argv[2][i])
        {
            if (!isdigit(argv[2][i]))
            {
                i = -1;
                break;
            }
            i++;
        }
        if (i > 0)
        {
            dis = atoi(argv[2]);
        }
    }

    glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, dis * 5.0f);

	float min = dis / 2.0f;

	glm::vec3 transformations[count];
	//glm::mat4 models[count];
	for (int i = 0; i < count; i++)
    {
        srand(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + i);
		transformations[i] = glm::vec3((rand() % dis) - min, (rand() % dis) - min, (rand() % dis) - min);
		//models[i] = glm::translate(glm::mat4(1.0f), transformations[i]);
	}

	while (!glfwWindowShouldClose(window))
    {
        Time::NewFrame();
		Debug::NewFrame();

        Input::processInput(window);

		//Utilities::PrintVec3(camPos);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::vec3 foward = glm::normalize(camPos - glm::vec3(0.0f, 0.0f, 0.0f));
		//view = glm::lookAt(camPos, camPos + foward, glm::vec3(0.0f, 1.0f, 0.0f));

		//view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::translate(glm::mat4(1.0f), glm::vec3(x * -50, z * -50, y * 50));

		//mesh.GetShader()->setFloat("mixAmount", sin(glfwGetTime()) / 2.0f + 0.5f);
		mesh.GetShader()->setFloat("time", glfwGetTime());
		mesh.GetShader()->setMatrix4("view", cam.View());
        mesh.GetShader()->setMatrix4("projection", projection);
        mesh.GetShader()->setFloat3("viewPos", cam.Position());

        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() + 1, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 0.0f, 1.0f));

		for (int i = 0; i < count; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, transformations[i]);
    		float angle = 20.0f * i;
    		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    		model = glm::rotate(model, (float)glfwGetTime() + 10 * i, glm::vec3(1.0f, 0.0f, 0.0f));
    		model = glm::rotate(model, (float)glfwGetTime() + 20 * i, glm::vec3(0.0f, 1.0f, 0.0f));

            // model = glm::rotate(model, -y, glm::vec3(1.0f, 0.0f, 0.0f));
            // model = glm::rotate(model, x, glm::vec3(0.0f, 1.0f, 0.0f));

			//mesh.GetShader()->setMatrix4("model", models[i]);
			mesh.GetShader()->setMatrix4("model", model);
			mesh.GetShader()->setInt("id", i);
            //srand(i);
            //float r = (rand() % 1000) / 1000.0f;
            //srand(r * 1000);
            //float g = (rand() % 1000) / 1000.0f;
            //srand(g * 1000);
            //float b = (rand() % 1000) / 1000.0f;
            //mesh.GetShader()->setFloat4("colMult", r, g, b, 1.0f);

            renderMesh(mesh);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    quit(EXIT_SUCCESS);
}