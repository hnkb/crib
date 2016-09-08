
#pragma once

#include <string>
#include <DirectXMath.h>


namespace crib
{
	namespace scene
	{

		class entity
		{
		public:
			entity(const std::wstring& id, const std::wstring& mesh, const std::wstring& effect) : id(id), mesh(mesh), effect(effect), world_transform(DirectX::XMMatrixIdentity()) {}

			std::wstring id;
			
			std::wstring mesh;
			std::wstring effect;
			
			DirectX::XMMATRIX world_transform;

			//std::vector<entity> children;
		};

	}
}
