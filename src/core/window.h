
#pragma once

#include "settings.h"
#include "timer.h"
#include "../input/buffer.h"
#include "../graphics/context.h"
#include "../scene/scene.h"
#include <memory>
#include <string>
#include <Windows.h>


namespace crib
{
	namespace core
	{

		class window
		{
		public:
			window(settings& setting) : window(setting, L"crib", L"crib") {}
			window(settings& setting, const std::wstring className, const std::wstring title);

			window(const window& other) = delete;
			window(window&& other) = delete;
			window& operator=(const window& other) = delete;
			window& operator=(window&& other) = delete;

			virtual ~window();

			void set_title(const std::wstring title);
			std::wstring get_title() const;
			HWND get_handle() const { return handle; }

		protected:
			void frame();
			virtual LRESULT proc(const UINT message, const WPARAM wParam, const LPARAM lParam);
			static LRESULT CALLBACK proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			void create_scene(std::wstring name);
			void create_graphics_context();

			HWND handle;
			settings& settings;
			std::unique_ptr<scene::scene> scene;
			std::unique_ptr<graphics::base::context> graphics;
			input::buffer input;
			timer timer;
		};


		namespace constants
		{
			constexpr UINT wm_app_windowclosed = (WM_APP + 1);
		}
	}
}
