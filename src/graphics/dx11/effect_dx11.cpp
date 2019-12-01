
#include "stdafx.h"
#include "effect.h"
#include <Crib/Platform/Windows.h>
#include <map>
#include <vector>

using crib::graphics::dx11::effect;
using crib::core::utility::throw_if_failed;


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{
			namespace effects
			{
#include "ps_basic_pcn_fxc.h"
#include "vs_basic_pcn_fxc.h"


				const std::vector<std::vector<D3D11_INPUT_ELEMENT_DESC>> input_layout({
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
					const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout;

					_effect_info(const void* vs_bytecode, const size_t vs_length, const void* ps_bytecode, const size_t ps_length, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout)
						: vs_bytecode(vs_bytecode), vs_length(vs_length), ps_bytecode(ps_bytecode), ps_length(ps_length), input_layout(input_layout) {}
				};

				const std::map<std::wstring, _effect_info> effect_info({
					{ L"basic", _effect_info(vs_basic_pcn, sizeof(vs_basic_pcn), ps_basic_pcn, sizeof(ps_basic_pcn), input_layout[0]) }
				});
			}
		}
	}
}


effect::effect(const std::wstring name, ID3D11Device* dev)
{
	auto ptr = effects::effect_info.find(name);
	if (ptr == effects::effect_info.end()) throw new std::invalid_argument("Effect name is invalid");
	const auto& e = ptr->second;

	throw_if_failed(dev->CreateVertexShader(e.vs_bytecode, e.vs_length, nullptr, &vs), "Create vertex shader");
	throw_if_failed(dev->CreatePixelShader(e.ps_bytecode, e.ps_length, nullptr, &ps), "Create pixel shader");
	throw_if_failed(dev->CreateInputLayout(e.input_layout.data(), UINT(e.input_layout.size()), e.vs_bytecode, e.vs_length, &layout), "Register vertex layout");
}

void effect::bind(ID3D11DeviceContext2* ctx)
{
	ctx->VSSetShader(vs, nullptr, 0);
	ctx->PSSetShader(ps, nullptr, 0);
	ctx->IASetInputLayout(layout);
}
