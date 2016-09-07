
#pragma once

#include "../renderer.h"


namespace crib
{
	namespace graphics
	{

		class d3d11_context;

		class d3d11_renderer : public renderer
		{
		public:
			d3d11_renderer(d3d11_context& context) : ctx(context) {}

			virtual ~d3d11_renderer() {}

		protected:
			d3d11_context& ctx;
		};

	}
}
