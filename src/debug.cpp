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
}