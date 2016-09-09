
#include "stdafx.h"
#include "house_scene.h"

using crib_scenes::house::scene;


void scene::add_assets()
{
	using crib::geometry::definition;

	definition::assets.emplace(L"earth", definition(
		// vertices
		{
			{ -200.f, 0.f, -200.f },
			{  200.f, 0.f, -200.f },
			{  200.f, 0.f,  200.f },
			{ -200.f, 0.f,  200.f },
		},
		// colors
		{
			{ 0, 1, 0, 1 }
		},
		// faces
		{
			{ 0, 1, 2, 3 },
		}
	));
}
