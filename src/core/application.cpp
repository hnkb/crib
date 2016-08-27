
#include "stdafx.h"
#include "application.h"


int application::run()
{
	MSG msg;
	int window_count = 0;

	// force message loop to quit immediately if no window exists
	PostMessageW(nullptr, WM_APP_WINDOWCOUNT, 0, 0);

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (msg.hwnd == nullptr && msg.message == WM_APP_WINDOWCOUNT)
		{
			do window_count += (int)msg.wParam; while (PeekMessageW(&msg, (HWND)-1, WM_APP_WINDOWCOUNT, WM_APP_WINDOWCOUNT, PM_REMOVE));
			if (window_count == 0) PostQuitMessage(0);
		}
	}

	return (int)msg.wParam;
}
