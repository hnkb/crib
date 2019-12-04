
#pragma once

#include "Strawman.h"
#include <Crib/Graphics.h>


namespace crib_scenes
{
	namespace strawman
	{

		class scene : public crib::scene::scene
		{
		public:
			scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			std::vector<D2D1_POINT_2F> leg;

		protected:
			virtual crib::graphics::base::renderer* create_custom_renderer(crib::graphics::base::context& context) override
			{
				return new renderer(dynamic_cast<crib::graphics::dx11::context&>(context), *this);
			}

			int active_leg = 0;
			static constexpr float max_dist = 50.f;
		};

	}
}
