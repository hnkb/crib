
#include <Crib/Window.h>
#include <Crib/Graphics/Direct3D11.h>
#include <Crib/Platform/Windows.h>

using namespace Crib::Platform::Windows;

using Crib::Window;


Window::Window(const std::wstring className, const std::wstring title) : settings(new Crib::PersistentSettings), handle(nullptr), graphics(nullptr)
{
	int width = settings->get(L"window.width", CW_USEDEFAULT),
		height = settings->get(L"window.height", 0),
		left = settings->get(L"window.left", CW_USEDEFAULT),
		top = settings->get(L"window.top", 0);
	if (width != CW_USEDEFAULT)
	{
		RECT rect { 0, 0, width, height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpszClassName = className.c_str();
	wcex.lpfnWndProc = proc;
	RegisterClassExW(&wcex);

	handle = CreateWindowExW(0, wcex.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, left, top, width, height, nullptr, nullptr, wcex.hInstance, nullptr);
	if (!handle) throw Error("CreateWindow");

	SetWindowLongPtrW(handle, GWLP_USERDATA, LONG_PTR(this));
	ShowWindow(handle, SW_SHOWDEFAULT);
	createGraphicsContext();
}

Window::~Window()
{
	if (handle)
	{
		DestroyWindow(handle);
	}
}


void Window::frame()
{
	if (scene && graphics)
	{
		try
		{
			graphics->draw();
		}
		catch (Graphics::Context::Invalid e)
		{
			createGraphicsContext();
		}
	}
}

LRESULT Window::proc(const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) || message == WM_KEYDOWN || message == WM_KEYUP)
	{
		if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN)
			SetCapture(handle);

		if (message == WM_LBUTTONUP || message == WM_RBUTTONUP || message == WM_MBUTTONUP)
			ReleaseCapture();

		input.push(message, wParam, lParam, timer.now());
		return 0;
	}

	switch (message)
	{
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(handle, &rect);
		settings->set(L"window.left", rect.left);
		settings->set(L"window.top", rect.top);
		break;
	}

	case WM_SIZE:
		if (wParam == SIZE_RESTORED)
		{
			settings->set(L"window.width", LOWORD(lParam));
			settings->set(L"window.height", HIWORD(lParam));
		}
		if (graphics)
		{
			try
			{
				graphics->resize();
			}
			catch (Graphics::Context::Invalid e)
			{
				createGraphicsContext();
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
		PostMessageW(nullptr, Application::Message::WindowClosed, 0, 0);
		return 0;
	}

	return DefWindowProcW(handle, message, wParam, lParam);
}

LRESULT CALLBACK Window::proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto wnd = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	return (wnd && wnd->handle == hWnd) ? wnd->proc(message, wParam, lParam) : DefWindowProcW(hWnd, message, wParam, lParam);
}


void Window::setTitle(const std::wstring title)
{
	SetWindowTextW(handle, title.c_str());
}

std::wstring Window::getTitle() const
{
	std::vector<wchar_t> buffer(GetWindowTextLengthW(handle) + 2);
	GetWindowTextW(handle, buffer.data(), (int)buffer.size() - 1);
	return std::wstring(buffer.data());
}


void Window::createGraphicsContext()
{
	graphics.reset(new Graphics::D3D11::Context(handle, *settings));
	if (scene) graphics->bind(scene.get());
}
