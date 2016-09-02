
#include "stdafx.h"
#include "hello_d3d11_renderer.h"
#include "hello_scene.h"
#include "graphics/d3d11/d3d11_context.h"
#include "shaders/pixel_fxc.h"
#include "shaders/vertex_fxc.h"

using crib::scene::hello_d3d11_renderer;
using crib::core::utility::throw_if_failed;


hello_d3d11_renderer::hello_d3d11_renderer(crib::graphics::d3d11_context& context, crib::scene::hello_scene& hello_scene) : d3d11_renderer(context), scene(hello_scene)
{
	// Set up pipeline
	{
		throw_if_failed(ctx.device->CreateVertexShader(hello_vs_bytecode, sizeof(hello_vs_bytecode), nullptr, &vs), "Create vertex shader");
		throw_if_failed(ctx.device->CreatePixelShader(hello_ps_bytecode, sizeof(hello_ps_bytecode), nullptr, &ps), "Create pixel shader");

		ctx.context->VSSetShader(vs, nullptr, 0);
		ctx.context->PSSetShader(ps, nullptr, 0);


		D3D11_INPUT_ELEMENT_DESC ld[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CComPtr< ID3D11InputLayout> layout;
		throw_if_failed(ctx.device->CreateInputLayout(ld, _countof(ld), hello_vs_bytecode, sizeof(hello_vs_bytecode), &layout), "Register vertex layout");
		ctx.context->IASetInputLayout(layout);
	}

	// Create vertex buffer
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(scene.vertex_data);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = scene.vertex_data;

		throw_if_failed(ctx.device->CreateBuffer(&bufferDesc, &initData, &vb), "Create vertex buffer");
	}
}


void hello_d3d11_renderer::resize(const float width, const float height)
{
	const float least = std::fminf(width, height);
	ctx.context->RSSetViewports(1, &CD3D11_VIEWPORT((width - least) * .5f, (height - least) * .5f, least, least));
}


void hello_d3d11_renderer::render()
{
	ctx.context->ClearRenderTargetView(ctx.rtv, (FLOAT*)&DirectX::XMFLOAT4(std::fabsf(std::sinf(scene.time)) * .4f, .2f, .4f, 1.f));
	ctx.context->ClearDepthStencilView(ctx.dsv, D3D11_CLEAR_DEPTH, 1.f, 0);

	{
		UINT stride = sizeof(scene.vertex_data[0]);
		UINT offset = 0;
		ctx.context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		ctx.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx.context->Draw(3, 0);
	}
}
