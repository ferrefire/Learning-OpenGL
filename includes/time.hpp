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

		static bool newSecond;
		static bool newTick;
		static bool newSubTick;

		static void NewFrame();
};

#endif