
#pragma once

#include <chrono>


namespace Crib
{

	class Timer
	{
	public:
		Timer() : frameOrigin(std::chrono::high_resolution_clock::now()) {}

		double nextFrame();
		double now() const;

	protected:
		std::chrono::time_point<std::chrono::high_resolution_clock> frameOrigin;
	};

}
