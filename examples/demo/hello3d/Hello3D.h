
#pragma once

#include <Crib/Graphics/Direct3D11.h>
#include <Crib/CameraControl.h>


namespace crib_scenes
{
	namespace hello3d
	{

		class stats
		{
		public:
			void update(const double delta, const crib::input::buffer& input)
			{
				time += delta;
				frames += 1.;
				buffer_size = std::max(buffer_size, size_t(input.end() - input.begin()));
			}

			const double avg_fps() const { return frames / time; }

			double time = 0;
			double frames = 0;
			size_t buffer_size = 0;
		};

		class mesh;
		class hello3d_scene;

		class hello3d_renderer_3d : public crib::graphics::dx11::renderer_3d<hello3d_scene>
		{
		public:
			hello3d_renderer_3d(crib::graphics::dx11::context& context, hello3d_scene& hello_scene);

			virtual void render() override;
			virtual void resize(const float w, const float h) override { renderer_3d::resize(width = w, height = h); }

		protected:
			void draw_stats();
			void draw_stat(std::wstring title, std::wstring value, float top, float line_width = 200);

			CComPtr<IDWriteTextFormat> tf_value;
			CComPtr<IDWriteTextFormat> tf_title;
			CComPtr<ID2D1SolidColorBrush> brush;
			float width;
			float height;
		};

		class hello3d_scene : public crib::scene::scene_3d
		{
		public:
			hello3d_scene(crib::core::settings& settings);

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			const std::wstring get_hit_test_result() const { return hit_test_result; }
			const stats& get_stats() const { return stats; }

		protected:
			virtual crib::graphics::base::renderer* create_custom_renderer(crib::graphics::base::context& context) override
			{
				return new hello3d_renderer_3d(dynamic_cast<crib::graphics::dx11::context&>(context), *this);
			}

			void load_assets();

			crib::input::camera_control_third_person camera_control;
			double time = 0;

			void hit_testing(const float x, const float y);
			std::wstring hit_test_result;
			stats stats;
		};

	}
}
