
#pragma once

#include "scene/scene.h"
#include "tetris_renderer.h"
#include <utility>


namespace crib_scenes
{
	namespace tetris
	{

		class scene : public crib::scene::scene
		{
		public:
			scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

		protected:
			virtual crib::graphics::d3d11_renderer* create_renderer(crib::graphics::d3d11_context& context) override { return new renderer(context, *this); }

		};

	}
}
