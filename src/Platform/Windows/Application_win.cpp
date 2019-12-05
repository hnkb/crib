
#include <Crib/Application.h>
#include <Crib/Platform/Windows.h>


int Crib::Application::messageLoop()
{
	MSG msg;

	if (EnumThreadWindows(GetCurrentThreadId(), [](HWND, LPARAM) { return FALSE; }, NULL))
		throw std::logic_error("Application needs at least one window.");

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (msg.message == Platform::Windows::Application::Message::WindowClosed && EnumThreadWindows(GetCurrentThreadId(), [](HWND, LPARAM) { return FALSE; }, NULL))
			PostQuitMessage(0);
	}

	return (int)msg.wParam;
}
