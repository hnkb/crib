
#include "Hello3D.h"

using CribDemo::Hello3D::Scene;


Scene::Scene(Crib::PersistentSettings& settings) : Crib::Graphics::Scene(settings), camControl(camera)
{
	loadAssets();

	entities.emplace_back(L"cube0", L"cube", L"basic");
	entities.emplace_back(L"cube1", L"white-cube", L"basic");
	entities.emplace_back(L"pyramid0", L"pyramid", L"basic");

	light.direction = DirectX::XMFLOAT3(DirectX::XMVector3Normalize(DirectX::XMVectorSet(-.2f, 1.f, -.5f, 0.f)).m128_f32);
	light.ambient = DirectX::XMFLOAT4(.2f, .1f, .1f, 1.f);
	light.diffuse = DirectX::XMFLOAT4(.6f, .7f, .7f, 1.f);
}


std::wstring Scene::update(const double delta, const Crib::Input::Buffer& input)
{
	time += delta;
	stats.update(delta, input);

	entities[0].worldTransform = DirectX::XMMatrixRotationX(float(time)) * DirectX::XMMatrixTranslation( 1.f, 0, 0);
	entities[1].worldTransform = DirectX::XMMatrixRotationY(float(time)) * DirectX::XMMatrixTranslation(-1.f, 0, 0);
	entities[2].worldTransform = DirectX::XMMatrixRotationZ(float(time)) * DirectX::XMMatrixTranslation(0, 0, 1.5f);

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";

		// Note: because object in the seen are animated and may move, even when mouse is stationary the result
		// of hit test might change. So, only calling hitTesting() on mouse move is not enough.
		if (e.message == WM_MOUSEMOVE)
			hitTesting(short(LOWORD(e.lParam)), short(HIWORD(e.lParam)));
	}

	camControl.update(delta, input);

	return L"";
}


void Scene::hitTesting(const float x, const float y)
{
	auto ray = camera.screenToWorld(x, y);

	// If we draw a line from camera location in the direction of `ray`, any triangle intersecting with it is a hit!
	// Instead of searching every vector triangle, we first test each object's bounding sphere to find candidates.

	hitTestResult = L"";

	for (const auto& e : entities)
	{
		// Object's center in world space
		const auto center = DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), e.worldTransform);

		const auto distance_from_cam_to_object_center = DirectX::XMVector3Length(DirectX::XMVectorSubtract(center, camera.position));
		const auto same_distance_in_ray_direction = DirectX::XMVectorAdd(camera.position, DirectX::XMVectorMultiply(ray, distance_from_cam_to_object_center));
		const auto distance_from_ray_to_object = DirectX::XMVector3Length(DirectX::XMVectorSubtract(center, same_distance_in_ray_direction));

		if (distance_from_ray_to_object.m128_f32[0] <= .75f)
		{
			// We assume bounding sphere radius is .75f (in current cube and pyramid models, it is, because furthest
			// vector from center (0,0,0) is something like (.5, .5, .5).

			// Objects who pass this test are candidates, because `ray` hits inside their bounding sphere. They are
			// not necessarily hits. We need to test if `ray` hits any of their triangles.
			// But this approximation is good enough for now.

			hitTestResult = hitTestResult + (hitTestResult.size() ? L", " : L"") + e.id;
		}
	}

}


void CribDemo::Hello3D::Scene::loadAssets()
{
	using Crib::Graphics::DrawableBase;

	DrawableBase::assets.emplace(L"white-cube", DrawableBase(
		// vertices
		{
			{ -.5f,  .5f, -.5f },
			{  .5f,  .5f, -.5f },
			{  .5f,  .5f,  .5f },
			{ -.5f,  .5f,  .5f },
			{ -.5f, -.5f, -.5f },
			{  .5f, -.5f, -.5f },
			{  .5f, -.5f,  .5f },
			{ -.5f, -.5f,  .5f }
		},
		// colors
		{
			{ 1, 1, 1, 1 }
		},
		// faces
		{
			{ 0, 1, 2, 3 },
			{ 7, 6, 5, 4 },
			{ 1, 0, 4, 5 },
			{ 3, 2, 6, 7 },
			{ 0, 3, 7, 4 },
			{ 2, 1, 5, 6 }
		}
		));
}
