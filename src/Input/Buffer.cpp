
#include "buffer.h"

using Crib::Input::Buffer;


void Buffer::push(Event&& evt)
{
	back.push_back(evt);
}

void Buffer::push(const UINT message, const WPARAM wParam, const LPARAM lParam, const double time)
{
	back.emplace_back(message, wParam, lParam, time);
}


Buffer& Buffer::swap()
{
	// TODO: to make this class thread-safe, ensure no access to buffer during execution of this function.
	// This may need creating a new type for reference-counted iterators, as return value to begin()/end(),
	// or maybe vector<> itself can invalidate iterators?

	front.swap(back);
	back.clear();

	return *this;
}
