
#pragma once

#include "../base/renderer.h"


namespace crib
{
	namespace graphics
	{
		namespace dx12
		{

			class context;

			class renderer : public base::renderer
			{
			public:
				renderer(context& context) : ctx(context) {}

				virtual ~renderer() {}

			protected:
				context& ctx;
			};

		}
	}
}
