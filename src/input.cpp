#include "input.hpp"

void Input::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.Move(camera.Front());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.Move(-camera.Front());
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.Move(camera.Side());
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.Move(-camera.Side());
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.Move(camera.Up());
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.Move(-camera.Up());
    }
}

void Input::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    //xpos = glm::clamp((float)xpos, 0.0f, width);
    //ypos = glm::clamp((float)ypos, 0.0f, height);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.Rotate(camera.Angles() + glm::vec3(yoffset, xoffset, 0.0f));
}

void Input::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if ((float)yoffset > 0.0f)
        camera.speed *= 1.25f;
    else if ((float)yoffset < 0.0f)
        camera.speed *= 0.8f;
}