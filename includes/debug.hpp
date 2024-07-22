#ifndef DEBUG_HPP
#define DEBUG_HPP

class Debug
{
	private:
		
	public:
		static unsigned int totalFramesThisSecond;
		static unsigned int totalFramesLastSecond;
		static unsigned int totalFramesThisTick;
		static unsigned int totalFramesLastTick;
		static float totalFrameTimeThisTick;
		static float totalFrameTimeLastTick;
		static float start;
		
		static void NewFrame();
		static void DurationCheck();
		static void CalculateFramesPerSecond();
};

#endif