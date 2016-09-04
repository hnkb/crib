
#pragma once

#include "input/event.h"
#include <DirectXMath.h>


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class camera
			{
			public:
				DirectX::XMMATRIX get_view_matrix() const;
				void update(const float delta, const input::event& input);

				float radius = 6.f;
				float theta = 0.f;
				float phi = 1.5f;
			};

		}
	}
}
