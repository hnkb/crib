
#include "stdafx.h"
#include "camera_control_third_person.h"
#include <algorithm>

using crib::input::camera_control_third_person;


void camera_control_third_person::update(const double delta, const crib::input::buffer& buffer)
{
	for (const auto& evt : buffer)
		process_message(float(delta), evt);

	update_camera();
}

void camera_control_third_person::update_camera()
{
	// In 3rd-person, focus is fixed (to player position), eye rotates around it.

	// If phi is not clamped, we can calculate correct up vector like this:
	// up = XMVectorSet(0, (phi > 0 ? 1.f : -1.f) * (1.f - 2.f*float(abs(int(phi / XM_PI) % 2))), 0, 1.f);

	// Note: phi is different from math convention, instead of rotating around Z it rotates around Y

	const auto unit = DirectX::XMVectorSet(0, 0, -radius, 1.f); // for left-hand, use +radius
	camera.position = DirectX::XMVectorAdd(camera.look_at, DirectX::XMVector4Transform(unit, DirectX::XMMatrixRotationX(phi) * DirectX::XMMatrixRotationY(theta)));
}

void camera_control_third_person::process_message(const float delta, const crib::input::event& e)
{
	switch (e.message)
	{
	case WM_MOUSEWHEEL:
		radius = std::min(max_radius, std::max(min_radius, radius + float(GET_WHEEL_DELTA_WPARAM(e.wParam)) * -.15f * delta));
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

			phi = std::min(max_phi, std::max(min_phi, origin_phi - (origin_cursor.y - current.y) / 200.0f));
			theta = origin_theta + (origin_cursor.x - current.x) / 200.0f;

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
			theta -= 3.f * delta;
			break;

		case VK_NUMPAD6:
		case VK_RIGHT:
			theta += 3.f * delta;
			break;

		case VK_NUMPAD8:
		case VK_UP:
			phi = std::min(max_phi, phi + 3.f * delta);
			break;

		case VK_NUMPAD2:
		case VK_DOWN:
			phi = std::max(min_phi, phi - 3.f * delta);
			break;

		case VK_ADD:
			radius = std::max(min_radius, radius - 5.f * delta);
			break;

		case VK_SUBTRACT:
			radius = std::min(max_radius, radius + 5.f * delta);
			break;
		}
		break;
	}
}
