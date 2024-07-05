#include "time.hpp"

void Time::NewFrame()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

	if (timeLastSecond == 0)
	{
		newSecond = false;
		timeLastSecond = currentFrame;
	}
	if (currentFrame - timeLastSecond > 1.0f)
	{
		newSecond = true;
		timeLastSecond = 0;
	}
}