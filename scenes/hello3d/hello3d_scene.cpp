
#include "stdafx.h"
#include "hello3d_scene.h"

using crib_scenes::hello3d::hello3d_scene;


hello3d_scene::hello3d_scene() : camera_control(camera)
{
	models.emplace(std::make_pair(L"cube", mesh::cube()));
	models.emplace(std::make_pair(L"pyramid", mesh::pyramid()));

	objects.emplace_back(L"cube0", L"cube");
	objects.emplace_back(L"cube1", L"cube");
	objects.emplace_back(L"pyramid0", L"pyramid");

	light.direction = DirectX::XMFLOAT3(DirectX::XMVector3Normalize(DirectX::XMVectorSet(-.2f, 1.f, -.5f, 0.f)).m128_f32);
	light.ambient = DirectX::XMFLOAT4(.2f, .1f, .1f, 1.f);
	light.diffuse = DirectX::XMFLOAT4(.6f, .7f, .7f, 1.f);
}


std::wstring hello3d_scene::update(const double delta, const crib::input::buffer& input)
{
	time += delta;
	stats.update(delta, input);

	objects[0].world_transform = DirectX::XMMatrixRotationX(float(time)) * DirectX::XMMatrixTranslation( 1.f, 0, 0);
	objects[1].world_transform = DirectX::XMMatrixRotationY(float(time)) * DirectX::XMMatrixTranslation(-1.f, 0, 0);
	objects[2].world_transform = DirectX::XMMatrixRotationZ(float(time)) * DirectX::XMMatrixTranslation(0, 0, 1.5f);

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";

		// Note: because object in the seen are animated and may move, even when mouse is stationary the result
		// of hit test might change. So, only calling hit_testing on mouse move is not enough.
		if (e.message == WM_MOUSEMOVE)
			hit_testing(short(LOWORD(e.lParam)), short(HIWORD(e.lParam)));
	}

	camera_control.update(delta, input);

	return L"";
}


void hello3d_scene::hit_testing(const float x, const float y)
{
	auto ray = camera.screen_to_world(x, y);

	// If we draw a line from camera location in the direction of `ray`, any triangle intersecting with it is a hit!
	// Instead of searching every vector triangle, we first test each object's bounding sphere to find candidates.

	hit_test_result = L"";

	for (const auto& o : objects)
	{
		// Object's center in world space
		const auto center = DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), o.world_transform);

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

			hit_test_result = hit_test_result + (hit_test_result.size() ? L", " : L"") + o.name;
		}
	}

}
