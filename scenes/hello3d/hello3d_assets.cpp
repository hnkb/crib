
#include "stdafx.h"
#include "hello3d_scene.h"


void crib_scenes::hello3d::hello3d_scene::load_assets()
{
	using crib::geometry::definition;

	definition::assets.emplace(L"white-cube", definition(
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
