
#include "stdafx.h"
#include "camera_control_first_person.h"
#include <algorithm>

using crib::input::camera_control_first_person;


camera_control_first_person::camera_control_first_person(crib::graphics::camera_3d& camera) : camera(camera)
{
	RECT r;
	GetWindowRect(GetForegroundWindow(), &r);
	
	SetCursorPos(center_x = r.left + (r.right - r.left) / 2, center_y = r.top + (r.bottom - r.top) / 2);
	ClipCursor(&r);

	update_camera();
}

camera_control_first_person::~camera_control_first_person()
{
	ClipCursor(nullptr);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}


void camera_control_first_person::update_camera(const DirectX::XMVECTOR& movement)
{
	// In 1st-person camera, eye is fixed (to player position), focus rotates around it.
	// Note: phi is different from math convention, instead of rotating around Z it rotates around Y

	const auto unit = DirectX::XMVectorSet(0, 0, 1.f, 1.f); // for left-hand, use -1
	const auto roty = DirectX::XMMatrixRotationY(theta);

	camera.position = DirectX::XMVectorAdd(camera.position, DirectX::XMVector4Transform(movement, roty));
	camera.look_at = DirectX::XMVectorAdd(camera.position, DirectX::XMVector4Transform(unit, DirectX::XMMatrixRotationX(phi) * roty));
}


void camera_control_first_person::update(const double delta, const crib::input::buffer& buffer)
{
	for (const auto& e : buffer)
	{
		if (e.message == WM_MOUSEMOVE)
		{
			POINT p;
			GetCursorPos(&p);

			phi = std::min(max_phi, std::max(min_phi, phi - float(center_y - p.y) / 300.0f));
			theta += float(center_x - p.x) / 300.f;

			SetCursorPos(center_x, center_y);
			SetCursor(nullptr);
		}
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		theta += float(delta);
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		theta -= float(delta);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		phi = std::min(max_phi, phi - float(delta));
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		phi = std::max(min_phi, phi + float(delta));


	auto d = DirectX::XMVectorSet(
		GetAsyncKeyState('A') & 0x8000 ? 1.f : (GetAsyncKeyState('D') & 0x8000 ? -1.f : 0), 0,
		GetAsyncKeyState('W') & 0x8000 ? 1.f : (GetAsyncKeyState('S') & 0x8000 ? -1.f : 0), 1.f);


	update_camera(DirectX::XMVectorScale(d, 10.f * float(delta)));
}
