
#include <Crib/Graphics.h>

using Crib::Graphics::DrawableBase;


std::map<std::wstring, DrawableBase> DrawableBase::assets({
	{ L"pyramid", DrawableBase(
		// vertices
		{
			{   0 ,  .5f,   0  },
			{ -.5f, -.5f, -.5f },
			{  .5f, -.5f, -.5f },
			{ -.5f, -.5f,  .5f },
			{  .5f, -.5f,  .5f }
		},
		// colors
		{
			{ 1, 0, 0, 1 }
		},
		// faces
		{
			{ 2, 1, 3, 4 },
			{ 0, 4, 3, 1 },
			{ 0, 1, 2 },
			{ 0, 3, 1 },
			{ 0, 2, 4 }
		})},

	{ L"cube", DrawableBase(
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
			{ 1, 0, 0, 1 }
		},
		// faces
		{
			{ 0, 1, 2, 3 },
			{ 7, 6, 5, 4 },
			{ 1, 0, 4, 5 },
			{ 3, 2, 6, 7 },
			{ 0, 3, 7, 4 },
			{ 2, 1, 5, 6 }
		})}
});
