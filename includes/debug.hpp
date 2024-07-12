#ifndef DEBUG_HPP
#define DEBUG_HPP

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