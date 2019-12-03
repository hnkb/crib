
#include <Crib/Graphics/Direct3D12.h>
#include "hello_d3d12_renderer.h"
#include "hello_scene.h"
#include <cmath>

using crib_scenes::hello::hello_d3d12_renderer;


hello_d3d12_renderer::hello_d3d12_renderer(crib::graphics::dx12::context& context, crib_scenes::hello::hello_scene& hello_scene) : renderer(context), scene(hello_scene)
{

}

void hello_d3d12_renderer::render()
{
	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f- std::fabs(std::sin(float(scene.time))) * .4f, 1.0f };
	ctx.cmdlist->ClearRenderTargetView(ctx.rtvhandle, clearColor, 0, nullptr);
}
