#include "input.hpp"
#include <glm/glm.hpp>
#include "manager.hpp"

void Input::AddKey(int keyCode)
{
    keys[keyCode] = Input::KeyStatus();
}

void Input::SetKeyStatus(GLFWwindow *window)
{
    for (auto &key : keys)
    {
        bool lastFrameDown = key.second.down;
        key.second.down = glfwGetKey(window, key.first) == GLFW_PRESS;

        key.second.pressed = (key.second.down && !lastFrameDown);
        key.second.released = (!key.second.down && lastFrameDown);
    }
}

void Input::processInput(GLFWwindow *window)
{
    SetKeyStatus(window);

	if (GetKey(GLFW_KEY_Q).pressed) Manager::Quit();

    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    //{
    //    Manager::wireframeActive = !Manager::wireframeActive;
    //    if (Manager::wireframeActive)
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    else
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}

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

Input::KeyStatus Input::GetKey(int keyCode)
{
    if (keys.find(keyCode) == keys.end())
    {
        AddKey(keyCode);
        return (Input::KeyStatus());
    }

    return (keys[keyCode]);
}