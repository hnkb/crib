
#pragma once

#include "house_renderer.h"


namespace crib_scenes
{
	namespace house
	{

		class scene : public crib::scene::scene_3d
		{
		public:
			scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

		protected:
			virtual crib::graphics::dx11::renderer* create_renderer(crib::graphics::dx11::context& context) override { return new renderer(context, *this); }

			double time = 0;
		};

	}
}