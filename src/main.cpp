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

std::tuple<int, int> getClosest(glm::vec3 from, std::vector<glm::vec3> verts, std::vector<int> used_indices)
{
	int i1 = -1;
	int i2 = -1;
	float d1 = 100;
	float d2 = 100;
	float dt;

	int i = 0;
	while (i < verts.size())
	{
		if (std::find(used_indices.begin(), used_indices.end(), i) != used_indices.end())
		{
			i++;
			continue;
		}
		dt = glm::distance(from, verts[i]);
		if (dt < d1)
		{
			i2 = i1;
			d2 = d1;
			d1 = dt;
			i1 = i;
		}
		else if (dt < d2)
		{
			d2 = dt;
			i2 = i;
		}
		i++;
	}

	return (std::make_tuple(i1, i2));
}

int getClosestOne(glm::vec3 from, std::vector<glm::vec3> verts, std::vector<int> used_indices)
{
	return (std::get<0>(getClosest(from, verts, used_indices)));
}

int getVertIndex(glm::vec3 vec, std::vector<glm::vec3> verts)
{
	for (int i = 0; i < verts.size(); i++)
	{
		if (verts[i].x == vec.x && verts[i].y == vec.y && verts[i].z == vec.z)
			return (i);
	}

	return (-1);
}

bool indicesExist(int i1, int i2, int i3, std::vector<Triangle> triangles)
{
	for (int i = 0; i < triangles.size(); i++)
	{
		bool i1f = i1 == triangles[i].GetInd(0) || i1 == triangles[i].GetInd(1) || i1 == triangles[i].GetInd(2);
		if (!i1f) continue;
		bool i2f = i2 == triangles[i].GetInd(0) || i2 == triangles[i].GetInd(1) || i2 == triangles[i].GetInd(2);
		if (!i2f) continue;
		bool i3f = i3 == triangles[i].GetInd(0) || i3 == triangles[i].GetInd(1) || i3 == triangles[i].GetInd(2);
		if (!i3f) continue;
		if (i1f && i2f && i3f) return (true);
	}

	return (false);
}

