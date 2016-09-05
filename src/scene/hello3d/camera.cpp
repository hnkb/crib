
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

	return XMMatrixLookAtRH(eye, focus, up);
}


void camera::update(const float delta, const crib::input::event& e)
{
	switch (e.message)
	{
	case WM_MOUSEWHEEL:
		radius = std::max(min_radius, radius + float(GET_WHEEL_DELTA_WPARAM(e.wParam)) * -.15f * delta);
		break;

	case WM_MBUTTONDOWN:
		// Assuming window captures all mouse-down events
		// If not, call SetCapture() here, and ReleaseCapture() in WM_MBUTTONUP
		GetCursorPos(&origin_cursor);
		origin_phi = phi, origin_theta = theta;
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		break;

	case WM_MOUSEMOVE:
		if (e.wParam & MK_MBUTTON)
		{
			POINT current;
			GetCursorPos(&current);

			phi = std::min(max_phi, std::max(min_phi, origin_phi + (origin_cursor.y - current.y) / 200.0f));
			theta = origin_theta - (origin_cursor.x - current.x) / 200.0f;

			// Cursor won't change as long as mouse capture is set, so no need for this,
			// unless on rare occasions when capture is lost unexpectedly (ALT+TAB)
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		}
		break;

	case WM_KEYDOWN:
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
		break;
	}
}


const std::wstring camera::print_params() const
{
	wchar_t buffer[24];
	StringCchPrintfW(buffer, 24, L"r=%.1f \u03c6=%.1f\u03c0 \u03b8=%.1f\u03c0", radius, phi / XM_PI, theta / XM_PI);
	return buffer;
}
