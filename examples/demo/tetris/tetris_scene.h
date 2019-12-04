
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
			virtual crib::graphics::base::renderer* create_custom_renderer(crib::graphics::base::context& context) override
			{
				return new renderer(dynamic_cast<crib::graphics::dx11::context&>(context), *this);
			}

		};

	}
}
