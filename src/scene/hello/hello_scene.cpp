
#include "stdafx.h"
#include "hello_scene.h"

using crib::scene::hello_scene;


std::wstring hello_scene::update(const double delta, const crib::input::buffer& input)
{
	time += float(delta);

	for (auto& e : input)
	{
		if (e.message == WM_KEYDOWN && e.wParam == VK_ESCAPE)
			return L"quit";
	}

	return L"";
}
