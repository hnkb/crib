
#include "stdafx.h"
#include "application.h"


int application::run()
{
	MSG msg;

	if (EnumThreadWindows(GetCurrentThreadId(), [](HWND, LPARAM) { return FALSE; }, NULL))
		throw std::logic_error("Application needs at least one window.");

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (msg.message == WM_APP_WINDOWCLOSED && EnumThreadWindows(GetCurrentThreadId(), [](HWND, LPARAM) { return FALSE; }, NULL))
			PostQuitMessage(0);
	}

	return (int)msg.wParam;
}
