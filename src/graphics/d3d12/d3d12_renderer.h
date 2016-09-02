
#pragma once

#include "graphics/renderer.h"


namespace crib
{
	namespace graphics
	{

		class d3d12_context;

		class d3d12_renderer : public renderer
		{
		public:
			d3d12_renderer(d3d12_context& context) : ctx(context) {}

			virtual ~d3d12_renderer() {}

		protected:
			d3d12_context& ctx;
		};

	}
}
