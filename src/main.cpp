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

double Debug::timeLastSecond = 0;
unsigned int Debug::totalFramesThisSecond = 0;
unsigned int Shader::currentActiveShader = INT_MAX;

float updown = 1, leftright = 1;
bool updownPressed, leftrightPressed;
float x, y, z;
int width = 1600;
int height = 900;

glm::vec3 camPos = glm::vec3(3.0f, 5.0f, 10.0f);

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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    //if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	//	glClearColor(0.25f, 0.87f, 0.81f, 1.0f);

	updownPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	if (updownPressed)
		updown += 0.02f;
	leftrightPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	if (leftrightPressed)
		leftright += 0.02f;

	y = 0;
	x = 0;
	z = 0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		y = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		y = -1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		x = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		x = -1.0f;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		z = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		z = -1.0f;

	//glm::vec3 foward = glm::normalize(camPos - glm::vec3(0.0f, 0.0f, 0.0f));
	//foward = glm::normalize(camPos - (camPos + foward));
	glm::vec3 foward = glm::normalize(camPos - glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 sideways = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), foward));
	glm::vec3 upwards = glm::cross(foward, sideways);
	glm::vec3 movement = foward * -y + sideways * x + upwards * z;
	//Utilities::PrintVec3(movement);
	//glm::vec3 movement = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(-x, -z, y, 0.0f);
	//Utilities::PrintVec3(movement);
	camPos += movement;
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

void setupSettings(int argc, char **argv)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (argc > 1 && strlen(argv[1]) == 1 && isdigit(*argv[1]) && *argv[1] - '0' == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
    //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
    //return (0);

	std::string path = std::filesystem::current_path();

    GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));

    setupSettings(argc, argv);

    Texture brickTex((path + "/textures/brick.png").c_str());
    Texture stoneTex((path + "/textures/stone.png").c_str());

    Shader shader((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment.glsl").c_str());
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    brickTex.bindTexture(GL_TEXTURE0);
    stoneTex.bindTexture(GL_TEXTURE1);

    Shape shape(CUBE);

    Mesh mesh(&shape, &shader);

	glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

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
		Debug::NewFrame();

        processInput(window);

		//Utilities::PrintVec3(camPos);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::vec3 foward = glm::normalize(camPos - glm::vec3(0.0f, 0.0f, 0.0f));
		//view = glm::lookAt(camPos, camPos + foward, glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::translate(glm::mat4(1.0f), glm::vec3(x * -50, z * -50, y * 50));

		//mesh.GetShader()->setFloat("mixAmount", sin(glfwGetTime()) / 2.0f + 0.5f);
		mesh.GetShader()->setFloat("time", glfwGetTime());
		mesh.GetShader()->setMatrix4("view", view);
        mesh.GetShader()->setMatrix4("projection", projection);

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