
#include "stdafx.h"
#include "hello_d3d11_renderer.h"
#include "hello_scene.h"
#include "shaders/pixel_fxc.h"
#include "shaders/vertex_fxc.h"
#include <strsafe.h>

using crib_scenes::hello::hello_d3d11_renderer;
using crib::core::utility::throw_if_failed;


hello_d3d11_renderer::hello_d3d11_renderer(crib::graphics::dx11::context& context, crib_scenes::hello::hello_scene& hello_scene) : renderer(context), scene(hello_scene)
{
	// Set up pipeline
	{
		throw_if_failed(ctx.device->CreateVertexShader(hello_vs_bytecode, sizeof(hello_vs_bytecode), nullptr, &vs), "Create vertex shader");
		throw_if_failed(ctx.device->CreatePixelShader(hello_ps_bytecode, sizeof(hello_ps_bytecode), nullptr, &ps), "Create pixel shader");

		ctx.context3d->VSSetShader(vs, nullptr, 0);
		ctx.context3d->PSSetShader(ps, nullptr, 0);


		D3D11_INPUT_ELEMENT_DESC ld[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CComPtr< ID3D11InputLayout> layout;
		throw_if_failed(ctx.device->CreateInputLayout(ld, _countof(ld), hello_vs_bytecode, sizeof(hello_vs_bytecode), &layout), "Register vertex layout");
		ctx.context3d->IASetInputLayout(layout);
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

	// D2D objects
	{
		throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 64.f, L"", &tf_value));
		throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.f, L"", &tf_title));
		tf_value->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		tf_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

		throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
	}
}


void hello_d3d11_renderer::resize(const float w, const float h)
{
	height = h, width = w;
	const float least = std::fminf(width, height);
	ctx.context3d->RSSetViewports(1, &CD3D11_VIEWPORT((width - least) * .5f, (height - least) * .5f, least, least));
}


void hello_d3d11_renderer::render()
{
	ctx.clear((FLOAT*)&DirectX::XMFLOAT4(std::fabsf(std::sinf(float(scene.time))) * .4f, .2f, .4f, 1.f));

	// Draw with D3D
	{
		UINT stride = sizeof(scene.vertex_data[0]);
		UINT offset = 0;
		ctx.context3d->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		ctx.context3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx.context3d->Draw(3, 0);
	}

	// Draw stats with D2D
	{
		ctx.context2d->BeginDraw();

		draw_stat(L"average fps", std::to_wstring(int(std::round(scene.frames / scene.time))), 40);

		wchar_t buffer[16];
		double minutes = std::floor(scene.time / 60.);
		StringCchPrintfW(buffer, 16, L"%d:%02d", int(minutes), int(std::floor(scene.time - minutes * 60.)));
		draw_stat(L"running time", buffer, 160);

		draw_stat(L"input buffer", std::to_wstring(scene.buffer_size), 280);

		throw_if_failed(ctx.context2d->EndDraw());
	}
}

void hello_d3d11_renderer::draw_stat(std::wstring title, std::wstring value, float top)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - 200, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
