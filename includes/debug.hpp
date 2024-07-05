#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "time.hpp"
#include "input.hpp"
#include "manager.hpp"

class Debug
{
	private:
		
	public:
		static unsigned int totalFramesThisSecond;
		static double start;
		
		static void NewFrame();
		static void DurationCheck();
};

#endif