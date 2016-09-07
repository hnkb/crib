
#pragma once

#include "crib.h"


namespace crib_scenes
{
	namespace hello
	{

		class hello_scene;

		class hello_d3d12_renderer : public crib::graphics::d3d12_renderer
		{
		public:
			hello_d3d12_renderer(crib::graphics::d3d12_context& context, hello_scene& hello_scene);

			virtual void render() override;

		protected:
			hello_scene& scene;
		};

	}
}
