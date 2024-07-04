#include "debug.hpp"

void Debug::NewFrame()
{
	if (timeLastSecond == 0)
		timeLastSecond = glfwGetTime();

	totalFramesThisSecond++;

	if (glfwGetTime() - timeLastSecond > 1.0f)
	{
		std::cout << "FPS: " << totalFramesThisSecond << std::endl;
		totalFramesThisSecond = 0;
		timeLastSecond = 0;
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
}