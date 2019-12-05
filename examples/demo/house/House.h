
#pragma once

#include <Crib/Graphics.h>
#include <Crib/CameraControl.h>


namespace CribDemo::House
{

	class Scene : public Crib::Graphics::Scene
	{
	public:
		Scene(Crib::PersistentSettings& settings);

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

	protected:
		void addAssets();

		Crib::Input::CameraControl::FirstPerson camControl;
		double time = 0;
	};

}
