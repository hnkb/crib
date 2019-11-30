
#pragma once

#include <chrono>


namespace crib
{
	namespace core
	{

		class timer
		{
		public:
			timer() : frame_origin(std::chrono::high_resolution_clock::now()) {}

			double next_frame();
			double now() const;

		protected:
			std::chrono::time_point<std::chrono::high_resolution_clock> frame_origin;
		};

	}
}
