
#include <Crib/Graphics/Direct3D11.h>
#include <map>
#include <vector>

using Crib::Graphics::D3D11::Effect;
using Crib::Platform::Windows::ThrowOnFail;


namespace Crib::Graphics::D3D11
{
	namespace Shaders
	{
#include "ps_basic_pcn_fxc.h"
#include "vs_basic_pcn_fxc.h"


		const std::vector<std::vector<D3D11_INPUT_ELEMENT_DESC>> inputLayout({
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			}
			});

		struct _effect_info
		{
			const void* vs_bytecode;
			const size_t vs_length;
			const void* ps_bytecode;
			const size_t ps_length;
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout;

			_effect_info(const void* vs_bytecode, const size_t vs_length, const void* ps_bytecode, const size_t ps_length, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout)
				: vs_bytecode(vs_bytecode), vs_length(vs_length), ps_bytecode(ps_bytecode), ps_length(ps_length), inputLayout(inputLayout) {}
		};

		const std::map<std::wstring, _effect_info> effect_info({
			{ L"basic", _effect_info(vs_basic_pcn, sizeof(vs_basic_pcn), ps_basic_pcn, sizeof(ps_basic_pcn), inputLayout[0]) }
			});
	}
}


Effect::Effect(const std::wstring name, ID3D11Device* dev)
{
	auto ptr = Shaders::effect_info.find(name);
	if (ptr == Shaders::effect_info.end()) throw new std::invalid_argument("Effect name is invalid");
	const auto& e = ptr->second;

	ThrowOnFail(dev->CreateVertexShader(e.vs_bytecode, e.vs_length, nullptr, &vs), "Create vertex shader");
	ThrowOnFail(dev->CreatePixelShader(e.ps_bytecode, e.ps_length, nullptr, &ps), "Create pixel shader");
	ThrowOnFail(dev->CreateInputLayout(e.inputLayout.data(), UINT(e.inputLayout.size()), e.vs_bytecode, e.vs_length, &layout), "Register vertex layout");
}

void Effect::bind(ID3D11DeviceContext2* ctx)
{
	ctx->VSSetShader(vs, nullptr, 0);
	ctx->PSSetShader(ps, nullptr, 0);
	ctx->IASetInputLayout(layout);
}
