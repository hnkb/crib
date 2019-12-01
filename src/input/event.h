
#pragma once

#include <Crib/Platform/Windows.h>


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

	}
}