int main(int argc, char **argv)
{
	std::vector<glm::vec3> verts;
	verts.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
	verts.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
	verts.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
	verts.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
	verts.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
	verts.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
	verts.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
	verts.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
	//verts.push_back(glm::vec3(-0.5f, -0.8f, 0.5f));
	//verts.push_back(glm::vec3(0.5f, -0.8f, 0.5f));
	//verts.push_back(glm::vec3(-0.5f, -1.0f, 0.75f));
	//verts.push_back(glm::vec3(0.5f, -1.0f, 0.75f));
	std::vector<glm::vec2> uvs;
	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(1.0f, 0.0f));
	uvs.push_back(glm::vec2(0.0f, 1.0f));
	uvs.push_back(glm::vec2(1.0f, 1.0f));
	uvs.push_back(glm::vec2(0.0f, 0.0f));
	uvs.push_back(glm::vec2(1.0f, 0.0f));
	uvs.push_back(glm::vec2(0.0f, 1.0f));
	uvs.push_back(glm::vec2(1.0f, 1.0f));

	std::vector<Triangle> triangles;

	//std::vector<std::tuple<int, int, int>> indices;
	std::vector<int> indices_used;

	indices_used.push_back(0);
	std::tuple<int, int> closest = getClosest(verts[0], verts, indices_used);
	indices_used.push_back(std::get<0>(closest));
	indices_used.push_back(std::get<1>(closest));
	triangles.push_back(Triangle(verts[0], verts[std::get<0>(closest)], verts[std::get<1>(closest)], 0, std::get<0>(closest), std::get<1>(closest)));
	//indices.push_back(std::make_tuple(0, std::get<0>(closest), std::get<1>(closest)));
	//glm::vec3 triangle_center = verts[std::get<0>(indices[0])];
	//triangle_center += verts[std::get<1>(indices[0])];
	//triangle_center += verts[std::get<2>(indices[0])];
	//triangle_center /= 3.0f;
	
	while (indices_used.size() < verts.size())
	{
		int closest_to_center = getClosestOne(triangles.back().Center(), verts, indices_used);
		indices_used.push_back(closest_to_center);
		closest = getClosest(verts[closest_to_center], triangles.back().GetVerticesVec(), std::vector<int>());
		int i1 = getVertIndex(triangles.back().GetVert(std::get<0>(closest)), verts);
		int i2 = getVertIndex(triangles.back().GetVert(std::get<1>(closest)), verts);
		triangles.push_back(Triangle(verts[closest_to_center], verts[i1], verts[i2], closest_to_center, i1, i2));
		//triangles.push_back(Triangle(verts[i2], verts[i1], verts[closest_to_center], i2, i1, closest_to_center));
	}

	int index = 0;
	while (index < triangles.size())
	{
		std::tuple<int, int> closestPair = getClosest(triangles[index].Center(), verts, triangles[index].GetIndicesVec());
		int c1 = std::get<0>(closestPair);
		// int c2 = std::get<1>(closestPair);
		closest = getClosest(verts[c1], triangles[index].GetVerticesVec(), std::vector<int>());
		int i1 = getVertIndex(triangles[index].GetVert(std::get<0>(closest)), verts);
		int i2 = getVertIndex(triangles[index].GetVert(std::get<1>(closest)), verts);
		if (indicesExist(c1, i1, i2, triangles))
		{
			c1 = std::get<1>(closestPair);
			closest = getClosest(verts[c1], triangles[index].GetVerticesVec(), std::vector<int>());
			i1 = getVertIndex(triangles[index].GetVert(std::get<0>(closest)), verts);
			i2 = getVertIndex(triangles[index].GetVert(std::get<1>(closest)), verts);
		}
		if (!indicesExist(c1, i1, i2, triangles))
		{
			triangles.push_back(Triangle(verts[c1], verts[i1], verts[i2], c1, i1, i2));
			index = 0;
		}
		else
		{
			index++;
		}
	}
	 

	//triangles[0].Print();
	//std::cout << std::endl;
	//triangles[1].Print();
	//return (0);
	std::vector<float> vertices;
	for (int i = 0; i < verts.size(); i++)
	{
		vertices.push_back(verts[i].x);
		vertices.push_back(verts[i].y);
		vertices.push_back(verts[i].z);
		//vertices.push_back(uvs[i].x);
		//vertices.push_back(uvs[i].y);
		vertices.push_back((rand() % 100) / 100.0f);
		vertices.push_back((rand() % 100) / 100.0f);
	}
	std::vector<unsigned int> indices;
	for (int i = 0; i < triangles.size(); i++)
	{
		indices.push_back(triangles[i].GetInd(0));
		indices.push_back(triangles[i].GetInd(1));
		indices.push_back(triangles[i].GetInd(2));
	}
	//std::vector<float> uv{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	//std::vector<float> end_vertices;
	//for (int i = 0; i < verts.size(); i++)
	//{
	//	end_vertices.push_back(vertices[3 * i]);
	//	end_vertices.push_back(vertices[3 * i + 1]);
	//	end_vertices.push_back(vertices[3 * i + 2]);
	//	end_vertices.push_back(uvs[]);
	//	end_vertices.push_back(uv[2 * i + 1]);
	//}

	

	//std::cout << indicesExist(0, 1, 2, triangles) << " true" << std::endl;
	//std::cout << indicesExist(3, 1, 2, triangles) << " true" << std::endl;
	//std::cout << indicesExist(9, 1, 2, triangles) << " false" << std::endl;
	//std::cout << indicesExist(7, 1, 2, triangles) << " false" << std::endl;
	//std::cout << indicesExist(7, 4, 3, triangles) << " false" << std::endl;
	//std::cout << indicesExist(1, 3, 7, triangles) << " true" << std::endl;
	//std::cout << indicesExist(4, 7, 6, triangles) << " true" << std::endl;

	if (argc > 1 && std::string(argv[1]).compare("debug") == 0)
	{
		std::cout << std::endl;
		for (int i = 0; i < vertices.size(); i++)
		{
			std::cout << vertices[i] << " ";
			if ((i + 1) % 5 == 0)
				std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < indices.size(); i++)
		{
			std::cout << indices[i] << " ";
			if ((i + 1) % 3 == 0)
				std::cout << std::endl;
		}
		std::cout << std::endl;

		return (0);
	}

	//Shape triangle("Triangle", sizeof(triangle_vertices), triangle_vertices, sizeof(triangle_indices), triangle_indices);
	//Shape square("Square", sizeof(square_vertices), square_vertices, sizeof(square_indices), square_indices);
	float ev[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), ev);
	unsigned int ind[indices.size()];
	std::copy(indices.begin(), indices.end(), ind);
	Shape square("Square", sizeof(ev), ev, sizeof(ind), ind);
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
    //Shader shader2((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment.glsl").c_str());
    //shader2.setInt("texture1", 0);
    //shader2.setInt("texture2", 1);
    brickTex.bindTexture(GL_TEXTURE0);
    stoneTex.bindTexture(GL_TEXTURE1);

    //Shader shader2((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment_2.glsl").c_str());
    //Shader shader3((path + "/shaders/simple_vertex.glsl").c_str(), (path + "/shaders/simple_fragment_3.glsl").c_str());

	Mesh mesh(square.vertices, square.indices, &shader);
	//Mesh mesh(end_vertices, indices, &shader);
	//Mesh mesh2(square, &shader2);
    //Mesh row_mesh(row, &shader2);
    //Mesh sqr_mesh(square, &shader3);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
    {
        processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//tri_mesh.shader->setFloat4("ourColor", (sin(glfwGetTime() + 1) / 4.0f) + 0.75f, (sin(glfwGetTime() + 2) / 4.0f) + 0.75f, (sin(glfwGetTime() + 3) / 4.0f) + 0.75f, 1.0f);

		//if (updownPressed)
		//	tri_mesh.shader->setFloat("upsideDown", sin(updown));
		//if (leftrightPressed)
		//	tri_mesh.shader->setFloat("leftsideRight", sin(leftright));

		//tri_mesh.shader->setFloat2("transform", x, y);
		//std::cout << "b" << std::endl;
		mesh.shader->setFloat("mixAmount", sin(glfwGetTime()) / 2.0f + 0.5f);
		//std::cout << "a" << std::endl;

		//glm::mat4 transformation = glm::mat4(1.0f);
        //transformation = glm::translate(transformation, glm::vec3(0.5f, -0.5f, 0.0f));
        //transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() + 1, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -y, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, x, glm::vec3(0.0f, 1.0f, 0.0f));

		//mesh.shader->setMatrix4("transform", transformation);
		mesh.shader->setMatrix4("model", model);
		mesh.shader->setMatrix4("view", view);
		mesh.shader->setMatrix4("projection", projection);

		renderMesh(mesh);

        //float time = sin(glfwGetTime());
        //glm::mat4 transformation2 = glm::mat4(1.0f);
        //transformation2 = glm::scale(transformation2, glm::vec3(time, time, time));

        //mesh2.shader->setMatrix4("transform", transformation2);

        //renderMesh(mesh2);

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