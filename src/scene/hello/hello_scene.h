
#pragma once

#include "scene/scene.h"
#include "hello_d3d11_renderer.h"


namespace crib
{
	namespace scene
	{

		class hello_scene : public scene
		{
		public:
			hello_scene() {}

			virtual std::wstring update(const double delta, const input::buffer& input) override;

		protected:
			virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new hello_d3d11_renderer(context, *this); }

			float time = 0;

			friend hello_d3d11_renderer;
		};

	}
}
