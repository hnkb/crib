
#pragma once

#include <string>
#include <DirectXMath.h>


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class object
			{
			public:
				object(const std::wstring& object_name, const std::wstring& model_name) : name(object_name), model(model_name), world_transform(DirectX::XMMatrixIdentity()) {}

				std::wstring name;
				std::wstring model;
				DirectX::XMMATRIX world_transform;
			};

		}
	}
}
