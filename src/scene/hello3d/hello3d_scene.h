
#pragma once

#include "scene/scene.h"
#include "hello3d_d3d11_renderer.h"
#include "model.h"
#include "object.h"
#include "camera.h"
#include "stats.h"


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class hello3d_scene : public scene
			{
			public:
				hello3d_scene();

				virtual std::wstring update(const double delta, const input::buffer& input) override;

				const std::map<std::wstring, model>& get_models() const { return models; }
				const std::vector<object>& get_objects() const { return objects; }
				DirectX::XMMATRIX get_view_matrix() const { return camera.get_view_matrix(); }
				const stats& get_stats() const { return stats; }

			protected:
				virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new hello3d_d3d11_renderer(context, *this); }

				std::map<std::wstring, model> models;
				std::vector<object> objects;
				camera camera;

				double time = 0;

				stats stats;
			};

		}
	}
}
