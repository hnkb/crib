
#pragma once

#include "MenuItem.h"
#include "MenuRender.h"
#include <Crib/Window.h>


namespace CribDemo::Menu
{

	class Scene : public Crib::Graphics::SceneBase
	{
	public:
		Scene(Crib::PersistentSettings& setting);

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

		const std::vector<MenuItem>& getItems() const { return navigation.back().first; }
		const size_t getSelectedIdx() const { return navigation.back().second; }
		const bool isActive(const MenuItem item) { return settings.get(item.settingsKey, L"") == item.settingsValue; }

		virtual void overlayInit(Crib::Graphics::Context& context) override;
		virtual void overlayDraw() { if (renderer) renderer->render(); }
		virtual void onScreenResize(const float width, const float height) override { if (renderer) renderer->resize(width, height); }


	protected:
		std::unique_ptr<Renderer> renderer;

		std::wstring handleEvent(const Crib::Input::Event& e);
		std::wstring navigateTo(MenuItem& item);
		std::wstring navigateBack();
		const size_t findItem(const float x, const float y) const;

		std::vector<MenuItem> rootItems;
		size_t rootSel;
		std::vector<std::pair<std::vector<MenuItem>&, size_t&>> navigation;

		Crib::PersistentSettings& settings;


		void updateBoundingRect(std::vector<MenuItem>& items, const Renderer* rndr);

		static constexpr float lineSpacingNormal = 48.f;
		static constexpr float lineSpacingExtra = 67.f;
	};

}
