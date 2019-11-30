
#pragma once

#include "crib.h"


namespace crib_scenes
{
	namespace hello
	{

		class hello_scene;

		class hello_d3d12_renderer : public crib::graphics::dx12::renderer
		{
		public:
			hello_d3d12_renderer(crib::graphics::dx12::context& context, hello_scene& hello_scene);

			virtual void render() override;

		protected:
			hello_scene& scene;
		};

	}
}
