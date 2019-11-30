
#pragma once

#include "event.h"
#include <iterator>
#include <vector>


namespace crib
{
	namespace input
	{

		class buffer
		{
		public:
			void push(event&& evt);
			void push(const UINT message, const WPARAM wParam, const LPARAM lParam, const double time);
			buffer& swap();
			std::vector<event>::const_iterator begin() const;
			std::vector<event>::const_iterator end() const;

		private:
			std::vector<event> front, back;
		};

	}
}
