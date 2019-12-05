
#include "timer.h"

using Crib::Timer;


double Timer::nextFrame()
{
	auto lastFrame = frameOrigin;
	frameOrigin = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double>(frameOrigin - lastFrame).count();
}

double Timer::now() const
{
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - frameOrigin).count();
}
