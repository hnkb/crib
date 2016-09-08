
#include "stdafx.h"
#include "hello3d_d3d11_renderer.h"
#include "hello3d_scene.h"
#include "graphics/dx11/shaders/ps_basic_pcn_fxc.h"
#include "graphics/dx11/shaders/vs_basic_pcn_fxc.h"
#include <strsafe.h>

using crib_scenes::hello3d::hello3d_d3d11_renderer;
using crib::core::utility::throw_if_failed;


hello3d_d3d11_renderer::hello3d_d3d11_renderer(crib::graphics::dx11::context& context, crib_scenes::hello3d::hello3d_scene& scene) : renderer_3d(context, scene)
{
	// Set up pipeline
	{
		CComPtr<ID3D11VertexShader> vs;
		CComPtr<ID3D11PixelShader> ps;
		CComPtr<ID3D11InputLayout> layout;


		throw_if_failed(ctx.device->CreateVertexShader(vs_basic_pcn, sizeof(vs_basic_pcn), nullptr, &vs), "Create vertex shader");
		throw_if_failed(ctx.device->CreatePixelShader(ps_basic_pcn, sizeof(ps_basic_pcn), nullptr, &ps), "Create pixel shader");


		D3D11_INPUT_ELEMENT_DESC ld[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		throw_if_failed(ctx.device->CreateInputLayout(ld, _countof(ld), vs_basic_pcn, sizeof(vs_basic_pcn), &layout), "Register vertex layout");
		
		
		// if there are more than one set of pipeline states (shaders), these calls must be made during drawing
		ctx.context3d->VSSetShader(vs, nullptr, 0);
		ctx.context3d->PSSetShader(ps, nullptr, 0);
		ctx.context3d->IASetInputLayout(layout);

		ctx.context3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	// Create buffers
	{
		for (const auto& model : scene.get_models())
		{
			models.emplace(model.first, model_assets());
			create_model_assets(model.second, models[model.first]);
		}
	}

	// D2D objects (for stats display)
	{
		throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 64.f, L"", &tf_value));
		throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.f, L"", &tf_title));
		tf_value->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		tf_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

		throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
	}
}


void hello3d_d3d11_renderer::render()
{
	ctx.clear(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32);

	cb_frame.data.light = scene.get_light();
	cb_frame.update(ctx.context3d);

	const auto view_proj = scene.get_view_matrix() * scene.get_projection_matrix();

	for (const auto& e : scene.get_entities())
	{
		cb_object.data.world = DirectX::XMMatrixTranspose(e.world_transform);
		cb_object.data.world_view_proj = DirectX::XMMatrixTranspose(e.world_transform * view_proj);
		cb_object.update(ctx.context3d);

		draw_model(models[e.mesh]);
	}

	
	// Draw 2D stats over 3D scene
	draw_stats();
}


void hello3d_d3d11_renderer::create_model_assets(const crib_scenes::hello3d::mesh& model, model_assets& buffers)
{
	D3D11_SUBRESOURCE_DATA initData = {};

	initData.pSysMem = model.vertex_data();
	throw_if_failed(ctx.device->CreateBuffer(&CD3D11_BUFFER_DESC(model.vertex_size_bytes(), D3D11_BIND_VERTEX_BUFFER), &initData, &buffers.vertex), "Create vertex buffer");

	initData.pSysMem = model.index_data();
	throw_if_failed(ctx.device->CreateBuffer(&CD3D11_BUFFER_DESC(model.index_size_bytes(), D3D11_BIND_INDEX_BUFFER), &initData, &buffers.index), "Create index buffer");

	buffers.vertex_stride = model.vertex_stride();
	buffers.idx_count = model.index_count();
}

void hello3d_d3d11_renderer::draw_model(hello3d_d3d11_renderer::model_assets& model)
{
	UINT stride = model.vertex_stride;
	UINT offset = 0;
	ctx.context3d->IASetVertexBuffers(0, 1, &model.vertex, &stride, &offset);

	ctx.context3d->IASetIndexBuffer(model.index, DXGI_FORMAT_R16_UINT, 0);

	ctx.context3d->DrawIndexed(model.idx_count, 0, 0);
}


void hello3d_d3d11_renderer::draw_stats()
{
	ctx.context2d->BeginDraw();

	draw_stat(L"average fps", std::to_wstring(int(std::round(scene.get_stats().avg_fps()))), 40);

	wchar_t buffer[16];
	double minutes = std::floor(scene.get_stats().time / 60.);
	StringCchPrintfW(buffer, 16, L"%d:%02d", int(minutes), int(std::floor(scene.get_stats().time - minutes * 60.)));
	draw_stat(L"running time", buffer, 160);

	draw_stat(L"input buffer", std::to_wstring(scene.get_stats().buffer_size), 280);

	if (scene.get_hit_test_result().size()) draw_stat(L"hit test", scene.get_hit_test_result(), height - 160, 500);

	throw_if_failed(ctx.context2d->EndDraw());
}

void hello3d_d3d11_renderer::draw_stat(std::wstring title, std::wstring value, float top, float line_width)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - line_width, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
