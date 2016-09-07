
#pragma once

#include "crib.h"
#include <string>
#include <DirectXMath.h>


namespace crib_scenes
{
	namespace hello3d
	{

		class camera
		{
		public:
			void update(const float delta, const crib::input::event& input);

			const DirectX::XMVECTOR get_focus() const;
			const DirectX::XMVECTOR get_up() const;
			const DirectX::XMVECTOR get_position() const;
			const DirectX::XMMATRIX get_view_matrix() const;

			const std::wstring print_params() const;

		protected:
			float radius = 4.f;
			float theta = .7f;
			float phi = 1.5f;

			static constexpr float min_radius = 3.f;
			static constexpr float max_radius = 99.f;
			static constexpr float min_phi = .3f;
			static constexpr float max_phi = 2.f;

			// for handling dragging camera with mouse
			POINT origin_cursor;
			float origin_theta;
			float origin_phi;
		};

	}
}
