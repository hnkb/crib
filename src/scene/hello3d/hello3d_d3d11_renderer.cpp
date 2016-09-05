
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
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		throw_if_failed(ctx.device->CreateInputLayout(ld, _countof(ld), hello3d_vs_bytecode, sizeof(hello3d_vs_bytecode), &layout), "Register vertex layout");
		
		
		// if there are more than one set of pipeline states (shaders), these calls must be made during drawing
		ctx.context3d->VSSetShader(vs, nullptr, 0);
		ctx.context3d->PSSetShader(ps, nullptr, 0);
		ctx.context3d->IASetInputLayout(layout);

		ctx.context3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	// Create buffers
	{
		throw_if_failed(ctx.device->CreateBuffer(&CD3D11_BUFFER_DESC(sizeof(pipeline::cb_vs_perobject_layout), D3D11_BIND_CONSTANT_BUFFER), nullptr, &cb_vs_perobject), "Create constant buffer");
		ctx.context3d->VSSetConstantBuffers(0, 1, &cb_vs_perobject);

		throw_if_failed(ctx.device->CreateBuffer(&CD3D11_BUFFER_DESC(sizeof(pipeline::cb_ps_perframe_layout), D3D11_BIND_CONSTANT_BUFFER), nullptr, &cb_ps_perframe), "Create constant buffer");
		ctx.context3d->PSSetConstantBuffers(0, 1, &cb_ps_perframe);

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

void hello3d_d3d11_renderer::resize(const float w, const float h)
{
	height = h, width = w;
}


void hello3d_d3d11_renderer::render()
{
	ctx.clear(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32);


	pipeline::cb_vs_perobject_layout vs_perobject;
	pipeline::cb_ps_perframe_layout ps_perframe;
	
	ps_perframe.light = scene.get_light();
	ctx.context3d->UpdateSubresource(cb_ps_perframe, 0, nullptr, &ps_perframe, 0, 0);

	for (const auto& obj : scene.get_objects())
	{
		vs_perobject.world = DirectX::XMMatrixTranspose(obj.world_transform);
		vs_perobject.wvp = DirectX::XMMatrixTranspose(obj.world_transform * scene.get_view_matrix() * get_projection_matrix());
		ctx.context3d->UpdateSubresource(cb_vs_perobject, 0, nullptr, &vs_perobject, 0, 0);

		draw_model(models[obj.model]);
	}

	
	// Draw 2D stats over 3D scene
	draw_stats();
}


void hello3d_d3d11_renderer::create_model_assets(const crib::scene::hello3d::model& model, model_assets& buffers)
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


DirectX::XMMATRIX hello3d_d3d11_renderer::get_projection_matrix() const
{
	return DirectX::XMMatrixPerspectiveFovRH(1.f, width / height, 1.f, 100.f);
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

	draw_stat(L"camera", scene.print_camera_params(), height - 160, 660);

	throw_if_failed(ctx.context2d->EndDraw());
}

void hello3d_d3d11_renderer::draw_stat(std::wstring title, std::wstring value, float top, float line_width)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - line_width, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
