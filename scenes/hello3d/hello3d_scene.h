
#pragma once

#include "crib.h"
#include "hello3d_d3d11_renderer.h"
#include "mesh.h"
#include "object.h"
#include "stats.h"


namespace crib_scenes
{
	namespace hello3d
	{

		class hello3d_scene : public crib::scene::scene_3d
		{
		public:
			hello3d_scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			const std::map<std::wstring, mesh>& get_models() const { return models; }
			const std::vector<object>& get_objects() const { return objects; }

			const stats& get_stats() const { return stats; }
			const std::wstring get_hit_test_result() const { return hit_test_result; }

		protected:
			virtual crib::graphics::dx11::renderer* create_renderer(crib::graphics::dx11::context& context) override { return new hello3d_d3d11_renderer(context, *this); }

			void hit_testing(const float x, const float y);

			std::map<std::wstring, mesh> models;
			std::vector<object> objects;
			crib::input::camera_control_third_person camera_control;

			double time = 0;

			stats stats;
			std::wstring hit_test_result;
		};

	}
}
