
#include "timer.h"

using crib::core::timer;


double timer::next_frame()
{
	auto last_frame = frame_origin;
	frame_origin = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double>(frame_origin - last_frame).count();
}

double timer::now() const
{
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - frame_origin).count();
}
