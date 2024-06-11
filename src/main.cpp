#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>
#include "shape.hpp"
#include "mesh.hpp"
#include "learning.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cmath>

int shape_index = 0;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";
const char *fragment2ShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   " FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\0";
const char *fragment3ShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
                                    "}\0";

float triangle_vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};

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
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f};

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) glClearColor(0.25f, 0.87f, 0.81f, 1.0f);
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
    if (argc > 1 && strlen(argv[1]) == 1 && isdigit(*argv[1]) && *argv[1] - '0' < 3 && *argv[1] - '0' >= 0)
        shape_index = *argv[1] - '0';
    if (argc > 2 && strlen(argv[2]) == 1 && isdigit(*argv[2]) && *argv[2] - '0' == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char **argv)
{
    Shape triangle("Triangle", sizeof(triangle_vertices), triangle_vertices, sizeof(triangle_indices), triangle_indices);
    Shape square("Square", sizeof(square_vertices), square_vertices, sizeof(square_indices), square_indices);
    Shape row("Row", sizeof(row_vertices), row_vertices, sizeof(row_indices), row_indices);

    std::string path = std::filesystem::current_path();

    GLFWwindow *window = setupGLFW();
    if (window == NULL) return (quit(EXIT_FAILURE));

    setupSettings(argc, argv);

    unsigned int shaderProgram1 = createShaderProgram((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment.glsl").c_str());
    unsigned int shaderProgram2 = createShaderProgram((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment_2.glsl").c_str());
    unsigned int shaderProgram3 = createShaderProgram((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment_3.glsl").c_str());

    int fragColorLocation = glGetUniformLocation(shaderProgram3, "ourColor");

    Mesh tri_mesh(triangle, shaderProgram1);
    Mesh row_mesh(row, shaderProgram2);
    Mesh sqr_mesh(square, shaderProgram3);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
   
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram3);
        glUniform4f(fragColorLocation, 0.0f, (sin(glfwGetTime()) / 2.0f) + 0.5f, 0.0f, 1.0f);

        renderMesh(row_mesh);
        renderMesh(sqr_mesh);
        renderMesh(tri_mesh);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    quit(EXIT_SUCCESS);
}