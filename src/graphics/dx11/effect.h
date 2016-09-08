
#pragma once

#include <string>
#include <atlbase.h>
#include <d3d11_2.h>


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{

			class effect
			{
			public:
				effect() {}
				effect(const std::wstring name, ID3D11Device* dev);

				void bind(ID3D11DeviceContext2* ctx);

			private:
				CComPtr<ID3D11VertexShader> vs;
				CComPtr<ID3D11PixelShader> ps;
				CComPtr<ID3D11InputLayout> layout;
			};

		}
	}
}
