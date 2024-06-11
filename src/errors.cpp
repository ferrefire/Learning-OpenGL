#include "errors.hpp"

int quit(int exitCode)
{
    glfwTerminate();

    std::cout << "quit with exit code " << exitCode << std::endl;

    exit(exitCode);
}