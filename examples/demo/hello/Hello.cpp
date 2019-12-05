
#include "Hello.h"
#include <Crib/Window.h>
#include "hello_vs_fxc.h"
#include "hello_ps_fxc.h"
#include <algorithm>
#include <cmath>
#include <strsafe.h>

using CribDemo::Hello::Renderer;
using CribDemo::Hello::Scene;
using Crib::Platform::Windows::ThrowOnFail;


Scene::Scene()
	: vertexData{
		DirectX::XMFLOAT3(.0f,  .5f, .5f), DirectX::XMFLOAT4(0.f, 1.f, 1.f, 1.f),
		DirectX::XMFLOAT3(.5f, -.5f, .5f), DirectX::XMFLOAT4(.2f, .0f, .2f, 1.f),
		DirectX::XMFLOAT3(-.5f, -.5f, .5f), DirectX::XMFLOAT4(.5f, .5f, .0f, 1.f),
	}
{

}

std::wstring Scene::update(const double delta, const Crib::Input::Buffer& input)
{
	time += delta;
	frames += 1.;
	bufferSize = std::max(bufferSize, size_t(input.end() - input.begin()));

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	return L"";
}


Renderer::Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene) : Crib::Graphics::D3D11::Renderer(context), scene(scene)
{
	// Set up pipeline
	{
		ThrowOnFail(ctx.device->CreateVertexShader(hello_vs, sizeof(hello_vs), nullptr, &vs), "Create vertex shader");
		ThrowOnFail(ctx.device->CreatePixelShader(hello_ps, sizeof(hello_ps), nullptr, &ps), "Create pixel shader");

		ctx.context3d->VSSetShader(vs, nullptr, 0);
		ctx.context3d->PSSetShader(ps, nullptr, 0);


		D3D11_INPUT_ELEMENT_DESC ld[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CComPtr< ID3D11InputLayout> layout;
		ThrowOnFail(ctx.device->CreateInputLayout(ld, _countof(ld), hello_vs, sizeof(hello_vs), &layout), "Register vertex layout");
		ctx.context3d->IASetInputLayout(layout);
	}

	// Create vertex buffer
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(scene.vertexData);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = scene.vertexData;

		ThrowOnFail(ctx.device->CreateBuffer(&bufferDesc, &initData, &vb), "Create vertex buffer");
	}

	// D2D objects
	{
		ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 64.f, L"", &tf_value));
		ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.f, L"", &tf_title));
		tf_value->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		tf_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

		ThrowOnFail(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
	}
}


void Renderer::resize(const float w, const float h)
{
	height = h, width = w;
	const float least = std::min(width, height);
	ctx.context3d->RSSetViewports(1, &CD3D11_VIEWPORT((width - least) * .5f, (height - least) * .5f, least, least));
}


void Renderer::render()
{
	ctx.clear((FLOAT*)&DirectX::XMFLOAT4(std::fabs(std::sin(float(scene.time))) * .4f, .2f, .4f, 1.f));

	// Draw with D3D
	{
		UINT stride = sizeof(scene.vertexData[0]);
		UINT offset = 0;
		ctx.context3d->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		ctx.context3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx.context3d->Draw(3, 0);
	}

	// Draw stats with D2D
	{
		ctx.context2d->BeginDraw();

		drawStats(L"average fps", std::to_wstring(int(std::round(scene.frames / scene.time))), 40);

		wchar_t buffer[16];
		double minutes = std::floor(scene.time / 60.);
		StringCchPrintfW(buffer, 16, L"%d:%02d", int(minutes), int(std::floor(scene.time - minutes * 60.)));
		drawStats(L"running time", buffer, 160);

		drawStats(L"input buffer", std::to_wstring(scene.bufferSize), 280);

		ThrowOnFail(ctx.context2d->EndDraw());
	}
}

void Renderer::drawStats(std::wstring title, std::wstring value, float top)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - 200, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
