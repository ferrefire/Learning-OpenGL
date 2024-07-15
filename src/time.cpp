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

	if (timeLastTick == 0)
	{
		newTick = false;
		timeLastTick = currentFrame;
	}
	if (currentFrame - timeLastTick > 0.02f)
	{
		newTick = true;
		timeLastTick = 0;
	}

	if (timeLastSubTick == 0)
	{
		newSubTick = false;
		timeLastSubTick = currentFrame;
	}
	if (currentFrame - timeLastSubTick > 0.25f)
	{
		newSubTick = true;
		timeLastSubTick = 0;
	}
}