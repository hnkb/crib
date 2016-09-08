
#pragma once

#include "crib.h"


namespace crib_scenes
{
	namespace house
	{

		class scene : public crib::scene::scene_3d
		{
		public:
			scene(crib::core::settings& settings);

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

		protected:
			double time = 0;
		};

	}
}
