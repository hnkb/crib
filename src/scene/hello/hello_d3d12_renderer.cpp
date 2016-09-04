
#include "stdafx.h"
#include "hello_d3d12_renderer.h"
#include "hello_scene.h"
#include "graphics/d3d12/d3d12_context.h"

using crib::scene::hello::hello_d3d12_renderer;


hello_d3d12_renderer::hello_d3d12_renderer(crib::graphics::d3d12_context& context, crib::scene::hello::hello_scene& hello_scene) : d3d12_renderer(context), scene(hello_scene)
{

}

void hello_d3d12_renderer::render()
{
	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f- std::fabsf(std::sinf(float(scene.time))) * .4f, 1.0f };
	ctx.cmdlist->ClearRenderTargetView(ctx.rtvhandle, clearColor, 0, nullptr);
}
