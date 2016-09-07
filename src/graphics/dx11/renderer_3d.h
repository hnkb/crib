
#pragma once

#include "renderer.h"


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{

			template <typename scene_type> class renderer_3d : public renderer
			{
			public:
				virtual ~renderer_3d() {}

				virtual void resize(const float width, const float height) { scene.screen_resize(width, height); }

			protected:
				renderer_3d(context& context, scene_type& scene) : renderer(context), scene(scene) {}

				scene_type& scene;
			};

		}
	}
}
