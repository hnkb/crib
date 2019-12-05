
#pragma once

#include <Crib/Graphics.h>


namespace Crib::Graphics::D3D11
{

	class Context;

	class Renderer : public Graphics::Renderer
	{
	public:
		Renderer(Context& context) : ctx(context) {}

		virtual ~Renderer() {}

	protected:
		Context& ctx;
	};

}
