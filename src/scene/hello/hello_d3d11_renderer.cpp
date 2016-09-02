
#include "stdafx.h"
#include "hello_d3d11_renderer.h"
#include "hello_scene.h"
#include "graphics/d3d11/d3d11_context.h"

using crib::scene::hello_d3d11_renderer;


hello_d3d11_renderer::hello_d3d11_renderer(crib::graphics::d3d11_context& context, crib::scene::hello_scene& hello_scene) : d3d11_renderer(context), scene(hello_scene)
{

}

void hello_d3d11_renderer::render()
{
	ctx.context->ClearRenderTargetView(ctx.rtv, (FLOAT*)&DirectX::XMFLOAT4(std::fabsf(std::sinf(scene.time)) * .4f, .2f, .4f, 1.f));
	ctx.context->ClearDepthStencilView(ctx.dsv, D3D11_CLEAR_DEPTH, 1.f, 0);
}
