
#pragma once

#include "crib.h"


namespace crib
{
	namespace scene
	{
		namespace hello
		{

			class hello_scene;

			class hello_d3d12_renderer : public graphics::d3d12_renderer
			{
			public:
				hello_d3d12_renderer(graphics::d3d12_context& context, hello_scene& hello_scene);

				virtual void render() override;

			protected:
				hello_scene& scene;
			};

		}
	}
}
