
#include "stdafx.h"
#include "camera.h"

using crib::scene::hello3d::camera;
using namespace DirectX;


XMMATRIX camera::get_view_matrix() const
{
	// in 1st-person camera, eye is fixed (to player position), focus rotates around it
	// in 3rd-person, focus is fixed (to player position), eye rotates around it

	// a simple non-rotating setup
	auto focus = XMVectorSet(0, 0, 0, 1.f);
	auto up = XMVectorSet(0, 1.f, 0, 1.f);
	//auto eye = XMVectorSet(radius, 0, radius, 1.f);

	// calculate 3rd-person camera position on a sphere around focus
	auto eye = XMVectorAdd(focus, XMVectorScale(XMVectorSet(XMScalarSin(phi)*XMScalarCos(theta), XMScalarCos(phi), XMScalarSin(phi)*XMScalarSin(theta), 1.f), radius));

	// if phi is not clamped to 0-PI, we can calculate correct up vector like this
	//auto up = XMVectorSet(0, (phi > 0 ? 1.f : -1.f) * (1.f - 2.f*float(abs(int(phi / XM_PI) % 2))), 0, 1.f);

	return XMMatrixTranspose(XMMatrixLookAtRH(eye, focus, up));
}


void camera::update(const float delta, const crib::input::event& e)
{
	if (e.message == WM_MOUSEWHEEL)
		radius = std::max(min_radius, radius + float(GET_WHEEL_DELTA_WPARAM(e.wParam)) * -.15f * delta);

	if (e.message == WM_KEYDOWN)
	{
		switch (e.wParam)
		{
		case VK_NUMPAD4:
		case VK_LEFT:
			theta += 3.f * delta;
			break;

		case VK_NUMPAD6:
		case VK_RIGHT:
			theta -= 3.f * delta;
			break;

		case VK_NUMPAD8:
		case VK_UP:
			phi = std::max(min_phi, phi - 3.f * delta);
			break;

		case VK_NUMPAD2:
		case VK_DOWN:
			phi = std::min(max_phi, phi + 3.f * delta);
			break;

		case VK_ADD:
			radius = std::max(min_radius, radius - 5.f * delta);
			break;

		case VK_SUBTRACT:
			radius += 5.f * delta;
			break;
		}
	}
}
