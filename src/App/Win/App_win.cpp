
#include "App.h"
#include <crib/App>


int crib::App::run()
{
	MSG msg;

	if (EnumThreadWindows(
			GetCurrentThreadId(),
			[](HWND, LPARAM) { return FALSE; },
			0))
		throw std::logic_error("[crib::App::run] At least one window is required.");

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if ((msg.message == (UINT)Platform::Win::Message::quit)
			|| (msg.message == (UINT)Platform::Win::Message::closed
				&& EnumThreadWindows(
					GetCurrentThreadId(),
					[](HWND, LPARAM) { return FALSE; },
					0)))
			PostQuitMessage(0);
	}

	return (int)msg.wParam;
}
