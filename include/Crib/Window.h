
#pragma once

#include "../src/Input/Buffer.h"
#include "../src/Input/Timer.h"
#include <memory>
#include <string>
#include "Platform/Windows.h"


namespace Crib
{

	namespace Graphics { class SceneBase; class Context; }
	class PersistentSettings;


	class Window
	{
	public:
		Window() : Window(L"crib", L"crib") {}
		Window(const std::wstring className, const std::wstring title);

		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		virtual ~Window();

		void setTitle(const std::wstring title);
		std::wstring getTitle() const;
		HWND getHandle() const { return handle; }

	protected:
		virtual void frame();
		virtual LRESULT proc(const UINT message, const WPARAM wParam, const LPARAM lParam);
		static LRESULT CALLBACK proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		void createGraphicsContext();

		HWND handle;
		std::unique_ptr<PersistentSettings> settings;
		std::unique_ptr<Graphics::SceneBase> scene;
		std::unique_ptr<Graphics::Context> graphics;
		Input::Buffer input;
		Timer timer;
	};

}
