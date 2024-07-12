#include "errors.hpp"
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

int quit(int exitCode)
{
    glfwTerminate();

    std::cout << "quit with exit code " << exitCode << std::endl;

    exit(exitCode);
}