
#include "stdafx.h"
#include "application.h"


int application::run()
{
	MSG msg;

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}
