
#include "stdafx.h"
#include "d3d11_renderer.h"
#include "d3d11_context.h"

using crib::graphics::d3d11_renderer;


void d3d11_renderer::render()
{
	ctx.context->ClearRenderTargetView(ctx.rtv, (FLOAT*)&DirectX::XMFLOAT4(0.0f, 0.2f, 0.4f, 1.0f));
	ctx.context->ClearDepthStencilView(ctx.dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
