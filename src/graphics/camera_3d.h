
#pragma once

#include <DirectXMath.h>


namespace crib
{
	namespace graphics
	{

		class camera_3d
		{
		public:
			virtual ~camera_3d() {}

			virtual void screen_resize(const float width, const float height) { screen_width = width, screen_height = height; }

			virtual const DirectX::XMMATRIX get_view_matrix() const;
			virtual const DirectX::XMMATRIX get_projection_matrix() const;

			DirectX::XMVECTOR look_at = DirectX::XMVectorSet(0, 0, 0, 1.f);
			DirectX::XMVECTOR up_direction = DirectX::XMVectorSet(0, 1.f, 0, 1.f);
			DirectX::XMVECTOR camera_position = DirectX::XMVectorSet(4.f, 4.f, 4.f, 1.f);

			float fov_angle = DirectX::XM_PIDIV2;
			float z_near = 1.f;
			float z_far = 100.f;

		protected:
			float screen_width;
			float screen_height;
		};

	}
}
