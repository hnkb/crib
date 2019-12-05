
#include "FirstPerson.h"
#include <algorithm>

using Crib::Input::CameraControl::FirstPerson;


FirstPerson::FirstPerson(Crib::Graphics::Camera3D& camera) : camera(camera)
{
	RECT r;
	GetWindowRect(GetForegroundWindow(), &r);

	SetCursorPos(centerX = r.left + (r.right - r.left) / 2, centerY = r.top + (r.bottom - r.top) / 2);
	ClipCursor(&r);

	updateCamera();
}

FirstPerson::~FirstPerson()
{
	ClipCursor(nullptr);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}


void FirstPerson::updateCamera(const DirectX::XMVECTOR& movement)
{
	// In 1st-person camera, eye is fixed (to player position), focus rotates around it.
	// Note: phi is different from math convention, instead of rotating around Z it rotates around Y

	const auto unit = DirectX::XMVectorSet(0, 0, 1.f, 1.f); // for left-hand, use -1
	const auto rotY = DirectX::XMMatrixRotationY(theta);

	camera.position = DirectX::XMVectorAdd(camera.position, DirectX::XMVector4Transform(movement, rotY));
	camera.lookAt = DirectX::XMVectorAdd(camera.position, DirectX::XMVector4Transform(unit, DirectX::XMMatrixRotationX(phi) * rotY));
}


void FirstPerson::update(const double delta, const Buffer& buffer)
{
	for (const auto& e : buffer)
	{
		if (e.message == WM_MOUSEMOVE)
		{
			POINT p;
			GetCursorPos(&p);

			phi = std::min(maxPhi, std::max(minPhi, phi - float(centerY - p.y) / 300.0f));
			theta += float(centerX - p.x) / 300.f;

			SetCursorPos(centerX, centerY);
			SetCursor(nullptr);
		}
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		theta += float(delta);
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		theta -= float(delta);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		phi = std::min(maxPhi, phi - float(delta));
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		phi = std::max(minPhi, phi + float(delta));


	auto d = DirectX::XMVectorSet(
		GetAsyncKeyState('A') & 0x8000 ? 1.f : (GetAsyncKeyState('D') & 0x8000 ? -1.f : 0), 0,
		GetAsyncKeyState('W') & 0x8000 ? 1.f : (GetAsyncKeyState('S') & 0x8000 ? -1.f : 0), 1.f);


	updateCamera(DirectX::XMVectorScale(d, 10.f * float(delta)));
}
