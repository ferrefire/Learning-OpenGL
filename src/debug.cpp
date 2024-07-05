#include "debug.hpp"

void Debug::NewFrame()
{
	totalFramesThisSecond++;

	if (Time::newSecond)
	{
		std::cout << "FPS: " << totalFramesThisSecond << std::endl;
		totalFramesThisSecond = 0;
	}

    if (Input::GetKey(GLFW_KEY_L).pressed)
    {
        Manager::wireframeActive = !Manager::wireframeActive;

        if (Manager::wireframeActive)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (Input::GetKey(GLFW_KEY_V).pressed)
    {
        Manager::vSyncActive = !Manager::vSyncActive;
        glfwSwapInterval(Manager::vSyncActive);
    }

	if (Input::GetKey(GLFW_KEY_M).pressed)
	{
		Manager::mouseLocked = !Manager::mouseLocked;
		glfwSetInputMode(Manager::window, GLFW_CURSOR, Manager::mouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}

void Debug::DurationCheck()
{
	if (start == -1)
	{
		start = glfwGetTime();
	}
	else
	{
		std::cout << "duration: " << (glfwGetTime() - start) * 1000.0 << " ms" << std::endl;
		start = -1;
	}
}