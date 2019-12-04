
#pragma once

#include <Crib/Platform/Windows.h>
#include <iterator>
#include <vector>


namespace crib
{
	namespace input
	{

		class event
		{
		public:
			event() : event(WM_NULL, 0, 0, 0) {}
			event(const UINT msg, const WPARAM wparam, const LPARAM lparam, const double timestamp) : message(msg), wParam(wparam), lParam(lparam), time(timestamp) {}

			UINT message;
			WPARAM wParam;
			LPARAM lParam;
			double time;
		};

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
