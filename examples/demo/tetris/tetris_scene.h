
#pragma once

#include "Tetris.h"
#include <Crib/Graphics.h>


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
			virtual crib::graphics::dx11::renderer* create_renderer(crib::graphics::dx11::context& context) override { return new renderer(context, *this); }

		};

	}
}
