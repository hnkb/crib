
#pragma once

#include "scene/scene.h"
#include "graphics/context.h"
#include "input/buffer.h"
#include "timer.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <Windows.h>


namespace crib
{
	namespace core
	{

		class window
		{
		public:
			window() : window(L"crib") {}
			window(const std::wstring title) : window(L"crib", title) {}
			window(const std::wstring className, const std::wstring title);

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
			std::unique_ptr<scene::scene> scene;
			std::unique_ptr<graphics::context> graphics;
			input::buffer input;
			timer timer;
		};


		namespace constants
		{
			constexpr UINT wm_app_windowclosed = (WM_APP + 1);
		}
	}
}
