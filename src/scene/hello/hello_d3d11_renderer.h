
#pragma once


#include "graphics/d3d11/d3d11_renderer.h"


namespace crib
{
	namespace scene
	{

		class hello_scene;

		class hello_d3d11_renderer : public graphics::d3d11_renderer
		{
		public:
			hello_d3d11_renderer(graphics::d3d11_context& context, hello_scene& hello_scene);

			virtual void render() override;

		protected:
			hello_scene& scene;

			CComPtr<ID3D11VertexShader> vs;
			CComPtr<ID3D11PixelShader> ps;
			CComPtr<ID3D11Buffer> vb;
		};

	}
}
