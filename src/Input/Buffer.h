
#pragma once

#include <Crib/Platform/Windows.h>
#include <iterator>
#include <vector>


namespace Crib::Input
{

	class Event
	{
	public:
		Event() : Event(WM_NULL, 0, 0, 0) {}
		Event(const UINT msg, const WPARAM wparam, const LPARAM lparam, const double timestamp) : message(msg), wParam(wparam), lParam(lparam), time(timestamp) {}

		UINT message;
		WPARAM wParam;
		LPARAM lParam;
		double time;
	};

	class Buffer
	{
	public:
		void push(Event&& evt);
		void push(const UINT message, const WPARAM wParam, const LPARAM lParam, const double time);
		Buffer& swap();
		std::vector<Event>::const_iterator begin() const { return front.begin(); }
		std::vector<Event>::const_iterator end() const { return front.end(); }

	private:
		std::vector<Event> front, back;
	};

}
