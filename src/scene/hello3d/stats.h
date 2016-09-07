
#pragma once

#include "crib.h"
#include <algorithm>
#include <string>


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class stats
			{
			public:
				void update(const double delta, const input::buffer& input)
				{
					time += delta;
					frames += 1.;
					buffer_size = std::max(buffer_size, size_t(input.end() - input.begin()));
				}

				const double avg_fps() const { return frames / time; }

				double time = 0;
				double frames = 0;
				size_t buffer_size = 0;
			};

		}
	}
}
