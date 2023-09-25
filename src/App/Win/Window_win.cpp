
#include "App.h"
#include <crib/App>

using crib::App::Window;


namespace
{

	LRESULT CALLBACK proc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Window*)GetWindowLongPtrW(handle, GWLP_USERDATA);

		if (window)
		{
			switch (message)
			{
				case WM_SIZE:
					return 0;

				case WM_ERASEBKGND:
					// This handler is not necessary. The window class was created with
					// hbrBackground set to nullptr, so the default processing (by
					// DefWindowProcW) is to do nothing.
					return TRUE;

				case WM_PAINT:
					return 0;

				case WM_DESTROY:
					window->impl = nullptr;
					PostMessageW(nullptr, (UINT)crib::Platform::Win::Message::closed, 0, 0);
					return 0;
			}
		}

		return DefWindowProcW(handle, message, wParam, lParam);
	}

}


Window::Window() : Window(Options {})
{}

Window::Window(Options opt)
{
	if (opt.size.x <= 0)
		opt.size.x = CW_USEDEFAULT;
	if (opt.pos.x <= 0)
		opt.pos.x = CW_USEDEFAULT;
	if (opt.title.empty())
		opt.title = "crib";

	if (opt.size.x != CW_USEDEFAULT)
	{
		RECT rect { 0, 0, opt.size.x, opt.size.y };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		opt.size.x = rect.right - rect.left;
		opt.size.y = rect.bottom - rect.top;
	}

	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpszClassName = Platform::Win::windowClass;
	wcex.lpfnWndProc = proc;
	RegisterClassExW(&wcex);

	impl = CreateWindowEx(
		0,
		wcex.lpszClassName,
		Platform::Win::WideString(opt.title),
		WS_OVERLAPPEDWINDOW,
		opt.pos.x,
		opt.pos.y,
		opt.size.x,
		opt.size.y,
		nullptr,
		nullptr,
		wcex.hInstance,
		nullptr);

	if (!impl)
		throw Platform::Win::Error();  //"CreateWindow");

	SetWindowLongPtrW((HWND)impl, GWLP_USERDATA, LONG_PTR(this));

	ShowWindow((HWND)impl, SW_SHOWDEFAULT);

	{
		RECT rect;
		GetClientRect((HWND)impl, &rect);
		PostMessageW(
			(HWND)impl,
			WM_SIZE,
			SIZE_RESTORED,
			MAKELPARAM(rect.right - rect.left, rect.bottom - rect.top));
	}
}

Window::~Window()
{
	if (impl)
		DestroyWindow((HWND)impl);
}


Window::Options Window::getOptions() const
{
	return {};
}

Window& Window::setOptions(const Options& options)
{
	SetWindowTextW((HWND)impl, Platform::Win::WideString(options.title));
	return *this;
}