#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

class Debug
{
	private:
		
	public:
		static double timeLastSecond;
		static unsigned int totalFramesThisSecond;
		
		static void NewFrame();
};

#endif