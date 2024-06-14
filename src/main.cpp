#include <iostream>
#include <glad/glad.h>
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
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f};

unsigned int square_indices[] = {
    0, 1, 2,
    2, 1, 3};

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
}

int main(int argc, char **argv)
{
    //Shape triangle("Triangle", sizeof(triangle_vertices), triangle_vertices, sizeof(triangle_indices), triangle_indices);
    Shape square("Square", sizeof(square_vertices), square_vertices, sizeof(square_indices), square_indices);
    //Shape row("Row", sizeof(row_vertices), row_vertices, sizeof(row_indices), row_indices);

    std::string path = std::filesystem::current_path();

    GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));

    setupSettings(argc, argv);

    Texture brickTex((path + "/textures/brick.png").c_str());
    Texture stoneTex((path + "/textures/stone.png").c_str());

    Shader shader((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment.glsl").c_str());
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    Shader shader2((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment.glsl").c_str());
    shader2.setInt("texture1", 0);
    shader2.setInt("texture2", 1);
    brickTex.bindTexture(GL_TEXTURE0);
    stoneTex.bindTexture(GL_TEXTURE1);

    //Shader shader2((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment_2.glsl").c_str());
    //Shader shader3((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment_3.glsl").c_str());

    Mesh mesh(square, &shader);
    Mesh mesh2(square, &shader2);
    //Mesh row_mesh(row, &shader2);
    //Mesh sqr_mesh(square, &shader3);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
   
        glClear(GL_COLOR_BUFFER_BIT);

		//tri_mesh.shader->setFloat4("ourColor", (sin(glfwGetTime() + 1) / 4.0f) + 0.75f, (sin(glfwGetTime() + 2) / 4.0f) + 0.75f, (sin(glfwGetTime() + 3) / 4.0f) + 0.75f, 1.0f);

		//if (updownPressed)
		//	tri_mesh.shader->setFloat("upsideDown", sin(updown));
		//if (leftrightPressed)
		//	tri_mesh.shader->setFloat("leftsideRight", sin(leftright));

		//tri_mesh.shader->setFloat2("transform", x, y);
        mesh.shader->setFloat("mixAmount", sin(glfwGetTime()) / 2.0f + 0.5f);

        glm::mat4 transformation = glm::mat4(1.0f);
        transformation = glm::translate(transformation, glm::vec3(0.5f, -0.5f, 0.0f));
        transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        mesh.shader->setMatrix4("transform", transformation);

        renderMesh(mesh);

        float time = sin(glfwGetTime());
        glm::mat4 transformation2 = glm::mat4(1.0f);
        transformation2 = glm::scale(transformation2, glm::vec3(time, time, time));

        mesh2.shader->setMatrix4("transform", transformation2);

        renderMesh(mesh2);

        //row_mesh.shader->setFloat4("ourColor", 0.0f, (sin(glfwGetTime() + 2) / 2.0f) + 0.5f, 0.0f, 1.0f);
        //sqr_mesh.shader->setFloat4("ourColor", 0.0f, 0.0f, (sin(glfwGetTime() + 3) / 2.0f) + 0.5f, 1.0f);

		//glBindTexture(GL_TEXTURE_2D, texture.ID);
        

        
		//renderMesh(row_mesh);
        //renderMesh(sqr_mesh);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    quit(EXIT_SUCCESS);
}