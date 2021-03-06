
#pragma once

#include "scene.h"
#include "entity.h"
#include "../graphics/camera_3d.h"
#include "../graphics/light.h"
#include "../core/settings.h"
#include <vector>


namespace crib
{
	namespace scene
	{

		class scene_3d : public scene
		{
		public:
			virtual ~scene_3d() {}

			virtual void screen_resize(const float width, const float height) override { camera.screen_resize(width, height); }

			const std::vector<entity> get_entities() const { return entities; }

			const DirectX::XMMATRIX get_view_matrix() const { return camera.get_view_matrix(); }
			const DirectX::XMMATRIX get_projection_matrix() const { return camera.get_projection_matrix(); }
			const graphics::directional_light& get_light() const { return light; }

		protected:
			scene_3d(crib::core::settings& settings) : settings(settings) { camera.fov_angle = settings.get(L"camera.fov", 1.f); }

			virtual crib::graphics::dx11::renderer* create_renderer(crib::graphics::dx11::context& context) override { return new crib::graphics::dx11::renderer_3d<scene_3d>(context, *this); }


			crib::core::settings& settings;

			std::vector<entity> entities;

			graphics::camera_3d camera;
			graphics::directional_light light;
		};

	}
}
