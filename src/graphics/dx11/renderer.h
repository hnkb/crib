
#pragma once

#include <Crib/Graphics.h>


namespace crib
{
	namespace graphics
	{
		namespace dx11
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
