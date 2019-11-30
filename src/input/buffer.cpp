
#include "stdafx.h"
#include "buffer.h"

using crib::input::buffer;


void buffer::push(crib::input::event&& evt)
{
	back.push_back(evt);
}

void buffer::push(const UINT message, const WPARAM wParam, const LPARAM lParam, const double time)
{
	back.emplace_back(message, wParam, lParam, time);
}


buffer& buffer::swap()
{
	// TODO: to make this class thread-safe, ensure no access to buffer during execution of this function.
	// This may need creating a new type for reference-counted iterators, as return value to begin()/end(),
	// or maybe vector<> itself can invalidate iterators?

	front.swap(back);
	back.clear();

	return *this;
}


std::vector<crib::input::event>::const_iterator buffer::begin() const
{
	return front.begin();
}

std::vector<crib::input::event>::const_iterator buffer::end() const
{
	return front.end();
}
