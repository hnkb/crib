
#include "stdafx.h"
#include "strawman_scene.h"

using crib_scenes::strawman::scene;


scene::scene() : leg({ D2D1::Point2F(100.f, 0),D2D1::Point2F(60.f, 0) })
{

}


std::wstring scene::update(const double delta, const crib::input::buffer& input)
{
	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if ((leg[active_leg].x - leg[1 - active_leg].x) >= max_dist)
			active_leg = 1 - active_leg;
		leg[active_leg].x += 75.f * float(delta);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if ((leg[active_leg].x - leg[1 - active_leg].x) <= -max_dist)
			active_leg = 1 - active_leg;
		leg[active_leg].x -= 75.f * float(delta);
	}

	return L"";
}
