
#include "stdafx.h"
#include "window.h"
#include "utility.h"


window::window(const std::wstring className, const std::wstring title)
{
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpszClassName = className.c_str();
	wcex.lpfnWndProc = proc;
	if (!RegisterClassExW(&wcex)) throw windows_error("RegisterClass");

	handle = CreateWindowExW(0, wcex.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, wcex.hInstance, nullptr);
	if (!handle) throw windows_error("CreateWindow");

	PostMessageW(nullptr, WM_APP_WINDOWCOUNT, 1, 0);
	SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(handle, SW_SHOWDEFAULT);
}

window::~window()
{
	if (handle)
	{
		DestroyWindow(handle);
	}
}


LRESULT window::proc(const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		handle = nullptr;
		PostMessageW(nullptr, WM_APP_WINDOWCOUNT, -1, 0);
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
