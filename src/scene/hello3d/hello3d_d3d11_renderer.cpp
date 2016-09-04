
#include "stdafx.h"
#include "hello3d_d3d11_renderer.h"
#include "hello3d_scene.h"
#include "graphics/d3d11/d3d11_context.h"
#include "shaders/pixel_fxc.h"
#include "shaders/vertex_fxc.h"

using crib::scene::hello3d::hello3d_d3d11_renderer;
using crib::core::utility::throw_if_failed;


hello3d_d3d11_renderer::hello3d_d3d11_renderer(crib::graphics::d3d11_context& context, crib::scene::hello3d::hello3d_scene& hello_scene) : d3d11_renderer(context), scene(hello_scene)
{
	// Set up pipeline
	{
		CComPtr<ID3D11VertexShader> vs;
		CComPtr<ID3D11PixelShader> ps;
		CComPtr<ID3D11InputLayout> layout;


		throw_if_failed(ctx.device->CreateVertexShader(hello3d_vs_bytecode, sizeof(hello3d_vs_bytecode), nullptr, &vs), "Create vertex shader");
		throw_if_failed(ctx.device->CreatePixelShader(hello3d_ps_bytecode, sizeof(hello3d_ps_bytecode), nullptr, &ps), "Create pixel shader");


		D3D11_INPUT_ELEMENT_DESC ld[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		throw_if_failed(ctx.device->CreateInputLayout(ld, _countof(ld), hello3d_vs_bytecode, sizeof(hello3d_vs_bytecode), &layout), "Register vertex layout");
		
		
		// if there are more than one set of pipeline states (shaders), these calls must be made during drawing
		ctx.context3d->VSSetShader(vs, nullptr, 0);
		ctx.context3d->PSSetShader(ps, nullptr, 0);
		ctx.context3d->IASetInputLayout(layout);
	}

	// Create buffers
	{
		CComPtr<ID3D11Buffer> vertex_buffer;
		CComPtr<ID3D11Buffer> index_buffer;
		
		
		throw_if_failed(ctx.device->CreateBuffer(&CD3D11_BUFFER_DESC(sizeof(constant_buffer_layout), D3D11_BIND_CONSTANT_BUFFER), nullptr, &const_buffer), "Create constant buffer");


		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = UINT(sizeof(scene.vertex_data[0]) * scene.vertex_data.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = scene.vertex_data.data();

		throw_if_failed(ctx.device->CreateBuffer(&bufferDesc, &initData, &vertex_buffer), "Create vertex buffer");


		bufferDesc.ByteWidth = UINT(sizeof(scene.index_data[0]) * scene.index_data.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		initData.pSysMem = scene.index_data.data();

		throw_if_failed(ctx.device->CreateBuffer(&bufferDesc, &initData, &index_buffer), "Create index buffer");


		// if there are more than one set of buffers, these calls must be made during drawing
		UINT stride = sizeof(scene.vertex_data[0]);
		UINT offset = 0;
		ctx.context3d->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
		ctx.context3d->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R16_UINT, 0);

		ctx.context3d->VSSetConstantBuffers(0, 1, &const_buffer);

		ctx.context3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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


void hello3d_d3d11_renderer::resize(const float w, const float h)
{
	height = h, width = w;
}


void hello3d_d3d11_renderer::render()
{
	constant_buffer_layout constants;


	// Clear background
	{
		const float clear[] = { 0.f, .2f, .4f, 1.f };
		ctx.clear(clear);
	}

	// Update camera
	{
		auto focus = DirectX::XMVectorSet(0, 0, 0, 1.f);
		auto up = DirectX::XMVectorSet(0, 1.f, 0, 1.f);
		auto eye = DirectX::XMVectorSet(4.f, 0, 4.f, 1.f);
		auto view = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, focus, up));

		auto projection = DirectX::XMMatrixPerspectiveFovRH(.5f, width / height, 1.f, 100.f);

		constants.camera = projection * view;
	}

	// Texture
	{
		//ctx.context3d->PSSetShaderResources(0, 1, m_textureSRV);
		//ctx.context3d->PSSetSamplers(0, 1, m_sampler);
	}

	// Draw first cube
	{
		constants.model = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(1.f, 0, 0)) * DirectX::XMMatrixRotationX(float(scene.time));
		ctx.context3d->UpdateSubresource(const_buffer, 0, nullptr, &constants, 0, 0);

		ctx.context3d->DrawIndexed(UINT(scene.index_data.size()), 0, 0);
	}

	// Draw second cube
	{
		constants.model = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-1.f, 0, 0)) * DirectX::XMMatrixRotationY(float(scene.time));
		ctx.context3d->UpdateSubresource(const_buffer, 0, nullptr, &constants, 0, 0);

		ctx.context3d->DrawIndexed(UINT(scene.index_data.size()), 0, 0);
	}


	// Draw stats with D2D
	{
		ctx.context2d->BeginDraw();

		draw_stat(L"average fps", std::to_wstring(int(std::round(scene.frames / scene.time))), 40);

		wchar_t msg[100];
		double minutes = std::floor(scene.time / 60.);
		wsprintfW(msg, L"%d:%02d", int(minutes), int(std::floor(scene.time - minutes * 60.)));
		draw_stat(L"running time", msg, 160);

		draw_stat(L"input buffer", std::to_wstring(scene.buffer_size), 280);

		throw_if_failed(ctx.context2d->EndDraw());
	}
}

void hello3d_d3d11_renderer::draw_stat(std::wstring title, std::wstring value, float top)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - 200, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
