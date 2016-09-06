
#include "stdafx.h"
#include "strawman_scene.h"

using crib_scenes::strawman::scene;


scene::scene()
{

}


std::wstring scene::update(const double delta, const crib::input::buffer& input)
{
	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	return L"";
}
