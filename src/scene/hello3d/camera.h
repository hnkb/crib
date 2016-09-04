
#pragma once

#include "input/event.h"
#include <Windows.h>
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
				const std::wstring print_params() const;

			protected:
				float radius = 6.f;
				float theta = 0.f;
				float phi = 1.5f;

				static constexpr float min_radius = 4.f;
				static constexpr float min_phi = .3f;
				static constexpr float max_phi = 2.f;

				// for handling dragging camera with mouse
				POINT origin_cursor;
				float origin_theta;
				float origin_phi;
			};

		}
	}
}
