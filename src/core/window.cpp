
#include "stdafx.h"
#include "window.h"
#include "utility.h"

using crib::core::window;


window::window(const std::wstring className, const std::wstring title) : handle(nullptr), graphics(nullptr)
{
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpszClassName = className.c_str();
	wcex.lpfnWndProc = proc;
	RegisterClassExW(&wcex);

	handle = CreateWindowExW(0, wcex.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, wcex.hInstance, nullptr);
	if (!handle) throw windows_error("CreateWindow");

	SetWindowLongPtrW(handle, GWLP_USERDATA, LONG_PTR(this));
	ShowWindow(handle, SW_SHOWDEFAULT);

	create_graphics_context();
}

window::~window()
{
	if (handle)
	{
		DestroyWindow(handle);
	}
}


void window::frame()
{
	if (graphics)
	{
		try
		{
			graphics->draw();
		}
		catch (graphics::context_invalid e)
		{
			create_graphics_context();
		}
	}
}

LRESULT window::proc(const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) || (message >= WM_KEYFIRST && message <= WM_KEYLAST))
	{
		input.push(message, wParam, lParam, timer.now());
		return 0;
	}
	
	switch (message)
	{
	case WM_SIZE:
		if (graphics)
		{
			try
			{
				graphics->resize();
			}
			catch (graphics::context_invalid e)
			{
				create_graphics_context();
			}
		}
		return 0;

	case WM_ERASEBKGND:
		// This handler is not necessary. The window class was created with hbrBackground set to nullptr,
		// so the default processing (by DefWindowProcW) is to do nothing.
		return TRUE;

	case WM_PAINT:
		// To have animation, do not validate window, so Windows keep sending WM_PAINT messages
		frame();
		return 0;

	case WM_DESTROY:
		graphics.reset();
		handle = nullptr;
		PostMessageW(nullptr, constants::wm_app_windowclosed, 0, 0);
		return 0;
	}

	return DefWindowProcW(handle, message, wParam, lParam);
}

LRESULT CALLBACK window::proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto wnd = (window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	return (wnd && wnd->handle == hWnd) ? wnd->proc(message, wParam, lParam) : DefWindowProcW(hWnd, message, wParam, lParam);
}


void window::set_title(const std::wstring title)
{
	SetWindowTextW(handle, title.c_str());
}

std::wstring window::get_title() const
{
	std::vector<wchar_t> buffer(GetWindowTextLengthW(handle) + 2);
	GetWindowTextW(handle, buffer.data(), (int)buffer.size() - 1);
	return std::wstring(buffer.data());
}


void window::create_graphics_context()
{
	graphics = graphics::context::create(L"d3d11", handle);
}
