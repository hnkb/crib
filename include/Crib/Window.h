
#pragma once

#include "../src/Input/Buffer.h"
#include "../src/Input/Timer.h"
#include <memory>
#include <string>
#include "Platform/Windows.h"


namespace crib
{
	namespace graphics { namespace base { class context; } }
	namespace scene { class scene; }

	namespace core
	{
		class settings;

		class window
		{
		public:
			window() : window(L"crib", L"crib") {}
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
			std::unique_ptr<settings> settings;
			std::unique_ptr<scene::scene> scene;
			std::unique_ptr<graphics::base::context> graphics;
			input::buffer input;
			timer timer;
		};

	}
}
