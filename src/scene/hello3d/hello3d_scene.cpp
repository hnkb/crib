
#include "stdafx.h"
#include "hello3d_scene.h"

using crib::scene::hello3d::hello3d_scene;


hello3d_scene::hello3d_scene()
{
	models.emplace(std::make_pair(L"cube", mesh::cube()));
	models.emplace(std::make_pair(L"pyramid", mesh::pyramid()));

	objects.emplace_back(L"cube0", L"cube");
	objects.emplace_back(L"cube1", L"cube");
	objects.emplace_back(L"pyramid0", L"pyramid");

	light.direction = DirectX::XMFLOAT3(DirectX::XMVector3Normalize(DirectX::XMVectorSet(.2f, 1.f, .5f, 0.f)).m128_f32);
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

		camera.update(float(delta), e);
	}

	return L"";
}


const DirectX::XMMATRIX hello3d_scene::get_projection_matrix() const
{
	return DirectX::XMMatrixPerspectiveFovRH(fov, width / height, 1.f, 100.f);
}


void hello3d_scene::hit_testing(const float x, const float y)
{
	auto ray = screen_to_world(x, y);

	// If we draw a line from camera location in the direction of `ray`, any triangle intersecting with it is a hit!
	// Instead of searching every vector triangle, we first test each object's bounding sphere to find candidates.

	hit_test_result = L"";
	auto cam_pos = camera.get_position();

	for (const auto& o : objects)
	{
		// Object's center in world space
		const auto center = DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), o.world_transform);

		const auto distance_from_cam_to_object_center = DirectX::XMVector3Length(DirectX::XMVectorSubtract(center, cam_pos));
		const auto same_distance_in_ray_direction = DirectX::XMVectorAdd(cam_pos, DirectX::XMVectorMultiply(ray, distance_from_cam_to_object_center));
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

const DirectX::XMVECTOR hello3d_scene::screen_to_world(const float x, const float y) const
{
	// convert to view space

	// In view space, x and y are between -1 to +1 (with axes parallel to screen), and z is the distance
	// from camera to the plane image is on (z axis orthogonal to screen). To calculate x,y we convert
	// from [0,height) and [0,width) to [-1,+1). We can use simple trig. to find z:
	//
	//  -1         +1                                       1
	//   -----------   (x/y axis)         sin(fov/2) = ------------
	//   \    |    /                                    hypotenuse
	//    \   |z  /                                                    ==>   z = cotan(fov/2)
	//     \  |  /                                          z
	//      \ | /                         cos(fov/2) = ------------
	//       \|/                                        hypotenuse
	//       fov
	//

	auto ray = DirectX::XMVector3Normalize(DirectX::XMVectorSet(
		2.f * x / width - 1.f,
		1.f - 2.f * y / height,
		-1.f / std::tanf(fov / 2.f), // for left-hand, remove minus sign
		0));

	if (0)
	{
		// Or, we could use projection matrix, as this is the reverse of the transformation it does.
		// This may be easier to calculate (two divisions instead of a tan) if Mproj is cached.

		auto Mproj = get_projection_matrix();
		auto ray = DirectX::XMVector3Normalize(DirectX::XMVectorSet(
			(2.f * x / width - 1.f) / Mproj.r[0].m128_f32[0],
			(1.f - 2.f * y / height) / Mproj.r[1].m128_f32[1],
			-1, // for left-hand, use 1
			0));
	}

	// convert to world space
	return DirectX::XMVector3TransformNormal(ray, DirectX::XMMatrixInverse(nullptr, camera.get_view_matrix()));
}
