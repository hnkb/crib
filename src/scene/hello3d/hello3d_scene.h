
#pragma once

#include "crib.h"
#include "hello3d_d3d11_renderer.h"
#include "mesh.h"
#include "object.h"
#include "camera.h"
#include "stats.h"


namespace crib_scenes
{
	namespace hello3d
	{

		class hello3d_scene : public crib::scene::scene
		{
		public:
			hello3d_scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			void resize(const float w, const float h) { height = h, width = w; }

			const std::map<std::wstring, mesh>& get_models() const { return models; }
			const std::vector<object>& get_objects() const { return objects; }
			const pipeline::directional_light& get_light() const { return light; }
			const DirectX::XMMATRIX get_view_matrix() const { return camera.get_view_matrix(); }
			const DirectX::XMMATRIX get_projection_matrix() const;

			const stats& get_stats() const { return stats; }
			const std::wstring print_camera_params() const { return camera.print_params(); }
			const std::wstring get_hit_test_result() const { return hit_test_result; }

		protected:
			virtual crib::graphics::d3d11_renderer* create_renderer(crib::graphics::d3d11_context& context) override { return new hello3d_d3d11_renderer(context, *this); }

			void hit_testing(const float x, const float y);
			const DirectX::XMVECTOR screen_to_world(const float x, const float y) const;

			std::map<std::wstring, mesh> models;
			std::vector<object> objects;
			camera camera;
			pipeline::directional_light light;

			float width;
			float height;
			float fov = 1.f;

			double time = 0;

			stats stats;
			std::wstring hit_test_result;
		};

	}
}
