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

        static void NewFrame();
};

#endif