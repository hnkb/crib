
#pragma once

#include "crib.h"
#include "hello3d_renderer_3d.h"
#include "stats.h"


namespace crib_scenes
{
	namespace hello3d
	{

		class hello3d_scene : public crib::scene::scene_3d
		{
		public:
			hello3d_scene(crib::core::settings& settings);

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			const std::wstring get_hit_test_result() const { return hit_test_result; }
			const stats& get_stats() const { return stats; }

		protected:
			virtual crib::graphics::dx11::renderer* create_renderer(crib::graphics::dx11::context& context) override { return new hello3d_renderer_3d(context, *this); }

			crib::input::camera_control_third_person camera_control;
			double time = 0;

			void hit_testing(const float x, const float y);
			std::wstring hit_test_result;
			stats stats;
		};

	}
}
