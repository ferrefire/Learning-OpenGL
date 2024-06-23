#ifndef INPUT_HPP
#define INPUT_HPP

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.hpp"

class Input
{
    private:
        

    public:
        static Camera &camera;

        static float lastX;
        static float lastY;
        static float sensitivity;
        static float height;
        static float width;

        static void processInput(GLFWwindow *window);
        static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif