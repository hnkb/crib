
#pragma once

#include "crib.h"


namespace crib_scenes
{
	namespace house
	{

		class scene;


		class renderer : public crib::graphics::dx11::renderer
		{
		public:
			renderer(crib::graphics::dx11::context& context, scene& scene);

			virtual void render() override;

		protected:
			scene& scene;
		};

	}
}
