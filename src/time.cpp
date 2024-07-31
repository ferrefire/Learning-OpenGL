#include "time.hpp"
#include <chrono>
#include <ctime>

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

	framesSinceLastFrameTick++;
	if (newFrameTick) newFrameTick = false;
	if (currentFrame - timeLastFrameTick >= 0.01666666)
	{
		newFrameTick = true;
		timeLastFrameTick = currentFrame;
		framesSinceLastFrameTick = 0;
	}
}

bool Time::NewFrameTickOffset(int frames)
{
	return (framesSinceLastFrameTick - frames == 0);
}

unsigned int Time::GetTime()
{
	std::time_t result = std::time(nullptr);
	return result;
}