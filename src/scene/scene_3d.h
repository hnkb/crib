
#pragma once

#include "scene.h"
#include "../graphics/camera_3d.h"
#include "../graphics/light.h"


namespace crib
{
	namespace scene
	{

		class scene_3d : public scene
		{
		public:
			virtual ~scene_3d() {}

			virtual void screen_resize(const float width, const float height) override { camera.screen_resize(width, height); }

			const DirectX::XMMATRIX get_view_matrix() const { return camera.get_view_matrix(); }
			const DirectX::XMMATRIX get_projection_matrix() const { return camera.get_projection_matrix(); }
			const graphics::directional_light& get_light() const { return light; }

		protected:
			graphics::camera_3d camera;
			graphics::directional_light light;
		};

	}
}
