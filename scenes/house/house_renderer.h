
#pragma once

#include "crib.h"


namespace crib_scenes
{
	namespace house
	{

		class scene;


		class renderer : public crib::graphics::dx11::renderer_3d<scene>
		{
		public:
			renderer(crib::graphics::dx11::context& context, house::scene& scene);

			virtual void render() override;
		};

	}
}
