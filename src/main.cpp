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
#include "triangle.hpp"

float updown = 1, leftright = 1;
bool updownPressed, leftrightPressed;
float x, y;

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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glClearColor(0.25f, 0.87f, 0.81f, 1.0f);

	updownPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	if (updownPressed)
		updown += 0.02f;
	leftrightPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	if (leftrightPressed)
		leftright += 0.02f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		y += 0.02f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		y -= 0.02f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		x += 0.02f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		x -= 0.02f;
}

GLFWwindow *setupGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    //glm::vec4 v = glm::vec4(0.5f, 1.0f, 0.25f, 1.0f);
    //v.
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
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
    {
        processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh.GetShader()->setFloat("mixAmount", sin(glfwGetTime()) / 2.0f + 0.5f);

		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() + 1, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -y, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, x, glm::vec3(0.0f, 1.0f, 0.0f));

		mesh.GetShader()->setMatrix4("model", model);
		mesh.GetShader()->setMatrix4("view", view);
		mesh.GetShader()->setMatrix4("projection", projection);

		renderMesh(mesh);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    quit(EXIT_SUCCESS);
}