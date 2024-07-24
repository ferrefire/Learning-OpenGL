#include "time.hpp"

void Time::NewFrame()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

	if (newSecond) newSecond = false;
	if (currentFrame - timeLastSecond >= 1.0)
	{
		newSecond = true;
		timeLastSecond = currentFrame;
	}

	if (newTick) newTick = false;
	if (currentFrame - timeLastTick >= 0.1)
	{
		newTick = true;
		timeLastTick = currentFrame;
	}

	if (newSubTick) newSubTick = false;
	if (currentFrame - timeLastSubTick >= 0.25)
	{
		newSubTick = true;
		timeLastSubTick = currentFrame;
	}

	if (newFrameTick) newFrameTick = false;
	if (currentFrame - timeLastFrameTick >= 0.01666666)
	{
		newFrameTick = true;
		timeLastFrameTick = currentFrame;
	}
}