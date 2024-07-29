#ifndef TIME_HPP
#define TIME_HPP

#include "glad.h"
#include <GLFW/glfw3.h>

class Time
{
    private:
        /* data */
    public:
        static float deltaTime;
        static float currentFrame;
        static float lastFrame;
		static double timeLastSecond;
		static double timeLastTick;
		static double timeLastSubTick;
		static double timeLastFrameTick;
		static int framesSinceLastFrameTick;

		static bool newSecond;
		static bool newTick;
		static bool newSubTick;
		static bool newFrameTick;
		static bool NewFrameTickOffset(int frames);

		static void NewFrame();
};

#endif