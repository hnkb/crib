
#include "stdafx.h"
#include "application.h"

using crib::core::application;


int application::run()
{
	MSG msg;

	if (EnumThreadWindows(GetCurrentThreadId(), [](HWND, LPARAM) { return FALSE; }, NULL))
		throw std::logic_error("Application needs at least one window.");

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (msg.message == constants::wm_app_windowclosed && EnumThreadWindows(GetCurrentThreadId(), [](HWND, LPARAM) { return FALSE; }, NULL))
			PostQuitMessage(0);
	}

	return (int)msg.wParam;
}
