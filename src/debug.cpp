#include "debug.hpp"
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "time.hpp"
#include "input.hpp"
#include "manager.hpp"
#include "utilities.hpp"

void Debug::CalculateFramesPerSecond()
{
	totalFramesThisSecond++;
	totalFramesThisTick++;
	totalFrameTimeThisTick += Time::deltaTime;

	if (Time::newSecond)
	{
		// std::cout << "FPS: " << totalFramesThisSecond << std::endl;
		// Utilities::PrintVec3(Manager::camera.Position());
		totalFramesLastSecond = totalFramesThisSecond;
		totalFramesThisSecond = 0;
	}

	if (Time::newSubTick)
	{
		glfwSetWindowTitle(Manager::window, std::to_string(int(totalFramesThisTick / totalFrameTimeThisTick)).c_str());

		totalFramesThisTick = 0;
		totalFrameTimeThisTick = 0;
	}
}

void Debug::NewFrame()
{
	CalculateFramesPerSecond();

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
		Manager::EnableVsync(!Manager::vSyncActive);
    }

	if (Input::GetKey(GLFW_KEY_M).pressed)
	{
		Manager::mouseLocked = !Manager::mouseLocked;
		glfwSetInputMode(Manager::window, GLFW_CURSOR, Manager::mouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	if (Input::GetKey(GLFW_KEY_P).pressed)
	{
		Manager::camera.PrintStatus();
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

void Debug::DurationCheck(const std::string &message)
{
	if (start != -1)
	{
		std::cout << message << " ";
	}
	DurationCheck();
}